#include "crear_proceso.h"

void atender_crear_proceso(t_buffer *buffer)
{
    pid = extraer_int_de_buffer(buffer);
    char *path = extraer_string_de_buffer(buffer);
    size = extraer_int_de_buffer(buffer);

    char *archivo = strcat(path_instrucciones, path);

    abrir_archivo(archivo);

    free(path);
};