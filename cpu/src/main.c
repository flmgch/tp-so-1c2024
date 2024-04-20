#include "main.h"

int main(int argc, char *argv[])
{
   // INICIALIZAR CPU
   cpu_logger = iniciar_logger("CPU.log", LOG_LEVEL_DEBUG);
   cpu_config = iniciar_config("./CPU.config");

   // INICIALIZAR VARIABLES GLOBALES
   cantidad_entradas_tlb = config_get_int_value(cpu_config, "CANTIDAD_ENTRADAS_TLB");
   algoritmo_tlb = config_get_string_value(cpu_config, "ALGORITMO_TLB");

   // INICIALIZAR VARIABLES DE CONEXION
   ip_memoria = config_get_string_value(cpu_config, "IP_MEMORIA");
   puerto_memoria = config_get_string_value(cpu_config, "PUERTO_MEMORIA");
   puerto_dispatch = config_get_string_value(cpu_config, "PUERTO_ESCUCHA_DISPATCH");
   puerto_interrupt = config_get_string_value(cpu_config, "PUERTO_ESCUCHA_INTERRUPT");

   // INICIALIZAR CONEXIONES
   int socket_cpu_memoria = crear_conexion(ip_memoria, puerto_memoria, "Memoria", cpu_logger);
   int socket_escucha_dispatch = iniciar_escucha(puerto_dispatch, "CPU para dispatch", cpu_logger);
   int socket_escucha_interrupt = iniciar_escucha(puerto_interrupt, "CPU para interrupt", cpu_logger);

   // ESPERAR CONEXIONES DE KERNEL
   int socket_dispatch = esperar_conexion(socket_escucha_dispatch, "Kernel para dispatch", cpu_logger);
   int socket_interrupt = esperar_conexion(socket_escucha_interrupt, "Kernel para interrupt", cpu_logger);

   close(socket_cpu_memoria);
   close(socket_escucha_dispatch);
   close(socket_dispatch);
   close(socket_escucha_interrupt);
   close(socket_interrupt);
   terminar_programa(cpu_logger, cpu_config);
   return 0;
}
