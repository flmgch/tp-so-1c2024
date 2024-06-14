#ifndef GESTOR_MEMORIA_H_
#define GESTOR_MEMORIA_H_

#include <utils/hello.h>

// VARIABLES GLOBALES
extern t_log *mem_logger;
extern t_config *mem_config;
extern char *puerto_escucha, *path_instrucciones;
extern int tamanio_memoria, tamanio_pagina, retardo_respuesta;
extern t_list *lista_de_procesos;

// ESPACIO USUARIO
extern int cantidad_de_marcos_libres;
extern int cantidad_marcos;
extern void *espacio_usuario;
extern void *bitmap_espacio_usuario;
extern t_bitarray *bitmap;

// SOCKETS
extern int socket_escucha, socket_cpu, socket_kernel, socket_interfaz;

// SEMAFOROS Y MUTEX
extern pthread_mutex_t mutex_bitmap;
extern pthread_mutex_t mutex_espacio_usuario;
extern pthread_mutex_t mutex_lista_procesos;

#endif