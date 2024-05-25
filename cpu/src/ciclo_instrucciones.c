#include "ciclo_instrucciones.h"
#include "solicitar_instruccion.h"

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
        case IO_GEN_SLEEP: return "IO_GEN_SLEEP";
        case IO_STDIN_READ: return "IO_STDIN_READ";
        case IO_STDOUT_WRITE: return "IO_STDOUT_WRITE";
        case IO_FS_CREATE: return "IO_FS_CREATE";
        case IO_FS_DELETE: return "IO_FS_DELETE";
        case IO_FS_TRUNCATE: return "IO_FS_TRUNCATE";
        case IO_FS_WRITE: return "IO_FS_WRITE";
        case IO_FS_READ: return "IO_FS_READ";
        case EXIT: return "EXIT";
        default: return "UNKNOWN";
    }
}

void decode(u_int32_t dir_instruccion){
    t_instruccion instruccion = solicitar_instruccion(dir_instruccion);

    log_info(cpu_logger,  "PID: %d - Ejecutando: %s - %s %s %s %s %s ",pcb->pid,instruccion_to_string(instruccion.codigo_instruccion), instruccion.param1, instruccion.param2, instruccion.param3, instruccion.param4, instruccion.param5);
    switch(instruccion.codigo_instruccion){
		case SET:
			ejecutar_set(instruccion.param1, instruccion.param2);
			break;
        case MOV_IN:
            break;
        case MOV_OUT:
            break;
        case SUM:
            ejecutar_sum(instruccion.param1, instruccion.param2);
            break;
        case SUB:
            ejecutar_sub(instruccion.param1, instruccion.param2);
            break;
        case JNZ:
            ejecutar_jnz(instruccion.param1, instruccion.param2);
            break;
        case IO_GEN_SLEEP:
            ejecutar_io_gen_sleep(instruccion.param1, instruccion.param2);
            break;
    }
}

void fetch (){ 
    u_int32_t instruccion_a_ejecutar = pcb->program_counter;
    log_info(cpu_logger, "PID: %d - FETCH - Program Counter: %d", pcb->pid, instruccion_a_ejecutar);

    decode(instruccion_a_ejecutar);
    pcb->program_counter += 1;
}

void ejecutar_proceso(){
    fetch();
}

