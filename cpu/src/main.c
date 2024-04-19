#include "main.h"

int main(int argc, char *argv[])
{
   char *dispatch, *interrupt, *ip_memoria, *puerto_memoria;

   cpu_logger = iniciar_logger("CPU.log", LOG_LEVEL_DEBUG);
   log_info(cpu_logger, "Soy log");

   cpu_config = iniciar_config("./CPU.config");
   ip_memoria = config_get_string_value(cpu_config, "IP_MEMORIA");
   puerto_memoria = config_get_string_value(cpu_config, "PUERTO_MEMORIA");
   dispatch = config_get_string_value(cpu_config, "PUERTO_ESCUCHA_DISPATCH");
   interrupt = config_get_string_value(cpu_config, "PUERTO_ESCUCHA_INTERRUPT");
   log_info(cpu_logger, "%s,%s,%s,%s", ip_memoria, puerto_memoria, dispatch, interrupt);

   int socket_cpu_memoria = crear_conexion(ip_memoria, puerto_memoria, "Memoria", cpu_logger);
   int socket_escucha_dispatch = iniciar_escucha(dispatch, "CPU para dispatch", cpu_logger);
   int socket_escucha_interrupt = iniciar_escucha(interrupt, "CPU para interrupt", cpu_logger);
   int socket_dispatch = esperar_conexion(socket_escucha_dispatch, "Kernel", cpu_logger);
   int socket_interrupt = esperar_conexion(socket_escucha_interrupt, "Kernel", cpu_logger);

   close(socket_cpu_memoria);
   close(socket_escucha_dispatch);
   close(socket_dispatch);
   close(socket_escucha_interrupt);
   close(socket_interrupt);
   terminar_programa(cpu_logger, cpu_config);
   return 0;
}
