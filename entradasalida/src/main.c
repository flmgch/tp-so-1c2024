#include "main.h"

int main(int argc, char *argv[])
{

  io_logger = iniciar_logger("EntradaYSalida.log", LOG_LEVEL_DEBUG);
  log_info(io_logger, "Soy log");

  io_config = iniciar_config("./EntradaYSalida.config");
  ip_kernel = config_get_string_value(io_config, "IP_KERNEL");
  puerto_kernel = config_get_string_value(io_config, "PUERTO_KERNEL");
  ip_memoria = config_get_string_value(io_config, "IP_MEMORIA");
  puerto_memoria = config_get_string_value(io_config, "PUERTO_MEMORIA");
  log_info(io_logger, "%s,%s,%s,%s", puerto_kernel, ip_kernel, ip_memoria, puerto_memoria);

  int socket_conexion_memoria = crear_conexion(ip_kernel, puerto_kernel, "Memoria", io_logger);
  int socket_conexion_kernel = crear_conexion(ip_memoria, puerto_memoria, "Kernel", io_logger);

  close(socket_conexion_kernel);
  close(socket_conexion_memoria);
  terminar_programa(io_logger, io_config);
  return 0;
}
