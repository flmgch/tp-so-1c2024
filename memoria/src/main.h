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
char* path;
t_list *lista_de_instrucciones;
int contador = 0;

// SOCKETS
int socket_escucha, socket_cpu, socket_kernel, socket_interfaz;

#endif