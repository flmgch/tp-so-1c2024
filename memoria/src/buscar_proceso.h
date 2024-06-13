#ifndef BUSCAR_PROCESO_
#define BUSCAR_PROCESO_

#include "gestor_memoria.h"

t_proceso *encontrar_proceso(t_list *lista, uint32_t pid_cpu);
bool soy_proceso_buscado(void *elemento, uint32_t pid_cpu);
bool auxiliar_busqueda(void *elemento);

#endif