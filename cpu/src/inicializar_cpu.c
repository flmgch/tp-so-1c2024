#include "inicializar_cpu.h"

void inicializar_cpu()
{
    inicializar_logger();
    inicializar_config();
}

void inicializar_logger()
{
    cpu_logger = iniciar_logger("CPU.log", LOG_LEVEL_DEBUG);
}

void inicializar_config()
{
    cpu_config = iniciar_config("./CPU.config");

    // INICIALIZAR VARIABLES GLOBALES
    cantidad_entradas_tlb = config_get_int_value(cpu_config, "CANTIDAD_ENTRADAS_TLB");
    algoritmo_tlb = config_get_string_value(cpu_config, "ALGORITMO_TLB");

    // INICIALIZAR VARIABLES DE CONEXION
    ip_memoria = config_get_string_value(cpu_config, "IP_MEMORIA");
    puerto_memoria = config_get_string_value(cpu_config, "PUERTO_MEMORIA");
    puerto_dispatch = config_get_string_value(cpu_config, "PUERTO_ESCUCHA_DISPATCH");
    puerto_interrupt = config_get_string_value(cpu_config, "PUERTO_ESCUCHA_INTERRUPT");
}