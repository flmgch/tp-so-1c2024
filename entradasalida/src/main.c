#include <main.h>

int main(int argc, char *argv[])
{
  // INICIALIZAR IO
  inicializar_io();

  // INICIALIZAR CONEXIONES
  socket_memoria = crear_conexion(ip_kernel, puerto_kernel, "Memoria", io_logger);
  handshake_cliente(socket_memoria, io_logger);
  socket_kernel = crear_conexion(ip_memoria, puerto_memoria, "Kernel", io_logger);
  handshake_cliente(socket_kernel, io_logger);

  // ATENDER KERNEL
  pthread_t hilo_kernel;
  pthread_create(&hilo_kernel, NULL, (void *)atender_kernel, NULL);
  pthread_detach(hilo_kernel);

  // ATENDER MEMORIA
  pthread_t hilo_memoria;
  pthread_create(&hilo_memoria, NULL, (void *)atender_memoria, NULL);
  pthread_join(hilo_memoria, NULL);

  // sleep(1000);
  // close(socket_kernel);
  // close(socket_memoria);
  // terminar_programa(io_logger, io_config);
  return 0;
}
