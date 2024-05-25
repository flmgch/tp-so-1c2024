#ifndef INICIALIZAR_KERNEL_H_
#define INICIALIZAR_KERNEL_H_

#include "gestor_kernel.h"

void inicializar_kernel();
void inicializar_logs();
void inicializar_configs();
void inicializar_semaforos();
t_list *inicializar_recursos();
int *string_to_int_array(char **array_de_strings);

#endif