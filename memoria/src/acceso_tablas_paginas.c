#include "acceso_tablas_paginas.h"

void atender_acceso_tabla_paginas(t_buffer *buffer)
{
    int numero_pagina = extraer_int_de_buffer(buffer);
    uint32_t pid = extraer_uint32_de_buffer(buffer);
    t_proceso *proceso_buscado = malloc(sizeof(t_proceso));

    pthread_mutex_lock(&mutex_lista_procesos);
    proceso_buscado = encontrar_proceso(lista_de_procesos, pid);
    pthread_mutex_unlock(&mutex_lista_procesos);

    int *frame = list_get(proceso_buscado->filas_tabla_paginas, numero_pagina);

    t_buffer *new_buffer = crear_buffer();

    agregar_int_a_buffer(new_buffer, *frame);

    t_paquete *paquete = crear_super_paquete(ENIVIAR_FRAME, new_buffer);

    enviar_paquete(paquete, socket_cpu);

    free(proceso_buscado);
    destruir_buffer(new_buffer);
    eliminar_paquete(paquete);
}