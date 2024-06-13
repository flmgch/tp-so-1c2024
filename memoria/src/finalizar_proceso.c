#include "finalizar_proceso.h"

void atender_finalizar_proceso(t_buffer *buffer)
{
    t_proceso *proceso_a_eliminar = malloc(sizeof(t_proceso));
    tabla_de_paginas *fila = malloc(sizeof(tabla_de_paginas));
    int tamanio_lista_procesos;
    uint32_t pid = extraer_uint32_de_buffer(buffer);

    pthread_mutex_lock(&mutex_lista_procesos);
    proceso_a_eliminar = encontrar_proceso(lista_de_procesos, pid);

    tamanio_lista_procesos = list_size(lista_de_procesos);

    if (list_remove_element(lista_de_procesos, proceso_a_eliminar) == false)
    {
        log_error(mem_logger, "No se encuentra el proceso en la lista");
    }
    else
    {
        log_info(mem_logger, "Se elimino el proceso de la lista de procesos");
    }
    pthread_mutex_unlock(&mutex_lista_procesos);

    pthread_mutex_lock(&mutex_bitmap);
    for (int i = 0; i < tamanio_lista_procesos; i++)
    {
        fila = list_get(proceso_a_eliminar->filas_tabla_paginas, i);
        bitarray_clean_bit(bitmap, fila->frame);
    }
    pthread_mutex_unlock(&mutex_bitmap);

    log_info(mem_logger, "Se limpiaron los frames de las paginas de los procesos");

    free(proceso_a_eliminar);
    free(fila);
};