#include <main.h>

int main(int argc, char *argv[])
{

  inicializar_kernel();

//INICIALIZAR CONEXIONES
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

  close(socket_conexion_cpu_interrupt);
  close(socket_escucha);
  close(socket_interfaz);
  close(socket_conexion_memoria);
  close(socket_conexion_cpu_dispatch);
  terminar_programa(kernel_logger, kernel_config);
  return 0;
}
