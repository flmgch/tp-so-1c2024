#include "main.h"
#include "inicializar_cpu.h"

int main(int argc, char *argv[])
{
   // INICIALIZAR CPU
   inicializar_cpu();

   // INICIALIZAR CONEXIONES
   int socket_memoria = crear_conexion(ip_memoria, puerto_memoria, "Memoria", cpu_logger);
   int socket_escucha_dispatch = iniciar_escucha(puerto_dispatch, "CPU para dispatch", cpu_logger);
   int socket_escucha_interrupt = iniciar_escucha(puerto_interrupt, "CPU para interrupt", cpu_logger);

   // ESPERAR CONEXIONES DE KERNEL
   int socket_kernel_dispatch = esperar_conexion(socket_escucha_dispatch, "Kernel para dispatch", cpu_logger);
   int socket_kernel_interrupt = esperar_conexion(socket_escucha_interrupt, "Kernel para interrupt", cpu_logger);

   close(socket_memoria);
   close(socket_escucha_dispatch);
   close(socket_kernel_dispatch);
   close(socket_escucha_interrupt);
   close(socket_kernel_interrupt);
   terminar_programa(cpu_logger, cpu_config);
   return 0;
}
