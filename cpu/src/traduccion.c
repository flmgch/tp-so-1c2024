#include "traduccion.h"
#include "tlb.h"
#include "consultar_memoria.h"

uint32_t traducir_direccion_logica (uint32_t direccion_logica){
    int  numero_pagina = floor(direccion_logica / tamanio_pagina);
    int desplazamiento = direccion_logica - numero_pagina * tamanio_pagina;

    int marco =  obtener_marco(numero_pagina,desplazamiento);
    uint32_t direccion_fisica = marco * tamanio_pagina + desplazamiento;

    return direccion_fisica;
}

int obtener_marco(int numero_pagina,int desplazamiento){
    int index_tlb = tlb_buscar(pcb->pid, numero_pagina);
    if (index_tlb != -1) {
        // tlb Hit
        log_info(cpu_logger, "PID: %d - TLB HIT - Pagina: %d",pcb->pid,numero_pagina);
        log_info(cpu_logger, "PID: %d - OBTENER MARCO - Página: %d - Marco: %d",pcb->pid, numero_pagina, tlb[index_tlb].marco);
        return tlb[index_tlb].marco;
    } else {
        // tlb Miss
        log_info(cpu_logger,"PID: %d- TLB MISS - Pagina: %d",pcb->pid,numero_pagina);
        int marco = consultar_memoria(pcb->pid,numero_pagina);
        tlb_agregar(pcb->pid, numero_pagina, marco);
        log_info(cpu_logger, "PID: %d - OBTENER MARCO - Página: %d - Marco: %d",pcb->pid, numero_pagina, marco);
        return marco;
    }
}
