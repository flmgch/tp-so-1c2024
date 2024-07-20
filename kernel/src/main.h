#ifndef KERNEL_H_
#define KERNEL_H_

#include "gestor_kernel.h"
#include "inicializar_kernel.h"
#include "kernel_cpu_dispatch.h"
#include "kernel_cpu_interrupt.h"
#include "kernel_memoria.h"
#include "kernel_interfaz.h"
#include "consola.h"
#include "planificador.h"

// VARIABLES GLOBALES
  t_log *kernel_logger;
  t_config *kernel_config;
  int identificador_pid = 1;
  bool planif_iniciada = false;

// VALORES DEL CONFIG
  char *puerto_escucha, *ip_memoria, *puerto_memoria, *ip_cpu, *dispatch, *interrupt, *algoritmo_planificacion;
  int quantum, grado_multiprogramacion;
  char **recursos;
  int *instancias_recursos;
  t_list *lista_recursos;
  t_list *lista_io_conectadas;

// SOCKETS
  int socket_conexion_cpu_dispatch, socket_conexion_cpu_interrupt, socket_conexion_memoria, socket_escucha;

// LISTAS DE PLANIFICACION
  t_list *cola_new, *cola_ready, *cola_ready_prioridad, *cola_execute, *cola_block, *cola_exit;

// HILO PARA QUANTUM
  pthread_t hilo_quantum;

// SEMAFOROS
  pthread_mutex_t mutex_pid;
  pthread_mutex_t mutex_cola_new;
  pthread_mutex_t mutex_cola_ready;
  pthread_mutex_t mutex_cola_ready_prioridad;
  pthread_mutex_t mutex_cola_exec;
  pthread_mutex_t mutex_cola_block;
  pthread_mutex_t mutex_cola_exit;
  pthread_mutex_t mutex_lista_io;
  sem_t sem_multiprogramacion;
  sem_t sem_new;
  sem_t sem_ready;
  sem_t sem_exec;
  sem_t sem_block_return;
  sem_t sem_exit;
  sem_t sem_planif_new;
  sem_t sem_planif_ready;
  sem_t sem_planif_exec;
  sem_t sem_planif_block;

#endif