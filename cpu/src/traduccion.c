#include "traduccion.h"
#include "tlb.h"
#include "consultar_memoria.h"

void traducir_direccion_logica (uint32_t direccion_logica, int* numero_pagina, int*desplazamiento, uint32_t* direccion_fisica){
    int aux_nro_pag = floor(direccion_logica / tamanio_pagina);
    numero_pagina = &aux_nro_pag;

    int aux_desplazamiento = direccion_logica - *numero_pagina * tamanio_pagina;
    desplazamiento = &aux_desplazamiento;

    int marco =  obtener_marco(*numero_pagina, *desplazamiento);

    uint32_t aux_dir_fisica = marco * tamanio_pagina + desplazamiento;
    direccion_fisica = &aux_dir_fisica;

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

t_list* separar_en_paginas (uint32_t direccion_logica,  int tamanio_dato){ //devuelve lista de direcciones fisicas, con el tamanio a l/e en c/u
    int numero_pagina; // = floor(direccion_logica / tamanio_pagina);
    int desplazamiento; // = direccion_logica - numero_pagina * tamanio_pagina;

    t_list* lista_dir_fis = malloc(sizeof(t_list));

    uint32_t dir_fisica_inicial ; //= traducir_direccion_logica (direccion_logica);

    traducir_direccion_logica(direccion_logica, &numero_pagina,&desplazamiento,&dir_fisica_inicial);
    t_direccion_fisica* direccion = malloc(sizeof(t_direccion_fisica));
    direccion->direccion_fisica = dir_fisica_inicial;
    int espacio_restante = tamanio_pagina - desplazamiento;
    direccion->tamanio_dato = espacio_restante;

    list_add(lista_dir_fis,direccion);

    int queda_escribir = tamanio_dato - espacio_restante;

    int paginas_restantes;

    if(queda_escribir > 0){
        paginas_restantes= ceil(queda_escribir/tamanio_pagina);

        for (int i = 1; i <= paginas_restantes; i++)
        {
            numero_pagina++;
            int marco = obtener_marco(numero_pagina,0);
            uint32_t dir_nueva = marco * tamanio_pagina;
            t_direccion_fisica* nueva_direccion = malloc(sizeof(t_direccion_fisica));
            nueva_direccion -> direccion_fisica = dir_nueva;
            if((queda_escribir - tamanio_pagina) < 0){
                nueva_direccion -> tamanio_dato = queda_escribir;
            } else {
                nueva_direccion -> tamanio_dato = tamanio_pagina;
            }
            list_add(lista_dir_fis,nueva_direccion);
        }
        
    }

    return lista_dir_fis;

}