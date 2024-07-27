#ifndef ENTRADAYSALIDA_H_
#define ENTRADAYSALIDA_H_

#include "gestor_io.h"
#include "inicializar_io.h"
#include "interfaz_memoria.h"
#include "interfaz_kernel.h"
#include "interfaces.h"
#include "dialfs.h"

// VARIABLES GLOBALES
t_log *io_logger;
t_config *io_config;
char *ip_kernel, *puerto_kernel, *ip_memoria, *puerto_memoria, *tipo_interfaz, *path_base_dialfs;
int tiempo_unidad_trabajo, tamanio_bloque, cantidad_bloque, retraso_compactacion;

// SOCKETS
int socket_memoria, socket_kernel;

// SEMAFOROS
sem_t sem_stdin;
sem_t sem_stdout;

#endif