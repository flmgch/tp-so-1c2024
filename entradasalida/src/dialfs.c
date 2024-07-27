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

// typedef struct
// {
//     void* direccion; 
//     int file_descriptor;
//     int tamanio;
//     t_bitarray *bitarray;
// } t_bitmap;

// typedef struct
// {
//     void *direccion;
//     int file_descriptor;
// } t_bloques_datos;

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

//////////////////// FS_CREATE

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

//////////////////// FS_DELETE

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

//////////////////// FS_TRUNCATE

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
                break; // Encontre los bloques libres, entonces salgo del bucle
            }
        } else 
        {
            break; // No encontre suficientes bloques libres, entonces salgo del bucle
        }
    }

    return bloques_libres_contiguos;
}


void realizar_compactacion(int *bloque_inicial_trunc, t_config *metadata_config_trunc, char* nombre_archivo_trunc, u_int32_t pid)
{   
    log_info(io_logger, "PID: %d - Inicio Compactación.", pid);

    int i; // Contador general
    int j; // Contador de bloques asignados
    int bloque_libre = 0; // Posiciones de inicio de los archivos

    // Pongo todo el bitarray en 0
    for(i = 0; i < cantidad_bloque; i++)
    {
        bitarray_clean_bit(bitmap->bitarray, i);
    }

    log_info(io_logger, "Reasignando el resto de archivos...");
    // Reasigno los archivos que no sean el archivo a truncar
    for(i = 0; i < list_size(lista_metadatas); i++)
    {
        char *nombre_archivo = list_get(lista_metadatas, i);
        char *path_metadata = crear_path(path_base_dialfs, nombre_archivo);

        // Si se trata del archivo truncado, lo dejo para lo ultimo
        if(strcmp(nombre_archivo, nombre_archivo_trunc) == 0) 
        { 
            free(path_metadata);
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
        if (bloques_necesarios == 0)
        {
            bloques_necesarios = 1;
        }

        for(j = 0; j < bloques_necesarios; j++) // Asigno los bloques necesarios...
        {                  
            bitarray_set_bit(bitmap->bitarray, bloque_libre + j); // Partiendo de la posicion del bloque libre
        }

        // Actualizo el bloque inicial del metadata de cada archivo
        config_set_value(metadata_config, "BLOQUE_INICIAL", string_itoa(bloque_libre));
        config_save(metadata_config);
        config_destroy(metadata_config);

        bloque_libre = bloque_libre + bloques_necesarios; // Actualizo la posicion de inicio del proximo archivo

        free(path_metadata);
    }

    int tamanio_original_trunc = atoi(config_get_string_value(metadata_config_trunc, "TAMANIO_ARCHIVO"));
    int bloques_necesarios_trunc = (tamanio_original_trunc + tamanio_bloque - 1) / tamanio_bloque;
    if (bloques_necesarios_trunc == 0)
    {
        bloques_necesarios_trunc = 1;
    }

    log_info(io_logger, "Reasignando el archivo a ampliar...");
    // Asigno los bloques del archivo a truncar al final de todo
    for (j = 0; j < bloques_necesarios_trunc; j++) 
    {
        bitarray_set_bit(bitmap->bitarray, bloque_libre + j);
    }

    // Sincronizo la reasignacion
    if (msync(bitmap->direccion, bitmap->tamanio, MS_SYNC) == -1)
    {
        perror("Error al sincronizar el bitmap despues de la compactacion");
        exit(EXIT_FAILURE);
    }    

    // Actualizo los datos del archivo a truncar
    config_set_value(metadata_config_trunc, "BLOQUE_INICIAL", string_itoa(bloque_libre));
    config_save(metadata_config_trunc);

    *bloque_inicial_trunc = bloque_libre;

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
    if (bloques_actuales == 0)
    {
        bloques_actuales = 1;
    }
    
    int bloques_necesarios = (tamanio + tamanio_bloque - 1) / tamanio_bloque; // Cantidad de bloques que ocuparia despues del truncate

    ////////// EN CASO DE QUE AUMENTE EL TAMANIO DEL ARCHIVO
    if (tamanio > tamanio_actual)
    {
        log_info(io_logger, "Se quiere ampliar el archivo");

        int bloques_incremento = bloques_necesarios - bloques_actuales;

        // Busco comprobar si, al final del archivo, existen suficientes bloques libres o no
        int bloques_libres_contiguos_1 = buscar_bloques_libres_contiguos(bloque_inicial, bloques_actuales, bloques_necesarios, bitmap->bitarray); 

        if (bloques_libres_contiguos_1 >= bloques_incremento) 
        {
            log_info(io_logger, "No se requiere compactar el archivo");
        } 
        else if (bloques_libres_contiguos_1 < bloques_incremento) 
        {
            log_info(io_logger, "Se requiere compactar el archivo");
            // Esta funcion retira el archivo, compacta el resto de archivos y al mismo le asigna un nuevo bloque inicial
            realizar_compactacion(&bloque_inicial, metadata_config, nombre_archivo, pid);

            // CASO BORDE: Compruebo si, incluso despues de la compactacion, siguen sin haber bloques libres contiguos
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
        log_info(io_logger, "Ampliando el archivo...");
        for(int i = bloque_inicial + bloques_actuales; i < bloque_inicial + bloques_actuales + bloques_incremento; i++)
        {
            bitarray_set_bit(bitmap->bitarray, i);
        }
    } 
    ////////// EN CASO DE QUE ACHIQUE EL TAMANIO DEL ARCHIVO
    else if (tamanio < tamanio_actual)
    { 
        log_info(io_logger, "Se quiere reducir el archivo");

        int nuevo_tamanio = bloque_inicial + bloques_necesarios;
        int antiguo_tamanio =  bloque_inicial + bloques_actuales;
        // Libero los bloques => Pongo en 0 los bits del nuevo tamanio al antiguo tamanio
        log_info(io_logger, "Reduciendo el archivo...");
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

//////////////////// FS_WRITE

io_fs fs_write;

void atender_fs_write(t_buffer *buffer)
{
    usleep(tiempo_unidad_trabajo * 1000);
    fs_write.pid = extraer_uint32_de_buffer(buffer);
    fs_write.nombre_archivo = extraer_string_de_buffer(buffer);
    fs_write.lista_direcciones = extraer_lista_direcciones_de_buffer(buffer);
    fs_write.tamanio = extraer_uint32_de_buffer(buffer);
    fs_write.puntero_archivo = extraer_uint32_de_buffer(buffer);

    log_info(io_logger, "PID: %d - Escribir Archivo: %s - Tamaño a Escribir: %d - Puntero Archivo: %d", fs_write.pid, fs_write.nombre_archivo, fs_write.tamanio, fs_write.puntero_archivo);

    // Le aviso a Memoria para leer 
    t_buffer *un_buffer = crear_buffer();
    agregar_uint32_a_buffer(un_buffer, fs_write.pid);
    agregar_lista_direcciones_a_buffer(un_buffer, fs_write.lista_direcciones);
    agregar_uint32_a_buffer(un_buffer, fs_write.tamanio);
    t_paquete *paquete = crear_super_paquete(ACCESO_ESPACIO_USUARIO_LECTURA, un_buffer);
    enviar_paquete(paquete, socket_memoria);

    eliminar_paquete(paquete);
}

void escribir_archivo(t_buffer *buffer)
{
    u_int32_t tamanio = extraer_uint32_de_buffer(buffer);
    void *texto_aux = extraer_de_buffer(buffer);

    char *texto = malloc(tamanio);
    memcpy(texto, texto_aux, tamanio);
    // Le agrego el caracter nulo 
    texto[tamanio] = '\0';

    // Leo lo necesario del archivo de metadata
    char *path_metadata = crear_path(path_base_dialfs, fs_write.nombre_archivo);
    t_config *metadata_config = config_create(path_metadata);
    int bloque_inicial = atoi(config_get_string_value(metadata_config, "BLOQUE_INICIAL"));
    
    log_info(io_logger, "Escribiendo en el archivo %s...", fs_write.nombre_archivo);

    // Hago los calculos necesarios para saber desde donde escribir en bloques.dat
    int posicion_inicial = tamanio_bloque * bloque_inicial;
    int posicion_desplazada = posicion_inicial + fs_write.puntero_archivo;

    memcpy(bloques_datos->direccion + posicion_desplazada, texto, tamanio);

    if (msync(bloques_datos->direccion, cantidad_bloque * tamanio_bloque, MS_SYNC) == -1)
    {
        perror("Error al sincronizar el archivo de bloques");
        free(texto);
        free(path_metadata);
        config_destroy(metadata_config);
        exit(EXIT_FAILURE);
    }

    // Le aviso a Kernel que ya termine la operacion
    t_buffer *un_buffer = crear_buffer();
    agregar_uint32_a_buffer(un_buffer, fs_write.pid);
    t_paquete *paquete = crear_super_paquete(FIN_INSTRUCCION_INTERFAZ, un_buffer);
    enviar_paquete(paquete, socket_kernel);

    log_info(io_logger, "Operacion FS_WRITE con %s finalizada", fs_write.nombre_archivo);

    free(texto);
    free(path_metadata);
    config_destroy(metadata_config);
    eliminar_paquete(paquete);
}

//////////////////// FS_READ

io_fs fs_read;

void atender_fs_read(t_buffer *buffer)
{
    usleep(tiempo_unidad_trabajo * 1000);

    fs_read.pid = extraer_uint32_de_buffer(buffer);
    fs_read.nombre_archivo = extraer_string_de_buffer(buffer);
    fs_read.lista_direcciones = extraer_lista_direcciones_de_buffer(buffer);
    fs_read.tamanio = extraer_uint32_de_buffer(buffer);
    fs_read.puntero_archivo = extraer_uint32_de_buffer(buffer);

    log_info(io_logger, "PID: %d - Leer Archivo: %s - Tamaño a Leer: %d - Puntero Archivo: %d", fs_read.pid, fs_read.nombre_archivo, fs_read.tamanio, fs_read.puntero_archivo);
    
    // Leo lo necesario del archivo de metadata
    char *path_metadata = crear_path(path_base_dialfs, fs_read.nombre_archivo);
    t_config *metadata_config = config_create(path_metadata);
    int bloque_inicial = atoi(config_get_string_value(metadata_config, "BLOQUE_INICIAL"));
    
    log_info(io_logger, "Leyendo el archivo %s...", fs_read.nombre_archivo);

    // Hago los calculos necesarios para saber desde donde escribir en bloques.dat
    int posicion_inicial = tamanio_bloque * bloque_inicial;
    int posicion_desplazada = posicion_inicial + fs_read.puntero_archivo;

    char *texto = malloc(fs_read.tamanio);
    void *texto_aux = malloc(fs_read.tamanio);

    memcpy(texto, bloques_datos->direccion + posicion_desplazada, fs_read.tamanio);

    memcpy(texto_aux, texto, fs_read.tamanio);

    // Le pido a Memoria que escriba el texto a partir de las direcciones
    t_buffer *un_buffer = crear_buffer();
    agregar_uint32_a_buffer(un_buffer, fs_read.pid);
    agregar_a_buffer(un_buffer, texto_aux, fs_read.tamanio);
    agregar_lista_direcciones_a_buffer(un_buffer, fs_read.lista_direcciones);
    t_paquete *paquete = crear_super_paquete(ACCESO_ESPACIO_USUARIO_ESCRITURA, un_buffer);
    enviar_paquete(paquete, socket_memoria);

    eliminar_paquete(paquete);

    free(texto);
    free(path_metadata);
    config_destroy(metadata_config);
}

void confirmar_escritura_fs(t_buffer *buffer)
{
    // Le aviso a Kernel que ya termine la operacion
    t_buffer *buffer_kernel = crear_buffer();
    agregar_uint32_a_buffer(buffer_kernel, fs_read.pid);
    t_paquete *paquete_kernel = crear_super_paquete(FIN_INSTRUCCION_INTERFAZ, buffer_kernel);
    enviar_paquete(paquete_kernel, socket_kernel);

    log_info(io_logger, "Operacion FS_READ con %s finalizada", fs_read.nombre_archivo);

    eliminar_paquete(paquete_kernel);
}

//////////////////// LIBERO LAS ESTRUCTURAS CREADAS

// No afecta a la persistencia de los archivos
void liberar_sistema_archivos()
{
    if (bitmap->direccion != MAP_FAILED)
    {
        munmap(bitmap->direccion, bitmap->tamanio);
    }
    if (bloques_datos->direccion != MAP_FAILED)
    {
        munmap(bloques_datos->direccion, cantidad_bloque * tamanio_bloque);
    }

    if (bitmap->file_descriptor != -1)
    {
        close(bitmap->file_descriptor);
    }
    if (bloques_datos->file_descriptor != -1)
    {
        close(bloques_datos->file_descriptor);
    }

    free(bitmap);
    free(bloques_datos);
    list_destroy_and_destroy_elements(lista_metadatas, free);
}