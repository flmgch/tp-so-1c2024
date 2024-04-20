#include <main.h>

int main(int argc, char *argv[])
{
  // INICIALIZAR MEMORIA
  inicializar_memoria();

  // INICIALIZAR SERVIDOR
  socket_escucha = iniciar_escucha(puerto_escucha, "Memoria", mem_logger);

  // ESPERAR CONEXIONES ENTRANTES
  socket_cpu = esperar_conexion(socket_escucha, "CPU", mem_logger);
  handshake_servidor(socket_cpu);
  socket_kernel = esperar_conexion(socket_escucha, "Kernel", mem_logger);
  handshake_servidor(socket_kernel);
  socket_interfaz = esperar_conexion(socket_escucha, "Interfaz", mem_logger);
  handshake_servidor(socket_interfaz);

  close(socket_escucha);
  close(socket_interfaz);
  close(socket_cpu);
  close(socket_kernel);
  terminar_programa(mem_logger, mem_config);
  return 0;
}
