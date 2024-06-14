#include "crear_proceso.h"

t_proceso *atender_crear_proceso(t_buffer *buffer)
{
    char *path = extraer_string_de_buffer(buffer);
    u_int32_t pid = extraer_uint32_de_buffer(buffer);
    t_proceso *proceso = malloc(sizeof(t_proceso));
    proceso->pid = pid;
    proceso->path = path;

    char *auxiliar = malloc(strlen(path_instrucciones) + 1);

    strcpy(auxiliar, path_instrucciones);

    char *archivo = strcat(auxiliar, path);

    proceso->listas = abrir_archivo(archivo);
    proceso->size = 0;
    proceso->filas_tabla_paginas = list_create();

    free(auxiliar);
    return proceso;
};