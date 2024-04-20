#include <stdlib.h>
#include <stdio.h>
#include <utils/hello.h>
#include <main.h>

int main(int argc, char *argv[])
{
// INICIALIZAR KERNEL
  kernel_logger = iniciar_logger("Kernel.log", LOG_LEVEL_DEBUG);
  kernel_config = iniciar_config("./Kernel.config");

//INICIALIZAR VARIABLES DE CONEXIÓN
  puerto_escucha = config_get_string_value(kernel_config, "PUERTO_ESCUCHA");
  ip_memoria = config_get_string_value(kernel_config, "IP_MEMORIA");
  puerto_memoria = config_get_string_value(kernel_config, "PUERTO_MEMORIA");
  ip_cpu = config_get_string_value(kernel_config, "IP_CPU");
  dispatch = config_get_string_value(kernel_config, "PUERTO_CPU_DISPATCH");
  interrupt = config_get_string_value(kernel_config, "PUERTO_CPU_INTERRUPT");

//INICIALIZAR CONEXIONES
  int socket_conexion_cpu_dispatch = crear_conexion(ip_cpu, dispatch, "CPU para dispatch", kernel_logger);
  int socket_conexion_cpu_interrupt = crear_conexion(ip_cpu, interrupt, "CPU para interrupt", kernel_logger);
  int socket_conexion_memoria = crear_conexion(ip_memoria, puerto_memoria, "Memoria", kernel_logger);
  int socket_escucha_interfaz = iniciar_escucha(puerto_escucha, "Kernel", kernel_logger);
  int socket_interfaz = esperar_conexion(socket_escucha_interfaz, "Interfaz", kernel_logger);


  close(socket_conexion_cpu_interrupt);
  close(socket_escucha_interfaz);
  close(socket_interfaz);
  close(socket_conexion_memoria);
  close(socket_conexion_cpu_dispatch);
  terminar_programa(kernel_logger, kernel_config);
  return 0;
}
