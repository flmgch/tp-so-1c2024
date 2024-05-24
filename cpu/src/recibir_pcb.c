#include "recibir_pcb.h"

void atender_recibir_pcb(t_buffer* buffer){
    pcb->pid = extraer_uint32_de_buffer(buffer);
    pcb->program_counter = extraer_uint32_de_buffer(buffer);
    pcb->quantum = extraer_uint32_de_buffer(buffer);
    pcb->registros_cpu = extraer_registros_de_buffer(buffer);

    free(pcb);
};