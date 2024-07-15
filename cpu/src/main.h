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
int tamanio_pagina;
t_instruccion instruccion;
//char* datos;
int aux_resize = 0;
int aux_marco = 0;
int numero_pagina = 0;
int desplazamiento = 0;
uint32_t dir_fisica_inicial = 0;
int tam_aux = 0;
void *aux_dato;
char *reg_aux;
// SOCKETS
int socket_memoria, socket_escucha_dispatch, socket_kernel_dispatch, socket_escucha_interrupt, socket_kernel_interrupt;

// TLB
tlb_entrada_t *tlb;
int tlb_proximo_reemplazo;
int tlb_entradas;
int contador_acceso;

// SEMAFOR
sem_t sem_instruccion;
sem_t sem_resize;
sem_t sem_resultado_escritura;
sem_t sem_resultado_lectura;
sem_t sem_consultar_memoria;

#endif