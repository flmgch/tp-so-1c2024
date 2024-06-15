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
  lista_recursos = inicializar_recursos();
  lista_io_conectadas = list_create();

  // INICIAR VARIABLES PARA TESTS
  inicializar_tests();

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
  pthread_create(&hilo_interfaz, NULL, (void *)atender_modulo_interfaz, NULL);
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

void inicializar_tests() {
  // Inicialización de los registros para los PCB
  registros1 = (t_registros){1, 2, 3, 4, 100, 200, 300, 400, 500, 600};
  registros2 = (t_registros){5, 6, 7, 8, 150, 250, 350, 450, 550, 650};
  registros3 = (t_registros){9, 10, 11, 12, 175, 275, 375, 475, 575, 675};

  // Ejemplo 1
  pcb1.pid = 1;
  pcb1.program_counter = 100;
  pcb1.estado = NEW;
  pcb1.motivo_block = IO_BLOCK;
  pcb1.motivo_exit = SUCCESS;
  pcb1.registros_cpu = &registros1;
  pcb1.quantum_remanente = 5;

  // Ejemplo 2
  pcb2.pid = 2;
  pcb2.program_counter = 200;
  pcb2.estado = EXEC;
  pcb2.motivo_block = IO_BLOCK;
  pcb2.motivo_exit = NONE_EXIT;
  pcb2.registros_cpu = &registros2;
  pcb2.quantum_remanente = 10;

  // Ejemplo 3
  pcb3.pid = 3;
  pcb3.program_counter = 300;
  pcb3.estado = BLOCK;
  pcb3.motivo_block = IO_BLOCK;
  pcb3.motivo_exit = NONE_EXIT;
  pcb3.registros_cpu = &registros3;
  pcb3.quantum_remanente = 15;
}