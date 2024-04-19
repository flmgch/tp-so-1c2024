#ifndef CPU_H_
#define CPU_H_

#include <utils/hello.h>

// VARIABLES GLOBALES
t_log *cpu_logger;
t_config *cpu_config;
char *puerto_dispatch, *puerto_interrupt, *ip_memoria, *puerto_memoria, *algoritmo_tlb;
int cantidad_entradas_tlb;

#endif