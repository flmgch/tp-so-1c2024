#include <main.h>

int main(int argc, char *argv[])
{
  // INICIALIZAR ESTRUCTURAS DE KERNEL
  inicializar_kernel();

  // INICIALIZAR COLAS
  cola_new = list_create();
  cola_ready = list_create();
  cola_execute = list_create();
  cola_block = list_create();
  cola_exit = list_create();

  //  INICIALIZAR CONEXIONES
  socket_conexion_cpu_dispatch = crear_conexion(ip_cpu, dispatch, "CPU para dispatch", kernel_logger);
  handshake_cliente(socket_conexion_cpu_dispatch, kernel_logger);

  socket_conexion_cpu_interrupt = crear_conexion(ip_cpu, interrupt, "CPU para interrupt", kernel_logger);
  handshake_cliente(socket_conexion_cpu_interrupt, kernel_logger);

  socket_conexion_memoria = crear_conexion(ip_memoria, puerto_memoria, "Memoria", kernel_logger);
  handshake_cliente(socket_conexion_memoria, kernel_logger);

  // INICIALIZAR SERVER
  socket_escucha = iniciar_escucha(puerto_escucha, "Kernel", kernel_logger);
  socket_interfaz = esperar_conexion(socket_escucha, "Interfaz", kernel_logger);
  handshake_servidor(socket_interfaz);

  // ATENDER CPU - DISPATCH
  pthread_t hilo_cpu_dispatch;
  pthread_create(&hilo_cpu_dispatch, NULL, (void *)atender_cpu_dispatch, NULL);
  pthread_detach(hilo_cpu_dispatch);

  // ATENDER CPU - INTERRUPT
  pthread_t hilo_cpu_interrupt;
  pthread_create(&hilo_cpu_interrupt, NULL, (void *)atender_cpu_interrupt, NULL);
  pthread_detach(hilo_cpu_interrupt);

  // ATENDER MEMORIA
  pthread_t hilo_memoria;
  pthread_create(&hilo_memoria, NULL, (void *)atender_memoria, NULL);
  pthread_detach(hilo_memoria);

  // ATENDER INTERFAZ
  pthread_t hilo_interfaz;
  pthread_create(&hilo_interfaz, NULL, (void *)atender_interfaz, NULL);
  pthread_detach(hilo_interfaz);

  // TODO: INICIAR CONSOLA INTERACTIVA
  inicializar_consola();

  // close(socket_escucha);
  // close(socket_interfaz);
  // close(socket_conexion_memoria);
  // close(socket_conexion_cpu_dispatch);
  // close(socket_conexion_cpu_interrupt);
  // terminar_programa(kernel_logger, kernel_config);
  return 0;
}
