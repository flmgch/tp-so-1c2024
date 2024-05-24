#ifndef ESTADOS_H_
#define ESTADOS_H_

#include "gestor_kernel.h"

void pasar_a_ready(t_pcb *pcb);
void ingresarANew(t_pcb *pcb);
void Aready();
t_pcb *obtenerSiguienteAready();

#endif