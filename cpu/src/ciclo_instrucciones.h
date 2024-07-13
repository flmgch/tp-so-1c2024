#ifndef CICLO_INSTRUCCIONES_H_
#define CICLO_INSTRUCCIONES_H_

#include "gestor_cpu.h"

void ejecutar_proceso();
void fetch();
void decode(u_int32_t dir_instruccion);
const char *instruccion_to_string(cod_instruccion codigo);

#endif