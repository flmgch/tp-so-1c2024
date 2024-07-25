#ifndef UTILS_HELLO_H_
#define UTILS_HELLO_H_

#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <readline/readline.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <semaphore.h>
#include <commons/bitarray.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <commons/temporal.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>

// TIPOS DE DATOS
typedef enum {
    SET,
    MOV_IN, 
    MOV_OUT, 
    SUM, 
    SUB,
    JNZ, 
    RESIZE,
    COPY_STRING,
    WAIT,
    SIGNAL, 
    IO_GEN_SLEEP,
    IO_STDIN_READ,
    IO_STDOUT_WRITE,
    IO_FS_CREATE,
    IO_FS_DELETE,
    IO_FS_TRUNCATE,
    IO_FS_WRITE,
    IO_FS_READ,
    EXIT,
    UNKNOWN
} cod_instruccion;


typedef struct 
{
    cod_instruccion codigo_instruccion;
    char param1[20];
    char param2[20];
    char param3[20];
    char param4[20];
    char param5[20];
}t_instruccion;

typedef enum
{
    MENSAJE,
    PAQUETE,
    HANDSHAKE,
    RESPUESTA_HANDSHAKE,
    // KERNEL - CPU
    ENVIO_PCB,
    CAMBIAR_ESTADO,
    ATENDER_WAIT,
    ATENDER_SIGNAL,
    INT_FIN_QUANTUM,
    INT_FINALIZAR_PROCESO,
    // KERNEL - MEMORIA
    CREAR_PROCESO,
    FINALIZAR_PROCESO,
    // KERNEL - IO
    CREAR_INTERFAZ,
    FIN_INSTRUCCION_INTERFAZ,
    // IO - KERNEL
    GENERICA,
    STDIN,
    STDOUT,
    FS_CREATE,
    FS_DELETE,
    FS_TRUNCATE,
    FS_WRITE,
    FS_READ,
    // MEMORIA-KERNEL
    // MEMORIA-CPU-IO
    // MEMORIA-CPU
    RESULTADO_AJUSTE_TAMAÑO,
    RESULTADO_LECTURA,
    RECIBIR_INSTRUCCION,
    RECIBIR_TAMANIO_PAGINAS,
    RESULTADO_ESCRITURA,
    // CPU-MEMORIA
    ACCESO_TABLA_PAGINAS,
    ENVIAR_INSTRUCCIONES,
    ENIVIAR_FRAME,
    AJUSTAR_TAMANIO,
    ACCESO_ESPACIO_USUARIO_LECTURA,
    ACCESO_ESPACIO_USUARIO_ESCRITURA,
    // CPU-KERNEL

    // PROVISIONAL_REVISAR!!!!!!!!!!!!!!!!!!afskdferiovndf
    OP_IO_GEN_SLEEP,
    OP_IO_STDIN_READ,
    OP_IO_STDOUT_WRITE,
    OP_IO_FS_CREATE,
    OP_IO_FS_DELETE,
    OP_IO_FS_TRUNCATE,
    OP_IO_FS_WRITE,
    OP_IO_FS_READ,

} op_code;

typedef struct {
    uint32_t direccion_fisica;
    int tamanio_dato;
} t_direccion_fisica;

typedef struct {
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
    char *recurso;
    int id;
    int instancias;
    t_list *cola_block_asignada;
    pthread_mutex_t mutex_asignado;
} t_recurso;

typedef enum
{
    IO_BLOCK,
    RESOURCE_BLOCK,
    NONE_BLOCK
} motivo_block;

typedef enum
{
    SUCCESS,
    INTERRUPTED_BY_USER,
    INVALID_RESOURCE,
    INVALID_INTERFACE,
    OUT_OF_MEMORY,
    FIN_QUANTUM,
    NONE_EXIT
} motivo_exit;
typedef enum
{
    NEW,
    READY,
    READY_PRIORIDAD,
    EXEC,
    BLOCK,
    FINISH_EXIT,
    FINISH_ERROR,
    UNKNOWN_STATE
} estado_proceso;

typedef struct
{
    u_int32_t pid;
    u_int32_t program_counter;
    estado_proceso estado;
    motivo_block motivo_block;
    motivo_exit motivo_exit;
    t_registros *registros_cpu;
    char** recursos_usados;
    u_int32_t quantum_remanente;
} t_pcb;

typedef struct
{
    char *nombre;
    char *tipo;
    int socket;
    t_list *cola_block_asignada;
    pthread_mutex_t mutex_asignado;
    sem_t interfaz_libre;
} t_interfaz_kernel;

typedef struct
{
    int pid;
    t_interfaz_kernel* interfaz;
    int unidades_de_trabajo; // PARA GENERICA
    t_list* direcciones_fisicas; // PARA STDOUT/STDIN/FS
    u_int32_t tamanio; // PARA STDOUT/STDIN/FS
    char* nombre_archivo; // PARA FS
    u_int32_t puntero; // PARA FS READ/WRITE
} t_manejo_io;

typedef struct
{
    uint32_t pid;
    char *path;
    char **instrucciones;
    int size;
    t_list *filas_tabla_paginas;
} t_proceso;

typedef struct
{
    int frame;
    int bit_precencia;
} tabla_de_paginas;

typedef struct {
    uint32_t pid;        
    int pagina; 
    int marco;
    int ultimo_acceso; //para el lru
} tlb_entrada_t;

typedef struct
{
    u_int32_t pid;
    u_int32_t unidades_trabajo;
    int tiempo_unidad_trabajo;
} io_generica;

typedef struct
{
    u_int32_t pid;
    t_list *lista_direcciones;
    u_int32_t tamanio_total;
} io_stdin;

typedef struct 
{
    u_int32_t pid;
    t_list *lista_direcciones; 
    u_int32_t tamanio_total;
} io_stdout;

typedef struct
{
    void* direccion; 
    int file_descriptor;
    int tamanio;
    t_bitarray *bitarray;
} t_bitmap;

typedef struct
{
    void *direccion;
    int file_descriptor;
} t_bloques_datos;

typedef struct 
{
    u_int32_t pid;
    char *nombre_archivo;
    t_list *lista_direcciones;
    u_int32_t tamanio;
    u_int32_t puntero_archivo;
} io_fs;

typedef struct
{
    int pid;
    int quantum;
} t_manejo_quantum;

// UTILIDADES

void
decir_hola(char *quien);
t_log *iniciar_logger(char *nombreLog, t_log_level level);
t_config *iniciar_config(char *nombreConfig);
void terminar_programa(t_log *logger, t_config *config);
char *estado_to_string(estado_proceso estado);
char *motivo_exit_to_string(motivo_exit motivo);
int remove_string_from_array(char*** array, const char* target);

// FUNCIONES CLIENTE
int crear_conexion(char *ip, char *puerto, char *mensaje, t_log *log);
void handshake_cliente(int socket_conexion, t_log *log);

// FUNCIONES SERVER
int iniciar_escucha(char *PUERTO, char *mensaje, t_log *log);
int esperar_conexion(int socket_conexion, char *mensaje, t_log *log);
void handshake_servidor(int socket_conexion);

// FUNCIONES BUFFER
t_buffer *crear_buffer(void);

void agregar_a_buffer(t_buffer *buffer, void *datos, int tamanio_datos);
void agregar_int_a_buffer(t_buffer *buffer, int valor);
void agregar_uint8_a_buffer(t_buffer *buffer, u_int8_t valor);
void agregar_uint32_a_buffer(t_buffer *buffer, u_int32_t valor);
void agregar_registros_a_buffer(t_buffer *buffer, t_registros* registros);
void agregar_string_a_buffer(t_buffer *buffer, char *string);
void agregar_estado_a_buffer(t_buffer *buffer, estado_proceso estado);
void agregar_lista_a_buffer(t_buffer *buffer, t_list *valor);
void agregar_array_strings_a_buffer(t_buffer *buffer, char**array_strings);
void agregar_motivo_block_a_buffer(t_buffer *buffer, motivo_block motivo);
void agregar_motivo_exit_a_buffer(t_buffer *buffer, motivo_exit motivo);
void agregar_cop_a_buffer(t_buffer *buffer, op_code cop);
void agregar_pcb_a_buffer(t_buffer * buffer, t_pcb * pcb);
void agregar_dirreccion_fisica_a_buffer(t_buffer *buffer, t_direccion_fisica *direccion);
void agregar_lista_direcciones_a_buffer(t_buffer *buffer, t_list *valor);

void *extraer_de_buffer(t_buffer *buffer);
char *extraer_string_de_buffer(t_buffer *buffer);
u_int8_t extraer_uint8_de_buffer(t_buffer *buffer);
u_int32_t extraer_uint32_de_buffer(t_buffer *buffer);
int extraer_int_de_buffer(t_buffer *buffer);
t_list *extraer_lista_de_buffer(t_buffer *buffer);
char** extraer_array_strings_de_buffer(t_buffer* buffer);
t_registros* extraer_registros_de_buffer(t_buffer* buffer);
motivo_exit extraer_motivo_exit_de_buffer(t_buffer *buffer);
estado_proceso extraer_estado_proceso_de_buffer(t_buffer *buffer);
motivo_block extraer_motivo_block_de_buffer(t_buffer *buffer);
t_pcb *extraer_pcb_de_buffer(t_buffer *buffer);
t_direccion_fisica *extraer_dirreccion_fisica_de_buffer(t_buffer *buffer);
t_list *extraer_lista_direcciones_de_buffer(t_buffer *buffer);

// FUNCIONES PAQUETE

t_paquete *crear_paquete(void);
t_paquete *crear_super_paquete(op_code cop, t_buffer *buffer);

void agregar_a_paquete(t_paquete *paquete, void *valor, int tamanio);
void *serializar_paquete(t_paquete *paquete);

// FUNCIONES ENVIAR
void enviar_mensaje(char *mensaje, int socket_cliente);
void enviar_paquete(t_paquete *paquete, int socket_cliente);
void enviar_pcb(t_pcb *pcb, int socket_servidor);

// FUNCIONES PAQUETE

t_paquete *crear_paquete(void);
t_paquete *crear_super_paquete(op_code cop, t_buffer *buffer);

void agregar_a_paquete(t_paquete *paquete, void *valor, int tamanio);
void *serializar_paquete(t_paquete *paquete);

// FUNCIONES ENVIAR
void enviar_mensaje(char *mensaje, int socket_cliente);
void enviar_paquete(t_paquete *paquete, int socket_cliente);
void enviar_pcb(t_pcb *pcb, int socket_servidor);

// FUNCIONES RECIBIR
int recibir_operacion(int);
t_buffer *recibir_buffer(int socket_cliente);

// FUNCIONES ELIMINAR
void destruir_buffer(t_buffer *buffer);
void eliminar_paquete(t_paquete *paquete);

#endif
