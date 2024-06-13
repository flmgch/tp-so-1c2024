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
    int cantidad_marcos = tamanio_memoria / tamanio_pagina;
    int tam = calcular_tamanio(cantidad_marcos);
    bitmap_espacio_usuario = malloc(tam);
    bitmap = bitarray_create_with_mode(bitmap_espacio_usuario, tam, LSB_FIRST);
}

int calcular_tamanio(int marcos)
{
    int tam;
    if (marcos % 8 == 0)
    {
        tam = marcos / 8;
    }
    else
    {
        tam = marcos / 8 + 1;
    }
    return tam;
}

void inicializar_semaforos()
{
    pthread_mutex_init(&mutex_lista_procesos, NULL);
    pthread_mutex_init(&mutex_bitmap, NULL);
    pthread_mutex_init(&mutex_espacio_usuario, NULL);
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
}
