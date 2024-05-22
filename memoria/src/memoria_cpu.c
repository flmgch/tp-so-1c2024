#include "memoria_cpu.h"

void atender_cpu()
{

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
        case ACCESO_ESPACIO_USUARIO_CPU:
            //
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