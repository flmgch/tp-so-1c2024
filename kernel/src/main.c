#include <main.h>

int main(int argc, char *argv[])
{
  // INICIALIZAR ESTRUCTURAS DE KERNEL
  inicializar_kernel();

  // INICIALIZAR COLAS
  cola_new = list_create();
  cola_ready = list_create();
  cola_ready_prioridad = list_create();
  cola_execute = list_create();
  cola_block = list_create();
  cola_exit = list_create();
  lista_recursos = inicializar_recursos();
  lista_io_conectadas = list_create();
  lista_global_pcb = list_create();

  //  INICIALIZAR CONEXIONES
  socket_conexion_cpu_dispatch = crear_conexion(ip_cpu, dispatch, "CPU para dispatch", kernel_logger);
  handshake_cliente(socket_conexion_cpu_dispatch, kernel_logger);

  socket_conexion_cpu_interrupt = crear_conexion(ip_cpu, interrupt, "CPU para interrupt", kernel_logger);
  handshake_cliente(socket_conexion_cpu_interrupt, kernel_logger);

  socket_conexion_memoria = crear_conexion(ip_memoria, puerto_memoria, "Memoria", kernel_logger);
  handshake_cliente(socket_conexion_memoria, kernel_logger);

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

  // INICIAR CONSOLA INTERACTIVA
  pthread_t hilo_consola;
  pthread_create(&hilo_consola, NULL, (void *)inicializar_consola, NULL);
  pthread_detach(hilo_consola);

  // INICIALIZAR SERVER
  socket_escucha = iniciar_escucha(puerto_escucha, "Kernel", kernel_logger);
  while (1)
  {
    // CREO UN HILO POR CADA INTERFAZ QUE SE ME CONECTA Y GUARDO EL SOCKET
    int* socket_interfaz = malloc(sizeof(int));
    *socket_interfaz = esperar_conexion(socket_escucha, "Interfaz", kernel_logger);
    handshake_servidor(*socket_interfaz);

    pthread_t hilo_interfaz;
    pthread_create(&hilo_interfaz, NULL, (void*)atender_modulo_interfaz, socket_interfaz);
    pthread_detach(hilo_interfaz);

  }

  return 0;
}
