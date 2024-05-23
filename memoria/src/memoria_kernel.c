#include "memoria_kernel.h"

void atender_kernel()
{
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(socket_kernel);
        switch (cod_op)
        {
        case MENSAJE:
            //
            break;
        case PAQUETE:
            //
            break;
        case CREAR_PROCESO:
           un_buffer=recibir_buffer(socket_kernel);
           atender_crear_proceso(un_buffer);

            break;
        case -1:
            log_error(mem_logger, "Se desconecto kernel");
            control_key = 0;
            break;
        default:
            log_warning(mem_logger, "Operacion desconocida de kernel");
            break;
        }
    }
};