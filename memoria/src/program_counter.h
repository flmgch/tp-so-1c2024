#ifndef PROGRAM_COUNTER_
#define PROGRAM_COUNTER_

#include "gestor_memoria.h"

void atender_program_counter(t_buffer *buffer);
t_proceso *encontrar_proceso(t_list *lista, uint32_t pid_cpu);
int soy_proceso_buscado(t_proceso *proceso, uint32_t pid_cpu);
void enviar_instruccion(char *instruccion);

#endif