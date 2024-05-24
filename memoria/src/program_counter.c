#include "program_counter.h"

void atender_program_counter(t_buffer *buffer)
{
    uint32_t program_counter = extraer_uint32_de_buffer(buffer);

    if (program_counter <= contador)
    {
        char *instruccion = list_get(lista_de_instrucciones, program_counter);

        t_buffer *new_buffer = crear_buffer();

        agregar_string_a_buffer(new_buffer, instruccion);

        t_paquete *un_paquete = crear_super_paquete(RECIBIR_INSTRUCCION, new_buffer);

        agregar_a_paquete(un_paquete, instruccion, strlen(instruccion) + 1);

        sleep(retardo_respuesta / 1000);

        enviar_paquete(un_paquete, socket_cpu);

        destruir_buffer(new_buffer);
        eliminar_paquete(un_paquete);
    }
    else
    {
        log_error(mem_logger, "Esa instruccion no pertenece a este proceso");
        exit(1);
    }
}