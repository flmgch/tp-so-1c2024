#include "recibir_pcb.h"

void atender_recibir_pcb(t_buffer* buffer){
    pcb->pid = extraer_uint32_de_buffer(buffer);
    pcb->program_counter = extraer_uint32_de_buffer(buffer);
    pcb->estado = extraer_estado_proceso_de_buffer(buffer);
    pcb->motivo_block = extraer_motivo_block_de_buffer(buffer);
    pcb->motivo_exit = extraer_motivo_exit_de_buffer(buffer);
    pcb->registros_cpu = extraer_registros_de_buffer(buffer);
    pcb->quantum_remanente = extraer_uint32_de_buffer(buffer);
};