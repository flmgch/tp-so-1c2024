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
#include "estados.h"

// VARIABLES GLOBALES
  t_log *kernel_logger;
  t_config *kernel_config;
  int identificador_pid = 1;
  int contador_pcbs = 1;
  bool planif_iniciada = false;

// VALORES DEL CONFIG
  char *puerto_escucha, *ip_memoria, *puerto_memoria, *ip_cpu, *dispatch, *interrupt, *algoritmo_planificacion;
  int quantum, grado_multiprogramacion;
  t_list *recursos, *instancias_recursos;

// SOCKETS
  int socket_conexion_cpu_dispatch, socket_conexion_cpu_interrupt, socket_conexion_memoria, socket_escucha, socket_interfaz;

// LISTAS DE PLANIFICACION
  t_list *cola_new, *cola_ready, *cola_execute, *cola_block, *cola_exit;

// TODO LISTAS DE MANEJO DE IO
  t_list *cola_block_io;
  t_list *cola_block_fs;

// SEMAFOROS
  pthread_mutex_t mutex_pid;
  pthread_mutex_t mutex_cola_new;
  pthread_mutex_t mutex_cola_ready;
  pthread_mutex_t mutex_cola_exec;
  pthread_mutex_t mutex_cola_block;
  pthread_mutex_t mutex_cola_block_io;
  pthread_mutex_t mutex_cola_block_fs;
  pthread_mutex_t mutex_cola_exit;
  sem_t sem_multiprogramacion;
  sem_t sem_new;
  sem_t sem_ready;
  sem_t sem_exec;
  sem_t sem_block_return;
  sem_t sem_exit;

#endif