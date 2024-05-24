#include "memoria_cpu.h"

void atender_cpu()
{
    t_buffer *un_buffer;
    bool control_key = 1;
    while (control_key)
    {
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
        case -1:
            log_error(mem_logger, "Se desconecto CPU");
            control_key = 0;
            break;
        default:
            log_warning(mem_logger, "Operacion desconocida de CPU");
            break;
        }
    }
};