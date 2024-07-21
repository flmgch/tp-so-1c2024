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

// FUNCIONES AUXILIARES

char *crear_path(char *path_base, char *nombre_archivo)
{
    char *path_absoluto = malloc(strlen(path_base) + strlen(nombre_archivo) + 2);
    sprintf(path_absoluto, "%s/%s", path_base, nombre_archivo);

    return path_absoluto;
}

int buscar_bloques_libres_contiguos(int bloque_inicial, int bloques_actuales, int bloques_necesarios, t_bitarray *bitarray)
{
    int bloques_libres_contiguos = 0;

    for (int i = bloque_inicial + bloques_actuales; i < cantidad_bloque; i++)
    {
        if (bitarray_test_bit(bitmap->bitarray, i) == 0) 
        {
            bloques_libres_contiguos++;
            if (bloques_libres_contiguos == bloques_necesarios) 
            {
                return bloques_libres_contiguos; // Encontre los bloques libres, entonces salgo del bucle
            }
        } else 
        {
            break; // No encontre suficientes bloques libres, entonces salgo del bucle
        }
    }
    return bloques_libres_contiguos; // No encontre suficientes bloques libres
}

// SISTEMA DE ARCHIVOS 

void iniciar_bitmap(){

    bitmap = malloc(sizeof(t_bitmap));

    bitmap->tamanio = (cantidad_bloque / 8);

    char *path_bitmap = crear_path(path_base_dialfs, "bitmap.dat");

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

    char *path_bloques_datos = crear_path(path_base_dialfs, "bloques.dat");

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

// FUNCIONES DEL FILESYSTEM

void atender_fs_create(t_buffer *buffer)
{   
    usleep(tiempo_unidad_trabajo * 1000);

    u_int32_t pid = extraer_uint32_de_buffer(buffer);
    char *nombre_archivo = extraer_string_de_buffer(buffer);
    
    log_info(io_logger, "PID: %d - Crear Archivo: %s", pid, nombre_archivo);

    char *path_metadata = crear_path(path_base_dialfs, nombre_archivo);
    
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
        FILE *config_aux = fopen(path_metadata, "wb+");
        fclose(config_aux);
        metadata_config = config_create(path_metadata);
    }

    // Escribo la informacion en el archivo de metadata
    config_set_value(metadata_config, "BLOQUE_INICIAL", string_itoa(bloque_libre));
    config_set_value(metadata_config, "TAMANIO_ARCHIVO", "0");
    config_save(metadata_config);

    list_add(lista_metadatas, nombre_archivo);

    // Le aviso a Kernel que ya termine la operacion
    t_buffer *un_buffer = crear_buffer();
    agregar_uint32_a_buffer(un_buffer, pid);
    t_paquete *paquete = crear_super_paquete(FIN_INSTRUCCION_INTERFAZ, un_buffer);
    enviar_paquete(paquete, socket_kernel);

    log_info(io_logger, "Operacion FS_CREATE con %s finalizada", nombre_archivo);

    eliminar_paquete(paquete);
    config_destroy(metadata_config);
    free(path_metadata);
}

void atender_fs_delete(t_buffer *buffer)
{
    usleep(tiempo_unidad_trabajo * 1000);

    u_int32_t pid = extraer_uint32_de_buffer(buffer);
    char *nombre_archivo = extraer_string_de_buffer(buffer);

    log_info(io_logger, "PID: %d - Eliminar Archivo: %s", pid, nombre_archivo);

    char *path_metadata = crear_path(path_base_dialfs, nombre_archivo);
    
    // Creo un config para manejar el metadata
    t_config *metadata_config = config_create(path_metadata);

    int bloque_inicial = atoi(config_get_string_value(metadata_config, "BLOQUE_INICIAL"));
    int tamanio_metadata = atoi(config_get_string_value(metadata_config, "TAMANIO_ARCHIVO"));

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
    t_paquete *paquete = crear_super_paquete(FIN_INSTRUCCION_INTERFAZ, un_buffer);
    enviar_paquete(paquete, socket_kernel);

    log_info(io_logger, "Operacion FS_DELETE con %s finalizada", nombre_archivo);

    eliminar_paquete(paquete);
    remove(path_metadata);
    config_destroy(metadata_config);
}

void realizar_compactacion(int *bloque_inicial_trunc, t_config *metadata_config_trunc, char* nombre_archivo_trunc, u_int32_t pid)
{   
    log_info(io_logger, "PID: %d - Inicio Compactación.", pid);

    int i; // Contador general
    int j; // Contador de bloques asignados
    int bloque_libre = 0; // Posiciones de inicio de los archivos

    char *path_metadata;

    // Pongo todo el bitarray en 0
    for(i = 0; i < cantidad_bloque; i++)
    {
        bitarray_clean_bit(bitmap->bitarray, i);
    }

    // Reasigno los archivos que no sean el archivo a truncar
    for(i = 0; i < list_size(lista_metadatas); i++)
    {
        char *nombre_archivo = list_get(lista_metadatas, i);
        path_metadata = crear_path(path_base_dialfs, nombre_archivo);

        // Si se trata del archivo truncado, lo dejo para lo ultimo
        if(strcmp(nombre_archivo, nombre_archivo_trunc) == 0) 
        { 
            continue; // Es para que salte a la siguiente iteracion sin tener que englobar todo lo siguiente en un if(strcmp(nombre_archivo,nombre_archivo_trunc)==1)
        }

        t_config *metadata_config = config_create(path_metadata);
        if (metadata_config == NULL) 
        {
            perror("Error al abrir el archivo de metadata para compactar");
            exit(EXIT_FAILURE);
        }

        int tamanio_archivo = atoi(config_get_string_value(metadata_config,"TAMANIO_ARCHIVO"));
        int bloques_necesarios = (tamanio_archivo + tamanio_bloque - 1) / tamanio_bloque;

        for(j = 0; j < bloques_necesarios; j++) // Asigno los bloques necesarios...
        {                      
            bitarray_set_bit(bitmap->bitarray, bloque_libre + j); // Partiendo de la posicion del bloque libre
        }

        // Actualizo el bloque inicial del metadata de cada archivo
        config_set_value(metadata_config, "BLOQUE_INICIAL", string_itoa(bloque_libre));
        config_save(metadata_config);
        config_destroy(metadata_config);

        bloque_libre = bloque_libre + bloques_necesarios; // Actualizo la posicion de inicio del proximo archivo
    }

    int tamanio_original_trunc = atoi(config_get_string_value(metadata_config_trunc, "TAMANIO_ARCHIVO"));
    int bloques_necesarios_trunc = (tamanio_original_trunc + tamanio_bloque - 1) / tamanio_bloque;

    for (j = 0; j < bloques_necesarios_trunc; j++) // Asigno los bloques del archivo a truncar al final de todo
    {
        bitarray_set_bit(bitmap->bitarray, bloque_libre + j);
    }

    // Actualizo los datos del archivo a truncar
    config_set_value(metadata_config_trunc, "BLOQUE_INICIAL", string_itoa(bloque_libre));
    config_save(metadata_config_trunc);
    *bloque_inicial_trunc = bloque_libre;

    // Sincronizo la reasignacion
    if (msync(bitmap->direccion, bitmap->tamanio, MS_SYNC) == -1)
    {
        perror("Error al sincronizar el bitmap despues de la compactacion");
        exit(EXIT_FAILURE);
    }

    usleep(retraso_compactacion * 1000);

    log_info(io_logger, "PID: %d - Fin Compactación.", pid);
}

void atender_fs_truncate(t_buffer *buffer)
{
    usleep(tiempo_unidad_trabajo * 1000);

    u_int32_t pid = extraer_uint32_de_buffer(buffer);
    char *nombre_archivo = extraer_string_de_buffer(buffer);
    int tamanio = extraer_int_de_buffer(buffer);

    log_info(io_logger, "PID: %d - Truncar Archivo: %s - Tamaño: %d", pid, nombre_archivo, tamanio);

    char *path_metadata = crear_path(path_base_dialfs, nombre_archivo);

    // Creo el config para manejar el metadata
    t_config *metadata_config = config_create(path_metadata);
    if (metadata_config == NULL) 
    {
        perror("Error al abrir el archivo de metadata");
        exit(EXIT_FAILURE);
    }

    int bloque_inicial = atoi(config_get_string_value(metadata_config, "BLOQUE_INICIAL"));
    int tamanio_actual = atoi(config_get_string_value(metadata_config, "TAMANIO_ARCHIVO"));

    int bloques_actuales = (tamanio_actual + tamanio_bloque - 1) / tamanio_bloque; // Cantidad de bloques ocupados actualmente
    int bloques_necesarios = (tamanio + tamanio_bloque - 1) / tamanio_bloque; // Cantidad de bloques que ocuparia despues del truncate

    ////////// EN CASO DE QUE AUMENTE EL TAMANIO DEL ARCHIVO
    if(bloques_necesarios > bloques_actuales) 
    {
        int bloques_incremento = bloques_necesarios - bloques_actuales;

        // Busco comprobar si, al final del archivo, existen suficientes bloques libres o no
        // Si es que existieran, se los asignan en la funcion buscar_bloques_libres_contiguos
        int bloques_libres_contiguos_1 = buscar_bloques_libres_contiguos(bloque_inicial, bloques_actuales, bloques_necesarios, bitmap->bitarray); 
        if (bloques_libres_contiguos_1 < bloques_incremento) 
        {
            // Esta funcion retira el archivo, compacta el resto de archivos y al mismo le asigna un nuevo bloque inicial
            realizar_compactacion(&bloque_inicial, metadata_config, nombre_archivo, pid);

            // CASO BORDER: Compruebo si, incluso despues de la compactacion, siguen sin haber bloques libres contiguos
            int bloques_libres_contiguos_2 = buscar_bloques_libres_contiguos(bloque_inicial, bloques_actuales, bloques_necesarios, bitmap->bitarray);
            if (bloques_libres_contiguos_2 < bloques_incremento) 
            {
                perror("No se encontraron suficientes bloques contiguos incluso despues de la compactacion");
                free(nombre_archivo);
                free(path_metadata);
                exit(EXIT_FAILURE);
            }
        }
        // Ahora si lo agrando :)
        // Rango: Bloque siguiente al ultimo (bloque_inicial + bloques_actuales) - incremento a partir del bloque siguiente al ultimo (bloque_inicial + bloques_actuales + bloques_incremento)
        for(int i = bloque_inicial + bloques_actuales; i < bloque_inicial + bloques_actuales + bloques_incremento; i++)
        {
            bitarray_set_bit(bitmap->bitarray, i);
        }

    } 
    ////////// EN CASO DE QUE ACHIQUE EL TAMANIO DEL ARCHIVO
    else if (bloques_necesarios < bloques_actuales) 
    { 
        int nuevo_tamanio = bloque_inicial + bloques_necesarios;
        int antiguo_tamanio =  bloque_inicial + bloques_actuales;
        // Libero los bloques => Pongo en 0 los bits del nuevo tamanio al antiguo tamanio
        for (int i = nuevo_tamanio; i < antiguo_tamanio; i++)
        {
            bitarray_clean_bit(bitmap->bitarray, i);
        }
    }

    // Sincronizo el bitmap   
    if (msync(bitmap->direccion, bitmap->tamanio, MS_SYNC) == -1)
    {
        perror("Error al sincronizar el bitmap");
        exit(EXIT_FAILURE);
    }

    // Actualizo el archivo del metadata
    config_set_value(metadata_config, "TAMANIO_ARCHIVO", string_itoa(tamanio));
    config_save(metadata_config);
    config_destroy(metadata_config);

    // Le aviso a kernel que ya termine operación
    t_buffer *un_buffer = crear_buffer();
    agregar_uint32_a_buffer(un_buffer, pid);
    t_paquete *paquete = crear_super_paquete(FIN_INSTRUCCION_INTERFAZ, un_buffer);
    enviar_paquete(paquete, socket_kernel);

    log_info(io_logger, "Operacion FS_TRUNCATE con %s finalizada", nombre_archivo);

    eliminar_paquete(paquete);
}

void atender_fs_write(t_buffer *buffer)
{
    //
}

void atender_fs_read(t_buffer *buffer)
{
    //
}

