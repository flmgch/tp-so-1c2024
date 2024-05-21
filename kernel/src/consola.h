#ifndef CONSOLA_H_
#define CONSOLA_H_

#include "gestor_kernel.h"

void inicializar_consola();
bool validar_instruccion(char *leido);
void atender_instruccion(char *leido);
void iniciar_proceso(t_buffer *buffer);
int asignar_pid();


#endif