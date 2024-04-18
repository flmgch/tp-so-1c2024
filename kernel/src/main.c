#include <stdlib.h>
#include <stdio.h>
#include <utils/hello.h>

int main(int argc, char *argv[])
{
  t_log *logger;
  t_config *config;
  char *puerto_escucha, *ip_memoria, *puerto_memoria, *ip_cpu, *dispatch, *interrupt;
  decir_hola("Kernel");

  logger = iniciar_logger("Kernel.log", LOG_LEVEL_DEBUG);
  log_info(logger, "Soy log");

  config = iniciar_config("./Kernel.config");
  puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");
  ip_memoria = config_get_string_value(config, "IP_MEMORIA");
  puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
  ip_cpu = config_get_string_value(config, "IP_CPU");
  dispatch = config_get_string_value(config, "PUERTO_CPU_DISPATCH");
  interrupt = config_get_string_value(config, "PUERTO_CPU_INTERRUPT");
  log_info(logger, "%s,%s,%s,%s,%s,%s", puerto_escucha, ip_memoria, puerto_memoria, ip_cpu, dispatch, interrupt);

  int socket_Conexion_CPU_Dispatch = crear_conexion(ip_cpu, dispatch, "CPU para dispatch", logger);
  int socket_Conexion_CPU_Interrupt = crear_conexion(ip_cpu, interrupt, "CPU para interrupt", logger);
  int socket_Conexion_Memoria = crear_conexion(ip_memoria, puerto_memoria, "Memoria", logger);
  int socket_Escucha_De_Interfaz = iniciar_escucha(puerto_escucha, "Kernel", logger);
  int socket_Interfaz = esperar_conexion(socket_Escucha_De_Interfaz, "Interfaz", logger);

  close(socket_Conexion_CPU_Interrupt);
  close(socket_Escucha_De_Interfaz);
  close(socket_Interfaz);
  close(socket_Conexion_Memoria);
  close(socket_Conexion_CPU_Dispatch);
  terminar_programa(logger, config);
  return 0;
}
