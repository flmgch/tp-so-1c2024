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
extern bool flag_execute;
extern int tamanio_pagina;
extern t_instruccion instr;

// SOCKETS
extern int socket_memoria, socket_escucha_dispatch, socket_kernel_dispatch, socket_escucha_interrupt, socket_kernel_interrupt;

// TLB
extern tlb_entrada_t* tlb; 
extern int tlb_proximo_reemplazo; 
extern int tlb_entradas;
extern int contador_acceso;

// SEMAFORO
extern sem_t sem_instruccion;

#endif