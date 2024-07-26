#include "inicializar_memoria.h"

void inicializar_memoria()
{
    inicializar_logger();
    inicializar_config();
    inicializar_semaforos();
    inicializar_espacio_usuario();
    lista_de_procesos = list_create();
}

void inicializar_logger()
{
    mem_logger = iniciar_logger("Memoria.log", LOG_LEVEL_DEBUG);
};

void inicializar_config()
{
    mem_config = iniciar_config("./Memoria.config");

    // INICIALIZAR VARIABLES GLOBALES (CONFIG)
    puerto_escucha = config_get_string_value(mem_config, "PUERTO_ESCUCHA");
    path_instrucciones = config_get_string_value(mem_config, "PATH_INSTRUCCIONES");
    tamanio_memoria = config_get_int_value(mem_config, "TAM_MEMORIA");
    tamanio_pagina = config_get_int_value(mem_config, "TAM_PAGINA");
    retardo_respuesta = config_get_int_value(mem_config, "RETARDO_RESPUESTA");
}

void inicializar_espacio_usuario()
{
    espacio_usuario = malloc(tamanio_memoria);
    cantidad_marcos = tamanio_memoria / tamanio_pagina;
    cantidad_de_marcos_libres = cantidad_marcos;
    int tam = ceil((double)cantidad_marcos / 8);
    bitmap_espacio_usuario = malloc(tam);
    bitmap = bitarray_create_with_mode(bitmap_espacio_usuario, tam, LSB_FIRST);
    for (int i = 0; i < cantidad_marcos; i++)
    {
        bitarray_clean_bit(bitmap, i);
    }
    /*
        int n = bitarray_test_bit(bitmap, i);
        log_info(mem_logger, "%d ", n);
    }*/
}

void inicializar_semaforos()
{
    pthread_mutex_init(&mutex_lista_procesos, NULL);
    pthread_mutex_init(&mutex_bitmap, NULL);
    pthread_mutex_init(&mutex_espacio_usuario, NULL);
    pthread_mutex_init(&mutex_cantidad_marcos_libres, NULL);
    pthread_cond_init(&condicion, NULL);
}

void borrar_espacio_usuario()
{
    free(espacio_usuario);
    free(bitmap_espacio_usuario);
    free(bitmap);
}

void borrar_semaforos()
{
    pthread_mutex_destroy(&mutex_bitmap);
    pthread_mutex_destroy(&mutex_espacio_usuario);
    pthread_mutex_destroy(&mutex_lista_procesos);
    pthread_mutex_destroy(&mutex_cantidad_marcos_libres);
    pthread_cond_destroy(&condicion);
}
