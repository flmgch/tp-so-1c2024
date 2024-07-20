#include "cpu_kernel_interrupt.h"

void atender_kernel_interrupt()
{
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
            pthread_mutex_lock(&mutex_flag_execute);
            flag_execute = false;
            pthread_mutex_unlock(&mutex_flag_execute);
            pcb->motivo_exit = FIN_QUANTUM;
            t_buffer* buffer = crear_buffer();
            agregar_pcb_a_buffer(buffer, pcb);
            agregar_cop_a_buffer(buffer, CAMBIAR_ESTADO);
            agregar_estado_a_buffer(buffer, READY);
            t_paquete* paquete = crear_super_paquete(ENVIO_PCB, buffer);
            enviar_paquete(paquete, socket_kernel_dispatch);
            eliminar_paquete(paquete);
            break;
        }
        case INT_FINALIZAR_PROCESO:{
            pthread_mutex_lock(&mutex_flag_execute);
            flag_execute = false;
            pthread_mutex_unlock(&mutex_flag_execute);
            pcb->motivo_exit = INTERRUPTED_BY_USER;
            t_buffer* buffer = crear_buffer();
            agregar_pcb_a_buffer(buffer, pcb);
            agregar_cop_a_buffer(buffer, CAMBIAR_ESTADO);
            agregar_estado_a_buffer(buffer, FINISH_ERROR);
            t_paquete* paquete = crear_super_paquete(ENVIO_PCB, buffer);
            enviar_paquete(paquete, socket_kernel_dispatch);
            eliminar_paquete(paquete);
            break;
        }
        case -1:
            log_error(cpu_logger, "Se desconecto kernel-interrupt");
            control_key = 0;
            break;
        default:
            log_warning(cpu_logger, "Operacion desconocida de kernel-interrupt");
            break;
        }
    }
};

