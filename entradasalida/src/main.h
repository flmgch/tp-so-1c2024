#ifndef ENTRADAYSALIDA_H_
#define ENTRADAYSALIDA_H_

#include <utils/hello.h>

// VARIABLES GLOBALES
t_log *io_logger;
t_config *io_config;
char *ip_kernel, *puerto_kernel, *ip_memoria, *puerto_memoria, *tipo_interfaz, *path_base_dialfs;
int tiempo_unidad_trabajo, tamanio_bloque, cantidad_bloque, retraso_compactacion;

#endif