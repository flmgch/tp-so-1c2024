#include "interfaz_kernel.h"

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
        case -1:
            log_error(io_logger, "Se desconecto Kernel");
            control_key = 0;
            break;
        default:
            log_warning(io_logger, "Operacion desconocida de Kernel");
            break;
        }
    }
};