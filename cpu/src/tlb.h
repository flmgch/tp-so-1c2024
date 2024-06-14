#ifndef TLB_
#define TLB_

#include "gestor_cpu.h"

int tlb_buscar(uint32_t  pid, int numero_pagina);
void tlb_agregar(int pid, uint32_t pagina, uint32_t marco);
int encontrar_entrada_lru();

#endif