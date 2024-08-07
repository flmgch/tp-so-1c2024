#include "memoria_cpu.h"

void atender_cpu()
{
    bool control_key = 1;
    enviar_tamnio_pagina();
    while (control_key)
    {
        t_buffer *un_buffer;
        int cod_op = recibir_operacion(socket_cpu);
        switch (cod_op)
        {
        case MENSAJE:
            //
            break;
        case PAQUETE:
            //
            break;
        case ENVIAR_INSTRUCCIONES:
            un_buffer = recibir_buffer(socket_cpu);
            atender_program_counter(un_buffer);
            destruir_buffer(un_buffer);
            break;
        case ACCESO_TABLA_PAGINAS:
            un_buffer = recibir_buffer(socket_cpu);
            atender_acceso_tabla_paginas(un_buffer);
            destruir_buffer(un_buffer);
            break;
        case AJUSTAR_TAMANIO:
            un_buffer = recibir_buffer(socket_cpu);
            atender_ajustar_tamanio(un_buffer);
            destruir_buffer(un_buffer);
            break;
        case ACCESO_ESPACIO_USUARIO_ESCRITURA:
            un_buffer = recibir_buffer(socket_cpu);
            escribir_memoria(un_buffer, socket_cpu);
            destruir_buffer(un_buffer);
            break;
        case ACCESO_ESPACIO_USUARIO_LECTURA:
            un_buffer = recibir_buffer(socket_cpu);
            leer_memoria(un_buffer, socket_cpu);
            destruir_buffer(un_buffer);
            break;
        case -1:
            log_error(mem_logger, "Se desconecto CPU");
            control_key = 0;
            break;
        default:
            log_warning(mem_logger, "Operacion desconocida de CPU");
            close(socket_cpu);
            control_key = 0;
            break;
        }
    }
}