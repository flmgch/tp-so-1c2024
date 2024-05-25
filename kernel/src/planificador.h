#ifndef PLANIFICADOR_H_
#define PLANIFICADOR_H_

#include "gestor_kernel.h"

void cambiar_estado(t_pcb *pcb, estado_proceso nuevo_estado);
void planificar();
void planificar_corto_plazo();
void execute_pcb();
void dispatch_pcb(t_pcb *pcb);
t_pcb *remover_pcb(t_list *list, pthread_mutex_t *mutex);
t_pcb *pcb_segun_algoritmo();

// TODO RR VRR
// t_pcb *obtener_pcb_RR();
// t_pcb *obtener_pcb_VRR();
#endif