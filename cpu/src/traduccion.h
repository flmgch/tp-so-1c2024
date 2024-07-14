#ifndef TRADUCCION_
#define TRADUCCION_

#include "gestor_cpu.h"

void traducir_direccion_logica(uint32_t direccion_logica);
int obtener_marco(int numero_pagina,int desplazamiento);
t_list* separar_en_paginas (uint32_t direccion_logica,  int tamanio_dato);
void consultar_memoria(uint32_t pid, int numero_pagina);

#endif