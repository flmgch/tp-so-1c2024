#ifndef UTILS_HELLO_H_
#define UTILS_HELLO_H_

#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <semaphore.h>

#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/collections/list.h>

// TIPOS DE DATOS
typedef enum
{
    MENSAJE,
    PAQUETE,
    HANDSHAKE,
    RESPUESTA_HANDSHAKE,
    // MEMORIA
    CREAR_PROCESO,
    TERMINAR_PROCESO,
    ACESO_TABLA_PAGINAS,
    AMPLIACION_PROCESO,
    REDUCCION_PROCESO,
    ACCESO_ESPACIO_USUARIO_CPU,
    ACCESO_ESPACIO_USUARIO_IO,
    ENVIAR_INSTRUCCIONES,
    // CPU
    RECIBIR_PCB,
    RECIBIR_INSTRUCCION
} op_code;

typedef struct
{
    uint8_t ax;   // Registro Numérico de propósito general
    uint8_t bx;   // Registro Numérico de propósito general
    uint8_t cx;   // Registro Numérico de propósito general
    uint8_t dx;   // Registro Numérico de propósito general
    uint32_t eax; // Registro Numérico de propósito general
    uint32_t ebx; // Registro Numérico de propósito general
    uint32_t ecx; // Registro Numérico de propósito general
    uint32_t edx; // Registro Numérico de propósito general
    uint32_t si;  // Dirección lógica de memoria desde donde copiar un string.
    uint32_t di;  // Dirección lógica de memoria a donde copiar un string.
} t_registros;

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

typedef struct
{
    u_int32_t pid;
    u_int32_t program_counter;
    u_int32_t quantum;
    t_registros *registros_cpu;
} t_pcb;

typedef enum{
	IO_BLOCK,
}motivo_block;

typedef enum{
	SUCCESS,
	SEG_FAULT,
	OUT_OF_MEMORY,
	RECURSO_INEXISTENTE,
}motivo_exit;

typedef enum{
	NEW,
	READY,
	EXEC,
	BLOCK,
	FINISH_EXIT,
	FINISH_ERROR,
	UNKNOWN_STATE
} estado_proceso;
typedef struct{
    int pid;
    int program_counter;
    t_list *instrucciones;
    t_list *tabla_de_segmentos;
    estado_proceso estado;
    motivo_exit motivo_exit;
    motivo_block motivo_block;
    t_registros* registros;
}t_contexto_ejecucion;

// FUNCIONES COMPARTIDAS

void decir_hola(char *quien);
t_log *iniciar_logger(char *nombreLog, t_log_level level);
t_config *iniciar_config(char *nombreConfig);
void terminar_programa(t_log *logger, t_config *config);

// FUNCIONES CLIENTE
int crear_conexion(char *ip, char *puerto, char *mensaje, t_log *log);
void enviar_mensaje(char *mensaje, int socket_cliente);

t_buffer *crear_buffer(void);
void destruir_buffer(t_buffer *buffer);
void agregar_a_buffer(t_buffer *buffer, void *datos, int tamanio_datos);
void agregar_int_a_buffer(t_buffer *buffer, int valor);
void agregar_uint32_a_buffer(t_buffer *buffer, u_int32_t valor);
void agregar_string_a_buffer(t_buffer *buffer, char *string);
void agregar_lista_a_buffer(t_buffer *buffer, t_list *valor);

t_paquete *crear_paquete(void);
t_paquete *crear_super_paquete(op_code cop, t_buffer * buffer);

void agregar_a_paquete(t_paquete *paquete, void *valor, int tamanio);
void enviar_paquete(t_paquete *paquete, int socket_cliente);
void * serializar_paquete(t_paquete *paquete);
void eliminar_paquete(t_paquete *paquete);

void handshake_cliente(int socket_conexion, t_log *log);
// void liberar_conexion(int socket_cliente);

// FUNCIONES SERVER

t_buffer *recibir_buffer(int socket_cliente);
void *extraer_de_buffer(t_buffer *buffer);
char *extraer_string_de_buffer(t_buffer *buffer);
u_int32_t extraer_uint32_de_buffer(t_buffer *buffer);
int extraer_int_de_buffer(t_buffer *buffer);
t_list *extraer_lista_de_buffer(t_buffer *buffer);
t_registros *extraer_registros_de_buffer(t_buffer *buffer);

int recibir_operacion(int);

int iniciar_escucha(char *PUERTO, char *mensaje, t_log *log);
int esperar_conexion(int socket_conexion, char *mensaje, t_log *log);

void handshake_servidor(int socket_conexion);

// void *recibir_buffer(int *, int);
// void recibir_mensaje(int);
// t_list *recibir_paquete(int);
#endif
