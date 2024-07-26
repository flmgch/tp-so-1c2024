#include "cpu_kernel_interrupt.h"

void atender_kernel_interrupt()
{   
    t_buffer* buffer;
    bool control_key = 1;
    while (control_key)
    {   
        int cod_op = recibir_operacion(socket_kernel_interrupt);
        switch (cod_op)
        {
        case MENSAJE:
            //
            break;
        case PAQUETE:
            //
            break;
        case INT_FIN_QUANTUM:{
            buffer = recibir_buffer(socket_kernel_interrupt);
            int pid = extraer_int_de_buffer(buffer);
            pid_interrupcion = pid;
            sem_post(&sem_interrupt_quantum);
            destruir_buffer(buffer);
            break;
        }
        case INT_FINALIZAR_PROCESO:{
            sem_post(&sem_interrupt_fp);
            break;
        }
        case -1:
            log_error(cpu_logger, "Se desconecto kernel-interrupt");
            control_key = 0;
            break;
        default:
            log_warning(cpu_logger, "Operacion desconocida de kernel-interrupt");
            control_key = 0;
            break;
        }
    }
};

