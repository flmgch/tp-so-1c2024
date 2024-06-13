#include "buscar_proceso.h"

t_proceso *encontrar_proceso(t_list *lista, uint32_t pid_cpu)
{
    t_proceso *proceso_buscado = malloc(sizeof(t_proceso));

    bool auxiliar_busqueda(void *elemento)
    {
        return soy_proceso_buscado(elemento, pid_cpu);
    }

    proceso_buscado = list_find(lista, auxiliar_busqueda);

    return proceso_buscado;
}

bool soy_proceso_buscado(void *elemento, uint32_t pid_cpu)
{
    t_proceso *proceso_buscado = malloc(sizeof(t_proceso));
    proceso_buscado = (t_proceso *)elemento;
    return proceso_buscado->pid == pid_cpu;
}