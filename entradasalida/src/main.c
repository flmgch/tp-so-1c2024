#include "main.h"

int main(int argc, char *argv[])
{
  // INICIALIZAR IO
  io_logger = iniciar_logger("EntradaYSalida.log", LOG_LEVEL_DEBUG);
  io_config = iniciar_config("./EntradaYSalida.config");

  // INICIALIZAR VARIABLES GLOBALES (CONFIG)
  tipo_interfaz = config_get_string_value(io_config, "TIPO_INTERFAZ");
  path_base_dialfs = config_get_string_value(io_config, "PATH_BASE_DIALFS");
  tiempo_unidad_trabajo = config_get_int_value(io_config, "TIEMPO_UNIDAD_TRABAJO");
  tamanio_bloque = config_get_int_value(io_config, "BLOCK_SIZE");
  cantidad_bloque = config_get_int_value(io_config, "BLOCK_COUNT");
  retraso_compactacion = config_get_int_value(io_config, "RETRASO_COMPACTACION");

  // INICIALIZAR VARIABLES PARA CONEXION (CONFIG)
  ip_kernel = config_get_string_value(io_config, "IP_KERNEL");
  puerto_kernel = config_get_string_value(io_config, "PUERTO_KERNEL");
  ip_memoria = config_get_string_value(io_config, "IP_MEMORIA");
  puerto_memoria = config_get_string_value(io_config, "PUERTO_MEMORIA");

  // CREAR CONEXIONES 
  int socket_memoria = crear_conexion(ip_kernel, puerto_kernel, "Memoria", io_logger);
  int socket_kernel = crear_conexion(ip_memoria, puerto_memoria, "Kernel", io_logger);

  // ENVIAR MENSAJES
  

  close(socket_kernel);
  close(socket_memoria);
  terminar_programa(io_logger, io_config);
  return 0;
}
