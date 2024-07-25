#include "ciclo_instrucciones.h"
#include "solicitar_instruccion.h"
#include "instrucciones.h"

const char* instruccion_to_string(cod_instruccion codigo) {
    switch (codigo) {
        case SET: return "SET";
        case MOV_IN: return "MOV_IN";
        case MOV_OUT: return "MOV_OUT";
        case SUM: return "SUM";
        case SUB: return "SUB";
        case JNZ: return "JNZ";
        case RESIZE: return "RESIZE";
        case COPY_STRING: return "COPY_STRING";
        case WAIT: return "WAIT";
        case SIGNAL: return "SIGNAL";
        case IO_STDIN_READ: return "IO_STDIN_READ";
        case IO_STDOUT_WRITE: return "IO_STDOUT_WRITE";
        case IO_FS_CREATE: return "IO_FS_CREATE";
        case IO_FS_DELETE: return "IO_FS_DELETE";
        case IO_FS_TRUNCATE: return "IO_FS_TRUNCATE";
        case IO_FS_WRITE: return "IO_FS_WRITE";
        case IO_FS_READ: return "IO_FS_READ";
        case IO_GEN_SLEEP: return "IO_SGEN_SLEEP";
        case EXIT: return "EXIT";
        default: return "UNKNOWN";
    }
}

void decode(u_int32_t dir_instruccion){
    solicitar_instruccion(dir_instruccion);
    sem_wait(&sem_instruccion);
    log_info(cpu_logger,  "PID: %d - Ejecutando: %s - %s %s %s %s %s ",pcb->pid,instruccion_to_string(instruccion.codigo_instruccion), instruccion.param1, instruccion.param2, instruccion.param3, instruccion.param4, instruccion.param5);
    switch(instruccion.codigo_instruccion){
		case SET:
			ejecutar_set(instruccion.param1, instruccion.param2);
            check_interrupt();
            fetch();
			break;
        case SUM:
            ejecutar_sum(instruccion.param1, instruccion.param2);
            check_interrupt();
            fetch();
            break;
        case SUB:
            ejecutar_sub(instruccion.param1, instruccion.param2);
            check_interrupt();
            fetch();
            break;
        case JNZ:
            ejecutar_jnz(instruccion.param1, instruccion.param2);
            check_interrupt();
            fetch();
            break;
        case IO_GEN_SLEEP:
            ejecutar_io_gen_sleep(instruccion.param1, instruccion.param2);
            break;
        case MOV_IN:
            ejecutar_mov_in(instruccion.param1, instruccion.param2);
            check_interrupt();
            fetch();
            break;
        case MOV_OUT:
            ejecutar_mov_out(instruccion.param1, instruccion.param2);
            check_interrupt();
            fetch();
            break;
        case RESIZE:
            ejecutar_resize(instruccion.param1);
            sem_wait(&sem_resize);
            if (aux_resize == 1)
            {
                check_interrupt();
                fetch();
            }

            break;
        case COPY_STRING:
            ejecutar_copy_string(instruccion.param1);
            check_interrupt();
            fetch();
            break;
        case SIGNAL:
            ejecutar_signal(instruccion.param1);
            // fetch();
            break;
        case WAIT:
            ejecutar_wait(instruccion.param1);
            // fetch();
            break;    
        case IO_STDIN_READ:
            ejecutar_io_stdin_read(instruccion.param1, instruccion.param2, instruccion.param3);
            break;
        case IO_STDOUT_WRITE:
            ejecutar_io_stdout_write(instruccion.param1, instruccion.param2, instruccion.param3);
            break;
        case IO_FS_CREATE:
            ejecutar_io_fs_create(instruccion.param1, instruccion.param2);
            break;
        case IO_FS_DELETE:
            ejecutar_io_fs_delete(instruccion.param1, instruccion.param2);
            break;
        case IO_FS_TRUNCATE:
            ejecutar_io_fs_truncate(instruccion.param1, instruccion.param2, instruccion.param3);
            break;
        case IO_FS_WRITE:
            ejecutar_io_fs_write(instruccion.param1, instruccion.param2, instruccion.param3, instruccion.param4, instruccion.param5);
            break;
        case IO_FS_READ:
            ejecutar_io_fs_read(instruccion.param1, instruccion.param2, instruccion.param3, instruccion.param4, instruccion.param5);
            break;
        case EXIT:
            ejecutar_exit();
            pcb->program_counter = 0;
            break;
        default:
			log_error(cpu_logger, "Instruccion no reconocida");
			return;
    }
}

void fetch (){ 
    if(flag_execute == true) {
    u_int32_t instruccion_a_ejecutar = pcb->program_counter;
    log_info(cpu_logger, "PID: %d - FETCH - Program Counter: %d", pcb->pid, instruccion_a_ejecutar);

    aux_resize = 0;
    pcb->program_counter += 1;
    decode(instruccion_a_ejecutar);
    }
}

void ejecutar_proceso(){
    fetch();
}

void check_interrupt()
{
    if (sem_trywait(&sem_interrupt_quantum) == 0)
    {
        // OCURRIO UN FIN DE QUANTUM
        if (pcb->pid == pid_interrupcion)
        {
            log_info(cpu_logger, "INTERRUPCION POR FIN DE QUANTUM");
            pthread_mutex_lock(&mutex_flag_execute);
            flag_execute = false;
            pthread_mutex_unlock(&mutex_flag_execute);
            pcb->motivo_exit = FIN_QUANTUM;
            t_buffer *buffer = crear_buffer();
            agregar_pcb_a_buffer(buffer, pcb);
            agregar_cop_a_buffer(buffer, CAMBIAR_ESTADO);
            agregar_estado_a_buffer(buffer, READY);
            t_paquete *paquete = crear_super_paquete(ENVIO_PCB, buffer);
            enviar_paquete(paquete, socket_kernel_dispatch);
            eliminar_paquete(paquete);
        }
        else
        {
            log_info(cpu_logger, "INTERRUPCION DESCARTADA, PID != PID EJECUTANDO");
        }
    }
    if (sem_trywait(&sem_interrupt_fp) == 0)
    {
        // OCURRIO UN FINALIZAR PROCESO
        log_info(cpu_logger, "INTERRUPCION POR FINALIZAR PROCESO");
        pthread_mutex_lock(&mutex_flag_execute);
        flag_execute = false;
        pthread_mutex_unlock(&mutex_flag_execute);
        pcb->motivo_exit = INTERRUPTED_BY_USER;
        t_buffer *buffer = crear_buffer();
        agregar_pcb_a_buffer(buffer, pcb);
        agregar_cop_a_buffer(buffer, CAMBIAR_ESTADO);
        agregar_estado_a_buffer(buffer, FINISH_ERROR);
        t_paquete *paquete = crear_super_paquete(ENVIO_PCB, buffer);
        enviar_paquete(paquete, socket_kernel_dispatch);
        eliminar_paquete(paquete);
    }
}