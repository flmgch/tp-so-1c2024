#ifndef UTILS_HELLO_H_
#define UTILS_HELLO_H_

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <assert.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/collections/list.h>

// TIPOS DE DATOS
typedef enum
{
    MENSAJE,
    PAQUETE
} op_code;

typedef struct
{
    int size;
    void *stream;
} t_buffer;

typedef struct
{
    op_code codigo_operacion;
    t_buffer *buffer;
} t_paquete;

// FUNCIONES COMPARTIDAS

void decir_hola(char *quien);
t_log *iniciar_logger(char *nombreLog, t_log_level level);
t_config *iniciar_config(char *nombreConfig);
void terminar_programa(t_log *logger, t_config *config);

// FUNCIONES CLIENTE
int crear_conexion(char *ip, char *puerto, char *mensaje, t_log *log);
void enviar_mensaje(char *mensaje, int socket_cliente);
t_paquete *crear_paquete(void);
void agregar_a_paquete(t_paquete *paquete, void *valor, int tamanio);
void enviar_paquete(t_paquete *paquete, int socket_cliente);
void liberar_conexion(int socket_cliente);
void eliminar_paquete(t_paquete *paquete);

// FUNCIONES SERVER
void *recibir_buffer(int *, int);
t_list *recibir_paquete(int);
void recibir_mensaje(int);
int recibir_operacion(int);
int iniciar_escucha(char *PUERTO, char *mensaje, t_log *log);
int esperar_conexion(int socket_conexion, char *mensaje, t_log *log);

#endif
