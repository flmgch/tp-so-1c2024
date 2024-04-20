#include <main.h>

int main(int argc, char *argv[])
{
  // INICIALIZAR MEMORIA
  inicializar_memoria();

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
