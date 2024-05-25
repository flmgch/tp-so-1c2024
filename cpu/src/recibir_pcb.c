#include "recibir_pcb.h"

void atender_recibir_pcb(t_buffer* buffer){
    pcb->process_id= extraer_uint32_de_buffer(buffer);
    pcb->program_counter = extraer_uint32_de_buffer(buffer);
    pcb->quantum_remanente = extraer_uint32_de_buffer(buffer);
    pcb->registros_cpu = extraer_registros_de_buffer(buffer);

    free(pcb);
};