#ifndef GESTOR_KERNEL_H_
#define GESTOR_KERNEL_H_

#include <utils/hello.h>

// VARIABLES GLOBALES
extern t_log *kernel_logger;
extern t_config *kernel_config;
extern int identificador_pid;
extern bool planif_iniciada;

// VALORES DEL CONFIG
extern char *puerto_escucha, *ip_memoria, *puerto_memoria, *ip_cpu, *dispatch, *interrupt, *algoritmo_planificacion;
extern int quantum, grado_multiprogramacion;
extern char **recursos;
extern int *instancias_recursos;
extern t_list *lista_recursos;
extern t_list *lista_io_conectadas;

// SOCKETS
extern int socket_conexion_cpu_dispatch, socket_conexion_cpu_interrupt, socket_conexion_memoria, socket_escucha, socket_interfaz;

// LISTAS DE PLANIFICACION
extern t_list *cola_new, *cola_ready, *cola_execute, *cola_block, *cola_exit;

// TODO LISTAS DE MANEJO DE IO
extern t_list *cola_block_io;
extern t_list *cola_block_fs;

// SEMAFOROS
extern pthread_mutex_t mutex_pid;
extern pthread_mutex_t mutex_cola_new;
extern pthread_mutex_t mutex_cola_ready;
extern pthread_mutex_t mutex_cola_exec;
extern pthread_mutex_t mutex_cola_block;
extern pthread_mutex_t mutex_cola_block_io;
extern pthread_mutex_t mutex_cola_block_fs;
extern pthread_mutex_t mutex_cola_exit;
extern pthread_mutex_t mutex_lista_io;
extern sem_t sem_multiprogramacion;
extern sem_t sem_new;
extern sem_t sem_ready;
extern sem_t sem_exec;
extern sem_t sem_block_return;
extern sem_t sem_exit;

// STRUCTS PARA TESTS
extern t_registros registros1;
extern t_registros registros2;
extern t_registros registros3;
extern t_pcb pcb1;
extern t_pcb pcb2;
extern t_pcb pcb3;

#endif