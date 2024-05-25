#ifndef PLANIFICADOR_H_
#define PLANIFICADOR_H_

#include "gestor_kernel.h"

void cambiar_estado(t_pcb *pcb, estado_proceso nuevo_estado);
void dispatch_pcb(t_pcb *pcb);

void planificar();
void planificar_corto_plazo();
void planificar_largo_plazo();

void pasar_a_ready(t_pcb *pcb);
void block_pcb();
void new_pcb();
void exit_pcb();
void execute_pcb();
t_pcb *remover_pcb(t_list *list, pthread_mutex_t *mutex);
t_pcb *pcb_segun_algoritmo();

// TODO RR VRR
// t_pcb *obtener_pcb_RR();
// t_pcb *obtener_pcb_VRR();
#endif