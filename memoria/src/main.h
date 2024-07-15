#ifndef MEMORIA_H_
#define MEMORIA_H_

#include "gestor_memoria.h"
#include "inicializar_memoria.h"
#include "memoria_cpu.h"
#include "memoria_kernel.h"
#include "memoria_interfaz.h"

// VARIABLES GLOBALES
t_log *mem_logger;
t_config *mem_config;
char *puerto_escucha, *path_instrucciones;
int tamanio_memoria, tamanio_pagina, retardo_respuesta;
t_list *lista_de_procesos;

// ESPACIOS MEMORIA
int cantidad_de_marcos_libres;
int cantidad_marcos;
void *espacio_usuario;
void *bitmap_espacio_usuario;
t_bitarray *bitmap;

// SOCKETS
int socket_escucha, socket_cpu, socket_kernel;

// SEMAFOROS Y MUTEX
pthread_mutex_t mutex_lista_procesos;
pthread_mutex_t mutex_bitmap;
pthread_mutex_t mutex_espacio_usuario;

#endif