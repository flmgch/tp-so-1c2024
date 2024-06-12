#include "kernel_cpu_dispatch.h"
#include "planificador.h"

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
        case ENVIO_PCB:
            // OBTENGO EL CONTEXTO DE LA CPU
            t_buffer *buffer = recibir_buffer(socket_conexion_cpu_dispatch);
            t_pcb *contexto_recibido = extraer_pcb_de_buffer(buffer);
            log_info(kernel_logger, "Recibi un PCB que me envio el CPU");
            destruir_buffer(buffer);

            // ACTUALIZO EL PCB QUE ESTABA EN EXEC
            t_pcb *pcb = remover_pcb(cola_execute, &mutex_cola_exec);
            pcb = contexto_recibido;

            // RECIBO OTRO COP PARA SABER QUE TENGO QUE HACER CON EL PCB
            recv(socket_conexion_cpu_dispatch, &cod_op, sizeof(op_code), 0);
            switch (cod_op)
            {
            case CAMBIAR_ESTADO:
                log_info(kernel_logger, "Recibi un aviso de cambio de estado");
                t_buffer *buffer = recibir_buffer(socket_conexion_cpu_dispatch);
                estado_proceso nuevo_estado = extraer_estado_proceso_de_buffer(buffer);
                procesar_cambio_estado(pcb, nuevo_estado);
                sem_post(&sem_exec);
                destruir_buffer(buffer);
                break;
            }
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

void procesar_cambio_estado(t_pcb *pcb, estado_proceso estado_nuevo)
{

    switch (estado_nuevo)
    {
    case READY:
        pasar_a_ready(pcb);
        sem_post(&sem_ready);
        break;
    case FINISH_EXIT:
        cambiar_estado(pcb, estado_nuevo);
        pcb->motivo_exit = SUCCESS;
        agregar_pcb(cola_exit, pcb, &mutex_cola_exit);
        sem_post(&sem_exit);
        break;
    case FINISH_ERROR:
        cambiar_estado(pcb, estado_nuevo);
        agregar_pcb(cola_exit, pcb, &mutex_cola_exit);
        sem_post(&sem_exit);
        break;
    default:
        log_error(kernel_logger, "Cambio de estado no reconocido");
        break;
    }
}
