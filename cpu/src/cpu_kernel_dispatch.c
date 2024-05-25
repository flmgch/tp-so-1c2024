#include "cpu_kernel_dispatch.h"

void atender_kernel_dispatch()
{
    t_buffer *un_buffer;
    bool control_key = 1;
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
        case ENVIO_PCB:
           un_buffer=recibir_buffer(socket_kernel_dispatch);
           atender_recibir_pcb(un_buffer);
           destruir_buffer(un_buffer);
           log_info(cpu_logger, "Recibí el contexto del proceso %d y se inicia el ciclo de instruccion", pcb->pid);
           ejecutar_proceso();
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