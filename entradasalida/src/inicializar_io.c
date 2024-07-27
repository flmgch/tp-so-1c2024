#include "inicializar_io.h"

void inicializar_io(char *nombre_interfaz, char* archivo_configuracion){
    inicializar_logger(nombre_interfaz);
    inicializar_config(archivo_configuracion);
    inicializar_semaforos();
}

void inicializar_logger(char *nombre_interfaz) {
    char log_filename[100];
    snprintf(log_filename, sizeof(log_filename), "%s.log", nombre_interfaz);
    io_logger = iniciar_logger(log_filename, LOG_LEVEL_DEBUG);
}

void inicializar_config(char *archivo_configuracion) {
    io_config = iniciar_config(archivo_configuracion);
    
    // INICIALIZAR VARIABLES GLOBALES (CONFIG)
    tipo_interfaz = config_get_string_value(io_config, "TIPO_INTERFAZ");
    path_base_dialfs = config_get_string_value(io_config, "PATH_BASE_DIALFS");
    tiempo_unidad_trabajo = config_get_int_value(io_config, "TIEMPO_UNIDAD_TRABAJO");
    tamanio_bloque = config_get_int_value(io_config, "BLOCK_SIZE");
    cantidad_bloque = config_get_int_value(io_config, "BLOCK_COUNT");
    retraso_compactacion = config_get_int_value(io_config, "RETRASO_COMPACTACION");

    // INICIALIZAR VARIABLES PARA CONEXION (CONFIG)
    ip_kernel = config_get_string_value(io_config, "IP_KERNEL");
    puerto_kernel = config_get_string_value(io_config, "PUERTO_KERNEL");
    ip_memoria = config_get_string_value(io_config, "IP_MEMORIA");
    puerto_memoria = config_get_string_value(io_config, "PUERTO_MEMORIA");
}

void inicializar_semaforos()
{
    sem_init(&sem_stdin, 0, 1);
    sem_init(&sem_stdout, 0, 1);
}
