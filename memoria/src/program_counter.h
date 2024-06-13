#ifndef PROGRAM_COUNTER_
#define PROGRAM_COUNTER_

#include "gestor_memoria.h"
#include "buscar_proceso.h"

void atender_program_counter(t_buffer *buffer);
void enviar_instruccion(char *instruccion);

#endif