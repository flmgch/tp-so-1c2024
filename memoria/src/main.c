#include <utils/hello.h>
#include <main.h>

int main(int argc, char *argv[])
{
  // INICIALIZAR MEMORIA
  mem_logger = iniciar_logger("Memoria.log", LOG_LEVEL_DEBUG);
  mem_config = iniciar_config("./Memoria.config");

  // INICIALIZAR VARIABLES GLOBALES (CONFIG)
  puerto_escucha = config_get_string_value(mem_config, "PUERTO_ESCUCHA");
  path_instrucciones = config_get_string_value(mem_config, "PATH_INSTRUCCIONES");
  tamanio_memoria = config_get_int_value(mem_config, "TAM_MEMORIA");
  tamanio_pagina = config_get_int_value(mem_config, "TAM_PAGINA");
  retardo_respuesta = config_get_int_value(mem_config, "RETARDO_RESPUESTA");

  // INICIALIZAR SERVIDOR
  int socket_escucha = iniciar_escucha(puerto_escucha, "Memoria", mem_logger);

  // ESPERAR CONEXIONES ENTRANTES
  int socket_cpu = esperar_conexion(socket_escucha, "CPU", mem_logger);
  int socket_kernel = esperar_conexion(socket_escucha, "Kernel", mem_logger);
  int socket_interfaz = esperar_conexion(socket_escucha, "Interfaz", mem_logger);

  close(socket_escucha);
  close(socket_interfaz);
  close(socket_cpu);
  close(socket_kernel);
  terminar_programa(mem_logger, mem_config);
  return 0;
}
