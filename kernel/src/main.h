#ifndef KERNEL_H_
#define KERNEL_H_

#include <utils/hello.h>

// VARIABLES GLOBALES
  t_log *kernel_logger;
  t_config *kernel_config;
  char *puerto_escucha, *ip_memoria, *puerto_memoria, *ip_cpu, *dispatch, *interrupt, *algoritmo_planificacion, *grado_multiprogramacion;
  int quantum;
  t_list *recursos, *instancias_recursos;
  
#endif