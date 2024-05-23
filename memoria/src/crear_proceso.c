#include "crear_proceso.h"

void atender_crear_proceso(t_buffer *buffer)
{
    path = extraer_string_de_buffer(buffer);

    free(path);
};