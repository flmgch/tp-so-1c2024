#include "memoria_kernel.h"

void atender_kernel()
{

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
            //
            break;
        case TERMINAR_PROCESO:
            //
            break;
        case AMPLIACION_PROCESO:
            //
            break;
        case REDUCCION_PROCESO:
            //
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