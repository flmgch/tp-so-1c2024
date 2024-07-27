#ifndef INTERFAZ
#define INTERFAZ

#include "gestor_io.h"

void atender_generica(t_buffer *buffer);
void atender_stdin(t_buffer *buffer);
void clear_stdin(void);
void confirmar_escritura(t_buffer *buffer);
void atender_stdout(t_buffer *buffer);
void imprimir_resultado_lectura(t_buffer *buffer);


#endif