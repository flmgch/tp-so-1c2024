#ifndef KERNEL_H_
#define KERNEL_H_

#include "gestor_kernel.h"
#include "inicializar_kernel.h"
#include "kernel_cpu_dispatch.h"
#include "kernel_cpu_interrupt.h"
#include "kernel_memoria.h"
#include "kernel_interfaz.h"

// VARIABLES GLOBALES
  t_log *kernel_logger;
  t_config *kernel_config;
  char *puerto_escucha, *ip_memoria, *puerto_memoria, *ip_cpu, *dispatch, *interrupt, *algoritmo_planificacion, *grado_multiprogramacion;
  int quantum;
  t_list *recursos, *instancias_recursos;
  
// SOCKETS
  int socket_conexion_cpu_dispatch, socket_conexion_cpu_interrupt, socket_conexion_memoria, socket_escucha, socket_interfaz;

#endif