#include "crear_proceso.h"

t_proceso *atender_crear_proceso(t_buffer *buffer)
{
    t_proceso *proceso = malloc(sizeof(t_proceso));

    proceso->pid = extraer_uint32_de_buffer(buffer);
    proceso->path = extraer_string_de_buffer(buffer);

    char *archivo = strcat(path_instrucciones, proceso->path);

    proceso->listas = abrir_archivo(archivo);

    return proceso;
};