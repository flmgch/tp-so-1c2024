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

            // ACTUALIZO EL PCB QUE ESTABA EN EXEC
            t_pcb *pcb = remover_pcb(cola_execute, &mutex_cola_exec);
            pcb = contexto_recibido;
            int cop = extraer_int_de_buffer(buffer);
            // RECIBO OTRO COP PARA SABER QUE TENGO QUE HACER CON EL PCB
            
            switch (cop)
            {
            case CAMBIAR_ESTADO:
                log_info(kernel_logger, "Recibi un aviso de cambio de estado");
                estado_proceso nuevo_estado = extraer_estado_proceso_de_buffer(buffer);
                procesar_cambio_estado(pcb, nuevo_estado);
                sem_post(&sem_exec);
                break;
            case ATENDER_WAIT:
                log_info(kernel_logger, "Recibi un aviso de atender WAIT");
				char* recurso_wait = extraer_string_de_buffer(buffer);
				atender_wait(pcb, recurso_wait);
				free(recurso_wait);
				break;
            case OUT_OF_MEMORY:
                log_info(kernel_logger, "Llego el out of memory");
                break;
            case ATENDER_SIGNAL:
                log_info(kernel_logger, "Recibi un aviso de atender SIGNAL");
				char* recurso_signal = extraer_string_de_buffer(buffer);
				atender_signal(pcb, recurso_signal);
				free(recurso_signal);
				break;
            }
            destruir_buffer(buffer);
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

void atender_wait(t_pcb *pcb, char *recurso)
{
	t_recurso *recursobuscado = buscar_recurso(recurso);

    // ! SI DA ERROR EL RECURSO
	if (recursobuscado->id == -1)
	{
		log_error(kernel_logger, "No existe el recurso solicitado: %s", recurso);
		pcb->motivo_exit = INVALID_RESOURCE;
		agregar_pcb(cola_exit, pcb, &mutex_cola_exit);
		sem_post(&sem_exit);
		sem_post(&sem_exec);
	}
	else
	{
		recursobuscado->instancias--;
		log_info(kernel_logger, "PID: %d - Wait: %s - Instancias: %d", pcb->pid, recurso, recursobuscado->instancias);

        // ! SI NO HAY SUFICIENTES INSTANCIAS DEL RECURSO DISPONIBLES
		if (recursobuscado->instancias < 0)
		{
			cambiar_estado(pcb, BLOCK);
			log_info(kernel_logger, "PID: %d - Bloqueado por: %s", pcb->pid, recurso);
            // TODO Calcular quantum remanente del proceso para usarlo en VRR
			// calcular_estimacion(pcb);
			agregar_pcb(recursobuscado->cola_block_asignada, pcb, &recursobuscado->mutex_asignado);
			sem_post(&sem_exec);
		}
        // SI SALE TODO BIEN, VUELVE A MANDAR EL PCB AL CPU
		else
		{
			dispatch_pcb(pcb);
		}
	}
}

t_recurso *buscar_recurso(char *recurso)
{
	int longitudLista = list_size(lista_recursos);
	t_recurso *recursobuscado;
	for (int i = 0; i < longitudLista; i++)
	{
		recursobuscado = list_get(lista_recursos, i);
		if (strcmp(recursobuscado->recurso, recurso) == 0)
		{
			return recursobuscado;
		}
	}
    // ! SI NO ENCUENTRA EL RECURSO, PONE ID = -1
	recursobuscado->id = -1;
	return recursobuscado;
}

void atender_signal(t_pcb *pcb, char *recurso)
{
	t_recurso *recursobuscado = buscar_recurso(recurso);

    // ! SI DA ERROR EL RECURSO
	if (recursobuscado->id == -1)
	{
		log_error(kernel_logger, "No existe el recurso solicitado: %s", recurso);
		pcb->motivo_exit = INVALID_RESOURCE;
		agregar_pcb(cola_exit, pcb, &mutex_cola_exit);
		sem_post(&sem_exit);
		sem_post(&sem_exec);
	}
	else
	{
		recursobuscado->instancias++;
		log_info(kernel_logger, "PID: %d - Signal: %s - Instancias: %d", pcb->pid, recurso, recursobuscado->instancias);

        // DESBLOQUEA AL PRIMER PROCESO DE LA COLA DE BLOQUEADOS DEL RECURSO SI CORRESPONDE
		if (recursobuscado->instancias <= 0)
		{
			t_pcb *pcb_bloqueado = remover_pcb(recursobuscado->cola_block_asignada, &recursobuscado->mutex_asignado);
			agregar_pcb(cola_block, pcb_bloqueado, &mutex_cola_block);
			sem_post(&sem_block_return);
		}

        // MANDA EL PROCESO RECIBIDO A EXEC
		dispatch_pcb(pcb);
	}
}