#ifndef KERNEL_CPU_DISPATCH_
#define KERNEL_CPU_DISPATCH_

#include "gestor_kernel.h"

void atender_cpu_dispatch();
void procesar_cambio_estado(t_pcb *pcb, estado_proceso estado_nuevo);
void atender_wait(t_pcb *pcb, char *recurso);
void atender_signal(t_pcb *pcb, char *recurso);
t_recurso *buscar_recurso(char *recurso);

#endif