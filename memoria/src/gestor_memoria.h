#ifndef GESTOR_MEMORIA_H_
#define GESTOR_MEMORIA_H_

#include <utils/hello.h>

// VARIABLES GLOBALES
extern t_log *mem_logger;
extern t_config *mem_config;
extern char *puerto_escucha, *path_instrucciones;
extern int tamanio_memoria, tamanio_pagina, retardo_respuesta;
extern t_buffer* un_buffer;
extern char* path;

// SOCKETS
extern int socket_escucha, socket_cpu, socket_kernel, socket_interfaz;

#endif