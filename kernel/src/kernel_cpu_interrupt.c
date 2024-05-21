#include "kernel_cpu_interrupt.h"

void atender_cpu_interrupt()
{
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(socket_conexion_cpu_interrupt);
        switch (cod_op)
        {
        case MENSAJE:
            //
            break;
        case PAQUETE:
            //
            break;
        case -1:
            log_error(kernel_logger, "Se desconecto CPU - Interrupt");
            control_key = 0;
            break;
        default:
            log_warning(kernel_logger, "Operacion desconocida de CPU - Interrupt");
            break;
        }
    }
};