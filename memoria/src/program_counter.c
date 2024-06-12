#include "program_counter.h"

void atender_program_counter(t_buffer *buffer)
{
    uint32_t pid_cpu = extraer_uint32_de_buffer(buffer);
    uint32_t program_counter = extraer_uint32_de_buffer(buffer);

    t_proceso *proceso = encontrar_proceso(lista_de_procesos, pid_cpu);

    if (program_counter <= (list_size(proceso->listas) - 1))
    {
        char *instruccion = list_get(proceso->listas, program_counter);
        enviar_instruccion(instruccion);
    }
    else
    {
        enviar_instruccion("");
    }
};

t_proceso *encontrar_proceso(t_list *lista, uint32_t pid_cpu)
{
    t_proceso *proceso_buscado = malloc(sizeof(t_proceso));

    bool auxiliar_busqueda(void *elemento)
    {
        return soy_proceso_buscado(elemento, pid_cpu);
    }

    proceso_buscado = list_find(lista, auxiliar_busqueda);

    return proceso_buscado;
};

bool soy_proceso_buscado(void *elemento, uint32_t pid_cpu)
{
    t_proceso *proceso_buscado = malloc(sizeof(t_proceso));
    proceso_buscado = (t_proceso *)elemento;
    return proceso_buscado->pid == pid_cpu;
}

void enviar_instruccion(char *instruccion)
{

    t_buffer *new_buffer = crear_buffer();

    agregar_string_a_buffer(new_buffer, instruccion);

    t_paquete *paquete = crear_super_paquete(RECIBIR_INSTRUCCION, new_buffer);

    sleep(retardo_respuesta / 1000);

    enviar_paquete(paquete, socket_cpu);

    destruir_buffer(new_buffer);
    eliminar_paquete(paquete);
};