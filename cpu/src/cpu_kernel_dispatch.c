#include "cpu_kernel_dispatch.h"

void atender_kernel_dispatch()
{

    while (control_key)
    {
        int cod_op = recibir_operacion(socket_kernel_dispatch);
        switch (cod_op)
        {
        case MENSAJE:
            //
            break;
        case PAQUETE:
            //
            break;
        case -1:
            log_error(cpu_logger, "Se desconecto kernel-dispatch");
            control_key = 0;
            break;
        default:
            log_warning(cpu_logger, "Operacion desconocida de kernel-dispatch");
            break;
        }
    }
};