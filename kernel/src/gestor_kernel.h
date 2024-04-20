#ifndef GESTOR_KERNEL_H_
#define GESTOR_KERNEL_H_

#include <utils/hello.h>
#include <stdlib.h>
#include <stdio.h>

// VARIABLES GLOBALES
extern t_log *kernel_logger;
extern t_config *kernel_config;
extern char *puerto_escucha, *ip_memoria, *puerto_memoria, *ip_cpu, *dispatch, *interrupt, *algoritmo_planificacion, *grado_multiprogramacion;
extern int quantum;
extern t_list *recursos, *instancias_recursos;

#endif