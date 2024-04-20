#include "main.h"

int main(int argc, char *argv[])
{
  // INICIALIZAR IO
  inicializar_io();
  
  // CREAR CONEXIONES 
  socket_memoria = crear_conexion(ip_kernel, puerto_kernel, "Memoria", io_logger);
  socket_kernel = crear_conexion(ip_memoria, puerto_memoria, "Kernel", io_logger);

  close(socket_kernel);
  close(socket_memoria);
  terminar_programa(io_logger, io_config);
  return 0;
}
