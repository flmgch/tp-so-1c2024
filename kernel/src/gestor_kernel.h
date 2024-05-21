#ifndef GESTOR_KERNEL_H_
#define GESTOR_KERNEL_H_

#include <utils/hello.h>

// VARIABLES GLOBALES
extern t_log *kernel_logger;
extern t_config *kernel_config;
extern int identificador_pid;
extern int contador_pcbs;
extern pthread_mutex_t mutex_pid;

// VALORES DEL CONFIG
extern char *puerto_escucha, *ip_memoria, *puerto_memoria, *ip_cpu, *dispatch, *interrupt, *algoritmo_planificacion, *grado_multiprogramacion;
extern int quantum;
extern t_list *recursos, *instancias_recursos;

// SOCKETS
extern int socket_conexion_cpu_dispatch, socket_conexion_cpu_interrupt, socket_conexion_memoria, socket_escucha, socket_interfaz;

#endif