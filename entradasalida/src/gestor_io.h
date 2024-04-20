#ifndef GESTOR_IO_H_
#define GESTOR_IO_H_

#include <utils/hello.h>

// VARIABLES GLOBALES
extern t_log *io_logger;
extern t_config *io_config;
extern char *ip_kernel, *puerto_kernel, *ip_memoria, *puerto_memoria, *tipo_interfaz, *path_base_dialfs;
extern int tiempo_unidad_trabajo, tamanio_bloque, cantidad_bloque, retraso_compactacion;

// SOCKETS
extern int socket_memoria, socket_kernel;

#endif