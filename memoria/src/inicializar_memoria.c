#include "inicializar_memoria.h"

void inicializar_memoria()
{
    inicializar_logger();
    inicializar_config();
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