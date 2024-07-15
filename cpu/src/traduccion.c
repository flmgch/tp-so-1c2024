#include "traduccion.h"
#include "tlb.h"

void traducir_direccion_logica(uint32_t direccion_logica)
{
    numero_pagina = floor(direccion_logica / tamanio_pagina);
    int aux_nro_pag = numero_pagina;

    desplazamiento = direccion_logica - (numero_pagina * tamanio_pagina);
    int desp = desplazamiento;

    int marco = obtener_marco(numero_pagina, desplazamiento);

    dir_fisica_inicial = desplazamiento + marco * tamanio_pagina;
    uint32_t dir_fi = dir_fisica_inicial;
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
        consultar_memoria(pcb->pid, numero_pagina);
        sem_wait(&sem_consultar_memoria);
        int marco = aux_marco;
        tlb_agregar(pcb->pid, numero_pagina, marco);
        log_info(cpu_logger, "PID: %d - OBTENER MARCO - Página: %d - Marco: %d",pcb->pid, numero_pagina, marco);
        return marco;
    }
}

t_list *separar_en_paginas(uint32_t direccion_logica, int tamanio_dato)
{ // devuelve lista de direcciones fisicas, con el tamanio a l/e en c/u
  //  = direccion_logica - numero_pagina * tamanio_pagina;

    t_list *lista_dir_fis = malloc(sizeof(t_list));

    //= traducir_direccion_logica (direccion_logica);

    traducir_direccion_logica(direccion_logica);
    t_direccion_fisica *direccion = malloc(sizeof(t_direccion_fisica));
    direccion->direccion_fisica = dir_fisica_inicial;
    int espacio_restante = tamanio_pagina - desplazamiento;

    if (tamanio_dato > espacio_restante)
    {
        direccion->tamanio_dato = espacio_restante;

        list_add(lista_dir_fis, direccion);

        int queda_escribir = tamanio_dato - espacio_restante;

        int paginas_restantes = ceil(queda_escribir / tamanio_pagina);

        for (int i = 1; i <= paginas_restantes; i++)
        {
            numero_pagina++;
            int marco = obtener_marco(numero_pagina, 0);
            uint32_t dir_nueva = marco * tamanio_pagina;
            t_direccion_fisica *nueva_direccion = malloc(sizeof(t_direccion_fisica));
            nueva_direccion->direccion_fisica = dir_nueva;

            if (queda_escribir < tamanio_pagina)
            {
                nueva_direccion->tamanio_dato = queda_escribir;
                //
            }
            else
            {
                nueva_direccion->tamanio_dato = tamanio_pagina;
                //
            }
            list_add(lista_dir_fis, nueva_direccion);
            //
        }
    }
    else
    {
        direccion->tamanio_dato = tamanio_dato;
        list_add(lista_dir_fis, direccion);
        //
    }
    return lista_dir_fis;
}

void consultar_memoria(uint32_t pid, int numero_pagina)
{
    // envio pid y pagina a cpu
    t_buffer *un_buffer = crear_buffer();
    agregar_int_a_buffer(un_buffer, numero_pagina);
    agregar_uint32_a_buffer(un_buffer, pid);
    t_paquete *paquete = crear_super_paquete(ACCESO_TABLA_PAGINAS, un_buffer);
    enviar_paquete(paquete, socket_memoria);
    eliminar_paquete(paquete);
}