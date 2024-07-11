#ifndef TRADUCCION_
#define TRADUCCION_

#include "gestor_cpu.h"

void traducir_direccion_logica (uint32_t direccion_logica, int* numero_pagina, int*desplazamiento, uint32_t* direccion_fisica);
int obtener_marco(int numero_pagina,int desplazamiento);
t_list* separar_en_paginas (uint32_t direccion_logica,  int tamanio_dato);


#endif