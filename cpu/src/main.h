#ifndef CPU_H_
#define CPU_H_

#include "gestor_cpu.h"

// VARIABLES GLOBALES
t_log *cpu_logger;
t_config *cpu_config;
char *puerto_dispatch, *puerto_interrupt, *ip_memoria, *puerto_memoria, *algoritmo_tlb;
int cantidad_entradas_tlb;

// SOCKETS
int socket_memoria, socket_escucha_dispatch, socket_kernel_dispatch, socket_escucha_interrupt, socket_kernel_interrupt;

#endif