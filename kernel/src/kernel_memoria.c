#include "kernel_memoria.h"

void atender_memoria()
{
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(socket_conexion_memoria);
        switch (cod_op)
        {
        case MENSAJE:
            //
            break;
        case PAQUETE:
            //
            break;
        case -1:
            log_error(kernel_logger, "Se desconecto Memoria");
            control_key = 0;
            break;
        default:
            log_warning(kernel_logger, "Operacion desconocida de Memoria");
            control_key = 0;
            break;
        }
    }
};