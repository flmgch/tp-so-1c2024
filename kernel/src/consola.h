#ifndef CONSOLA_H_
#define CONSOLA_H_

#include "gestor_kernel.h"

void inicializar_consola();
bool validar_instruccion(char *leido);
void atender_instruccion(char *leido);
t_pcb *crear_pcb(int pid);
void inicializar_registros_pcb(t_pcb *pcb);
int asignar_pid();

#endif