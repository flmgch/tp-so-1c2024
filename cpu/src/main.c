#include "main.h"

int main(int argc, char *argv[])
{
   t_log *logger;
   t_config *config;
   char *dispatch, *interrupt, *ip_memoria, *puerto_memoria;
   decir_hola("CPU");

   logger = iniciar_logger("CPU.log", LOG_LEVEL_DEBUG);
   log_info(logger, "Soy log");

   config = iniciar_config("./CPU.config");
   ip_memoria = config_get_string_value(config, "IP_MEMORIA");
   puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
   dispatch = config_get_string_value(config, "PUERTO_ESCUCHA_DISPATCH");
   interrupt = config_get_string_value(config, "PUERTO_ESCUCHA_INTERRUPT");
   log_info(logger, "%s,%s,%s,%s", ip_memoria, puerto_memoria, dispatch, interrupt);

   int socket_CPU_Memoria = crear_conexion(ip_memoria, puerto_memoria, "Memoria", logger);

   int socket_Escucha_Dispatch = iniciar_escucha(dispatch, "CPU para dispatch", logger);
   int socket_Escucha_Interrupt = iniciar_escucha(interrupt, "CPU para interrupt", logger);
   int socket_Dispatch = esperar_conexion(socket_Escucha_Dispatch, "Kernel", logger);
   int socket_Interrupt = esperar_conexion(socket_Escucha_Interrupt, "Kernel", logger);

   close(socket_CPU_Memoria);
   close(socket_Escucha_Dispatch);
   close(socket_Dispatch);
   close(socket_Escucha_Interrupt);
   close(socket_Interrupt);
   terminar_programa(logger, config);
   return 0;
}
