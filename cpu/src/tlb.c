#include "tlb.h"

int tlb_buscar(uint32_t pid, int numero_pagina){
    contador_acceso++;
    for (int i = 0; i < tlb_entradas; ++i) {
        if (tlb[i].pid == pid && tlb[i].pagina == numero_pagina) {
            tlb[i].ultimo_acceso=contador_acceso;
            return i; 
        }
    }
    return -1;
}

void tlb_agregar(uint32_t pid, int pagina, int marco) {
    //si hay espacio, lo agrego de una
    if(cantidad_entradas_tlb!=0){
    if (tlb_entradas < cantidad_entradas_tlb) {
        tlb[tlb_entradas].pid = pid;
        tlb[tlb_entradas].pagina = pagina;
        tlb[tlb_entradas].marco = marco;
        tlb[tlb_entradas].ultimo_acceso = contador_acceso; // para el lru
        tlb_entradas++;
    } else {
        contador_acceso++;
        //no tengo espacio, sacrifico una entrada
        int index_reemplazo; //q entrada voy a sacrificar
        if(strcmp(algoritmo_tlb,"LRU")==0){ //si es lru, sacrifico el q tiene menos accesos
            index_reemplazo = encontrar_entrada_lru();
        } else if (strcmp(algoritmo_tlb,"FIFO")==0){ //si es fifo, saco el primero que agregue antes
            index_reemplazo = tlb_proximo_reemplazo;
        } else {
            log_error(cpu_logger,"Algoritmo de reemplazo invalido");
            exit(EXIT_FAILURE);
        }
    
        tlb[index_reemplazo].pid = pid;
        tlb[index_reemplazo].pagina = pagina;
        tlb[index_reemplazo].marco = marco;
        tlb[index_reemplazo].ultimo_acceso = contador_acceso;                        // para el lru
        tlb_proximo_reemplazo = (tlb_proximo_reemplazo + 1) % cantidad_entradas_tlb; // para el fifo
    }
    }
}

int encontrar_entrada_lru() {
    int lru_index = 0;
    int min_acceso = tlb[0].ultimo_acceso;

    for (int i = 0; i < tlb_entradas; ++i)
    { // busco la entrada cn menor acceso (accedido hace mas) para sacrificar
        if (tlb[i].ultimo_acceso < min_acceso) {
            min_acceso = tlb[i].ultimo_acceso;
            lru_index = i;
        }
    }

    return lru_index;
}