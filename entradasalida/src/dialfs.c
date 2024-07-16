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
char* path_metadata;

void iniciar_bitmap(){

    bitmap = malloc(sizeof(t_bitmap));

    bitmap->tamanio = (cantidad_bloque / 8);

    char *path_bitmap[strlen(path_base_dialfs) + 12]; 
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
    }

	bitmap->bitarray = bitarray_create_with_mode(bitmap->direccion, bitmap->tamanio, LSB_FIRST);
}

void iniciar_bloques_datos()
{
    bloques_datos = malloc(sizeof(t_bloques_datos));

    char *path_bloques_datos[strlen(path_base_dialfs) + 13];
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

void atender_fs_create(char* nombre_archivo){   

    int bloque_libre = -1;

    // busco un bloqu libre en el bitmap
    for (int i = 0; i < cantidad_bloque; i++) {
        if (!bitarray_test_bit(bitmap, i)) {
            bloque_libre = i;
            bitarray_set_bit(bitmap, i);
            break;
        }
    }

    if (bloque_libre == -1) {
        log_error(io_logger, "No se encontraron bloques libres en el bitmap");
        return;
    }

    //sincronizo el bitmap.dat con el array en si
    if (msync(bitmap->direccion, bitmap->tamanio, MS_SYNC) == -1) {
        perror("Error al sincronizar el bitmap");
        log_error(io_logger, "Error al sincronizar el bitmap");
        return;
    }

    // creo el nombre del archivo de metadata
    char *metadata_path = malloc(strlen(path_metadata) + strlen(nombre_archivo) + 2);
    if (!metadata_path) {
        log_error(io_logger, "Error al asignar memoria para la ruta del archivo de metadata");
        return;
    }

    //armo el nombre del path
    strcpy(metadata_path, path_metadata);
    strcat(metadata_path, "/");
    strcat(metadata_path, nombre_archivo);

    // creoe y abr el archivo de metadata 
    t_config *metadata_config = config_create(metadata_path);
    if (metadata_config == NULL) {
        log_error(io_logger, "Error al crear el archivo de metadata");
        free(metadata_path);
        return;
    }

    // escribo la info en el archivo de metadata
    config_set_value(metadata_config, "BLOQUE_INICIAL", string_itoa(bloque_libre));
    config_set_value(metadata_config, "TAMANIO_ARCHIVO", "0");
    config_save(metadata_config);
    config_destroy(metadata_config);

    log_info(io_logger, "Archivo %s creado exitosamente con bloque inicial %d y tamaño 0", nombre_archivo, bloque_libre);

    free(metadata_path);

    log_info(io_logger, "PID:   - Crear Archivo: %d",nombre_archivo); //FALTA EL PID!!
}

    

void atender_fs_delete(t_buffer *buffer)
{
    //   
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

