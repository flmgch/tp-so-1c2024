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

  // ATENDER CPU

  pthread_t hilo_cpu;
  pthread_create(&hilo_cpu, NULL, (void *)atender_cpu, NULL);
  pthread_detach(hilo_cpu);

  // ATENDER KERNEL

  pthread_t hilo_kernel;
  pthread_create(&hilo_kernel, NULL, (void *)atender_kernel, NULL);
  pthread_detach(hilo_kernel);

  // ATENDER INTERFAZ

  pthread_t hilo_interfaz;
  pthread_create(&hilo_interfaz, NULL, (void *)atender_interfaz, NULL);
  pthread_join(hilo_interfaz, NULL);

  // borrar_espacio_usuario();
  // borrar_semaforos();
  // list_destroy(lista_de_procesos);
  //  close(socket_escucha);
  //  close(socket_interfaz);
  //  close(socket_cpu);
  //  close(socket_kernel);
  //  terminar_programa(mem_logger, mem_config);
  return 0;
}
