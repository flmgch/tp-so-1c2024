#include "dialfs.h"

// VARIABLES GLOBALES
//t_log *io_logger;
//t_config *io_config;
//char *ip_kernel, *puerto_kernel, *ip_memoria, *puerto_memoria, *tipo_interfaz, *path_base_dialfs;
//int tiempo_unidad_trabajo, tamanio_bloque, cantidad_bloque, retraso_compactacion;

//VARIABLES ARCHIVOS DE BLOQUES
//int tamanio_archivo_bloques;
//void* buffer_bitmap;
//void* buffer_bloques;

//typedef struct
///{
//    void* direccion_bitmap;
//    int descriptor_bitmap;
//    int tamanio_bitmap;
//    t_bitarray *bitarray;
//} t_bitmap;

//typedef struct
//{
//    void *direccion_bloques_datos;
//    int descriptor_bloques_datos;
//} t_bloques_datos

// VARIABLES GLOBALES
t_bitmap *bitmap;
t_bloques_datos *bloques_datos;
t_list *lista_metadatas;

void iniciar_bitmap(){

    bitmap = malloc(sizeof(t_bitmap));

    bitmap->tamanio = (cantidad_bloque / 8);

    char path_bitmap[strlen(path_base_dialfs) + 12]; 
    sprintf(path_bitmap, "%s/bitmap.dat", path_base_dialfs);

	if(access(path_bitmap, F_OK) == -1)
    {
        bitmap->file_descriptor = open(path_bitmap, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        if(bitmap->file_descriptor == -1)
        {
            perror("Error al abrir bitmap.dat");
		    exit(EXIT_FAILURE);
        }
        if(ftruncate(bitmap->file_descriptor, bitmap->tamanio) == -1)
        {
            perror("Error al truncar bitmap.dat");
            exit(EXIT_FAILURE); 
        }
	} else
    {
        bitmap->file_descriptor = open(path_bitmap, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    }

	bitmap->direccion = mmap(NULL, bitmap->tamanio, PROT_READ | PROT_WRITE, MAP_SHARED, bitmap->file_descriptor, 0);
    
    if(bitmap->direccion == MAP_FAILED)
    {
        perror("Error al mapear bitmap.dat");
        exit(EXIT_FAILURE);
    }

	bitmap->bitarray = bitarray_create_with_mode(bitmap->direccion, bitmap->tamanio, LSB_FIRST);
}

void iniciar_bloques_datos()
{
    bloques_datos = malloc(sizeof(t_bloques_datos));

    char path_bloques_datos[strlen(path_base_dialfs) + 13];
    sprintf(path_bloques_datos, "%s/bloques.dat", path_base_dialfs);

    if (access(path_bloques_datos, F_OK) == -1) 
    {
        bloques_datos->file_descriptor = open(path_bloques_datos, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        if (bloques_datos->file_descriptor == -1) 
        {
            perror("Error al abrir bloques.dat");
            exit(EXIT_FAILURE);
        }
        if (ftruncate(bloques_datos->file_descriptor, cantidad_bloque * tamanio_bloque) == -1) 
        {
            perror("Error al truncar bloques.dat");
            exit(EXIT_FAILURE);
        }
    } else 
    {
        bloques_datos->file_descriptor = open(path_bloques_datos, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    }

    bloques_datos->direccion = mmap(NULL, cantidad_bloque * tamanio_bloque, PROT_READ | PROT_WRITE, MAP_SHARED, bloques_datos->file_descriptor, 0);
    
    if(bloques_datos->direccion == MAP_FAILED)
    {
        perror("Error al mapear bloques.dat");
        exit(EXIT_FAILURE);
    }
}

void cargar_metadatas()
{
    DIR *directorio;
    struct dirent *entrada;
    lista_metadatas = list_create();

    if((directorio = opendir(path_base_dialfs)) == NULL)
    {
        perror("Error al abrir el directorio");
        exit(EXIT_FAILURE);
    } 

    while((entrada = readdir(directorio)) != NULL)
    {
        if(entrada->d_type == DT_REG)
        {
            if(strstr(entrada->d_name, ".txt") != NULL)
            {
                char *nombre_archivo = strdup(entrada->d_name);
                list_add(lista_metadatas, nombre_archivo);
            }
        }
    }
    closedir(directorio);
}

void iniciar_sistema_archivos(){
    iniciar_bloques_datos();
    iniciar_bitmap();
    cargar_metadatas();

    log_info(io_logger, "Sistema de archivos inicializado correctamente");
}

void atender_fs_create(t_buffer *buffer)
{   
    usleep(tiempo_unidad_trabajo * 1000);

    u_int32_t pid = extraer_uint32_de_buffer(buffer);
    char *nombre_archivo = extraer_string_de_buffer(buffer);
    
    log_info(io_logger, "PID: %d - Crear Archivo: %s", pid, nombre_archivo);

    char path_metadata[strlen(path_base_dialfs) + strlen(nombre_archivo) + 2];
    sprintf(path_metadata, "%s/%s", path_base_dialfs, nombre_archivo);
    
    // Busco un bloque libre en el bitmap
    int bloque_libre = -1;
    for (int i = 0; i < cantidad_bloque; i++) {
        if (!bitarray_test_bit(bitmap->bitarray, i)) {
            bloque_libre = i;
            bitarray_set_bit(bitmap->bitarray, i);
            break;
        }
    }

    if (bloque_libre == -1) 
    {
        perror("No se encontraron bloques libres en el bitmap");
        exit(EXIT_FAILURE);
    }

    // Sincronizo los cambios con el bitmap.dat
    if (msync(bitmap->direccion, bitmap->tamanio, MS_SYNC) == -1) 
    {
        perror("Error al sincronizar el bitmap");
        exit(EXIT_FAILURE);
    }

    // Creo un config para poder manejar el metadata
    t_config *metadata_config = config_create(path_metadata);
    if (metadata_config == NULL) 
    {
        perror("Error al crear el archivo de metadata");
        exit(EXIT_FAILURE);
    }

    // Escribo la informacion en el archivo de metadata
    config_set_value(metadata_config, "BLOQUE_INICIAL", string_itoa(bloque_libre));
    config_set_value(metadata_config, "TAMANIO_ARCHIVO", "0");
    config_save(metadata_config);
    config_destroy(metadata_config);

    list_add(lista_metadatas, nombre_archivo);

    // Le aviso a Kernel que ya termine la operacion
    t_buffer *un_buffer = crear_buffer();
    agregar_uint32_a_buffer(un_buffer, pid);
    t_paquete *paquete = crear_paquete();
    crear_super_paquete(FIN_INSTRUCCION_INTERFAZ, un_buffer);
    enviar_paquete(paquete, socket_kernel);

    log_info(io_logger, "Operacion FS_CREATE con %s finalizada", nombre_archivo);

    eliminar_paquete(paquete);
}

void atender_fs_delete(t_buffer *buffer)
{
    usleep(tiempo_unidad_trabajo * 1000);

    u_int32_t pid = extraer_uint32_de_buffer(buffer);
    char *nombre_archivo = extraer_string_de_buffer(buffer);

    log_info(io_logger, "PID: %d - Eliminar Archivo: %s", pid, nombre_archivo);

    char path_metadata[strlen(path_base_dialfs) + strlen(nombre_archivo) + 2];
    sprintf(path_metadata, "%s/%s", path_base_dialfs, nombre_archivo);
    
    // Creo un config para manejar el metadata
    t_config *metadata_config = config_create(path_metadata);

    char *bloque_inicial_aux = config_get_string_value(metadata_config, "BLOQUE_INICIAL");
    char *tamanio_metadata_aux = config_get_string_value(metadata_config, "TAMANIO_ARCHIVO");

    int bloque_inicial = atoi(bloque_inicial_aux);
    int tamanio_metadata = atoi(tamanio_metadata_aux);

    // Libero todos los bloques ocupados por el metadata
    int bloques_ocupados = (tamanio_metadata + tamanio_bloque - 1) / tamanio_bloque; // Redondeo para arriba
    for (int i = 0; i < bloques_ocupados; i++)
    {
        int bloque_actual = bloque_inicial + i;
        bitarray_clean_bit(bitmap->bitarray, bloque_actual);
    }

    // Sincronizo los cambios con el bitmap.dat
    if (msync(bitmap->direccion, bitmap->tamanio, MS_SYNC) == -1) 
    {
        perror("Error al sincronizar el bitmap");
        exit(EXIT_FAILURE);
    }

    // Actualizo la lista de metadatas
    int tamanio_lista = list_size(lista_metadatas);
    for (int i = 0; i < tamanio_lista; i++) 
    {
        char *nombre_metadata = list_get(lista_metadatas, i);
        if (strcmp(nombre_metadata, nombre_archivo) == 0) {
            list_remove_and_destroy_element(lista_metadatas, i, free);
            break;
        }
    }

    // Le aviso a Kernel que ya termine la operacion
    t_buffer *un_buffer = crear_buffer();
    agregar_uint32_a_buffer(un_buffer, pid);
    t_paquete *paquete = crear_paquete();
    crear_super_paquete(FIN_INSTRUCCION_INTERFAZ, un_buffer);
    enviar_paquete(paquete, socket_kernel);

    log_info(io_logger, "Operacion FS_DELETE con %s finalizada", nombre_archivo);

    eliminar_paquete(paquete);
    remove(path_metadata);
    config_destroy(metadata_config);
}

void atender_fs_truncate(t_buffer *buffer)
{
    //
}

void atender_fs_write(t_buffer *buffer)
{
    //
}

void atender_fs_read(t_buffer *buffer)
{
    //
}

