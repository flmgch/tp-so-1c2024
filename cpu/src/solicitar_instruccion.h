#ifndef SOLICITAR_INSTRUCCION_H_
#define SOLICITAR_INSTRUCCION_H_

#include "gestor_cpu.h"

t_instruccion solicitar_instruccion(u_int32_t direccion_instruccion);
t_instruccion formular_instruccion(char* string);
cod_instruccion obtener_codigo_instruccion(char *operation);

#endif