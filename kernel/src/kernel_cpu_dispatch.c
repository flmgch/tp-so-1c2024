#include "kernel_cpu_dispatch.h"

void atender_cpu_dispatch() {
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(socket_conexion_cpu_dispatch);
        switch (cod_op)
        {
        case MENSAJE:
            // 
            break;
        case PAQUETE:
            // 
            break;
        case -1:
            log_error(kernel_logger, "Se desconecto CPU - Dispatch");
            control_key = 0;
            break;
        default:
            log_warning(kernel_logger, "Operacion desconocida de CPU - Dispatch");
            break;
        }
    }
}