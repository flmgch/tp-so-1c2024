#ifndef CONSULTAR_MEMORIA_
#define CONSULTAR_MEMORIA_

#include "gestor_cpu.h"

int consultar_memoria(uint32_t pid,int numero_pagina);
int recibir_marco(t_buffer *un_buffer);

#endif