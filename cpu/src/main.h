#ifndef CPU_H_
#define CPU_H_

#include "gestor_cpu.h"
#include "cpu_memoria.h"
#include "cpu_kernel_dispatch.h"
#include "cpu_kernel_interrupt.h"

// VARIABLES GLOBALES
t_log *cpu_logger;
t_config *cpu_config;
char *puerto_dispatch, *puerto_interrupt, *ip_memoria, *puerto_memoria, *algoritmo_tlb;
int cantidad_entradas_tlb;
t_pcb* pcb;
t_registros *registros;
bool flag_execute;

// SOCKETS
int socket_memoria, socket_escucha_dispatch, socket_kernel_dispatch, socket_escucha_interrupt, socket_kernel_interrupt;

#endif