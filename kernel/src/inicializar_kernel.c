#include "inicializar_kernel.h"

void inicializar_kernel()
{
    inicializar_logs();
    inicializar_configs();
}

void inicializar_logs()
{
    kernel_logger = iniciar_logger("Kernel.log", LOG_LEVEL_DEBUG);
};

void inicializar_configs()
{
    kernel_config = iniciar_config("./Kernel.config");

    // INICIALIZAR VARIABLES DE CONEXIÓN
    puerto_escucha = config_get_string_value(kernel_config, "PUERTO_ESCUCHA");
    ip_memoria = config_get_string_value(kernel_config, "IP_MEMORIA");
    puerto_memoria = config_get_string_value(kernel_config, "PUERTO_MEMORIA");
    ip_cpu = config_get_string_value(kernel_config, "IP_CPU");
    dispatch = config_get_string_value(kernel_config, "PUERTO_CPU_DISPATCH");
    interrupt = config_get_string_value(kernel_config, "PUERTO_CPU_INTERRUPT");
    // INICIALIZAR VARIABLES DE KERNEL
    algoritmo_planificacion = config_get_string_value(kernel_config, "ALGORITMO_PLANIFICACION");
    quantum = config_get_int_value (kernel_config, "QUANTUM");
    // recursos = config_get_string_value(kernel_config, "RECURSOS");
    // instancias_recursos = config_get_string_value(kernel_config, "INSTANCIAS_RECURSOS");
    grado_multiprogramacion = config_get_int_value(kernel_config, "GRADO_MULTIPROGRAMACION");
};