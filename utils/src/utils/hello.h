#ifndef UTILS_HELLO_H_
#define UTILS_HELLO_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<assert.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/collections/list.h>

void decir_hola(char* quien);
t_log* iniciar_logger(char* nombreLog,t_log_level level);
t_config* iniciar_config(char* nombreConfig);
void terminar_programa(t_log* logger, t_config* config);
int crear_conexion(char *ip, char* puerto);
int iniciar_escucha(char* PUERTO);
int esperar_conexion(int socket_conexion);
void liberar_conexion(int socket_cliente);

#endif
