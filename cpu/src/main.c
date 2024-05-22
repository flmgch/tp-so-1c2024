#include "main.h"
#include "inicializar_cpu.h"
#include "cpu_memoria.h"
#include "cpu_kernel_dispatch.h"
#include "cpu_kernel_interrupt.h"

int main(int argc, char *argv[])
{
   // INICIALIZAR CPU
   inicializar_cpu();

   // INICIALIZAR CONEXIONES
   int socket_memoria = crear_conexion(ip_memoria, puerto_memoria, "Memoria", cpu_logger);
   handshake_cliente(socket_memoria, cpu_logger);
   int socket_escucha_dispatch = iniciar_escucha(puerto_dispatch, "CPU para dispatch", cpu_logger);
   int socket_escucha_interrupt = iniciar_escucha(puerto_interrupt, "CPU para interrupt", cpu_logger);

   // ESPERAR CONEXIONES DE KERNEL
   int socket_kernel_dispatch = esperar_conexion(socket_escucha_dispatch, "Kernel para dispatch", cpu_logger);
   handshake_servidor(socket_kernel_dispatch);
   int socket_kernel_interrupt = esperar_conexion(socket_escucha_interrupt, "Kernel para interrupt", cpu_logger);
   handshake_servidor(socket_kernel_interrupt);

   // ATENDER MEMORIA

   pthread_t hilo_memoria;
   pthread_create(&hilo_memoria, NULL, (void *)atender_memoria, NULL);
   pthread_detach(hilo_memoria);

   // ATENDER KERNEL-DISPATCH

   pthread_t hilo_kernel_dispatch;
   pthread_create(&hilo_kernel_dispatch, NULL, (void *)atender_kernel_dispatch, NULL);
   pthread_detach(hilo_kernel_dispatch);

   // ATENDER KERNEL-INTERRUPT

   pthread_t hilo_kernel_interrupt;
   pthread_create(&hilo_kernel_interrupt, NULL, (void *)atender_kernel_interrupt, NULL);
   pthread_join(hilo_kernel_interrupt, NULL);

   // close(socket_memoria);
   // close(socket_escucha_dispatch);
   // close(socket_kernel_dispatch);
   // close(socket_escucha_interrupt);
   // close(socket_kernel_interrupt);
   // terminar_programa(cpu_logger, cpu_config);
   return 0;
}
