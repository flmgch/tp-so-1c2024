#ifndef GESTOR_CPU_H_
#define GESTOR_CPU_H_

#include <utils/hello.h>

// VARIABLES GLOBALES
extern t_log *cpu_logger;
extern t_config *cpu_config;
extern char *puerto_dispatch, *puerto_interrupt, *ip_memoria, *puerto_memoria, *algoritmo_tlb;
extern int cantidad_entradas_tlb;
extern t_pcb* pcb;
extern t_registros* registros;
bool flag_execute;

// SOCKETS
extern int socket_memoria, socket_escucha_dispatch, socket_kernel_dispatch, socket_escucha_interrupt, socket_kernel_interrupt;

#endif