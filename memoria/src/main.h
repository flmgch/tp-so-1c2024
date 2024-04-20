#ifndef MEMORIA_H_
#define MEMORIA_H_

#include "gestor_memoria.h"
#include "inicializar_memoria.h"

// VARIABLES GLOBALES
t_log *mem_logger;
t_config *mem_config;
char *puerto_escucha, *path_instrucciones;
int tamanio_memoria, tamanio_pagina, retardo_respuesta;

// SOCKETS
int socket_escucha, socket_cpu, socket_kernel, socket_interfaz;

#endif