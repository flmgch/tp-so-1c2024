#include "interfaces.h"

void atender_generica(t_buffer *buffer)
{
    io_generica* una_interfaz = malloc(sizeof(io_generica));
    una_interfaz->pid = extraer_uint32_de_buffer(buffer);
    una_interfaz->unidades_trabajo = extraer_int_de_buffer(buffer);
    una_interfaz->tiempo_unidad_trabajo = extraer_int_de_buffer(buffer);
    una_interfaz->ip_kernel = extraer_string_de_buffer(buffer);
    una_interfaz->puerto_kernel = extraer_string_de_buffer(buffer);

    log_info(io_logger, "PID: %d - Operacion: IO_GEN_SLEEP - %d", una_interfaz->pid, una_interfaz->unidades_trabajo);
    usleep(una_interfaz->unidades_trabajo * una_interfaz->tiempo_unidad_trabajo * 1000);

    free(una_interfaz->pid);
    free(una_interfaz->unidades_trabajo);
    free(una_interfaz->tiempo_unidad_trabajo);
    free(una_interfaz->ip_kernel);
    free(una_interfaz->puerto_kernel);
    free(una_interfaz);
}

void atender_stdin(t_buffer *buffer)
{
    io_stdin* una_interfaz = malloc(sizeof(io_stdin));
    una_interfaz->pid = extraer_uint32_de_buffer(buffer);
    una_interfaz->direccion = extraer_uint32_de_buffer(buffer);
    una_interfaz->tamanio = extraer_uint32_de_buffer(buffer);
    una_interfaz->ip_kernel = extraer_string_de_buffer(buffer);
    una_interfaz->puerto_kernel = extraer_string_de_buffer(buffer);
    una_interfaz->ip_memoria = extraer_string_de_buffer(buffer);
    una_interfaz->puerto_memoria = extraer_string_de_buffer(buffer);
    
    log_info(io_logger, "PID: %d - Operacion: IO_STDIN_READ - %d, %d", una_interfaz->pid, una_interfaz->direccion, una_interfaz->tamanio);

    char texto[una_interfaz->tamanio];
    printf("> Ingrese un texto: ");
    fgets(texto, sizeof(texto), stdin);

    // guardar texto en memoria 

    free(una_interfaz->pid);
    free(una_interfaz->ip_kernel);
    free(una_interfaz->puerto_kernel);
    free(una_interfaz->ip_memoria);
    free(una_interfaz->puerto_memoria);
    free(una_interfaz);
}

void atender_stdout(t_buffer *buffer)
{
    io_stdout* una_interfaz = malloc(sizeof(io_stdout));
    una_interfaz->pid = extraer_uint32_de_buffer(buffer);
    una_interfaz->direccion = extraer_uint32_de_buffer(buffer);
    una_interfaz->tamanio = extraer_uint32_de_buffer(buffer);
    una_interfaz->ip_kernel = extraer_string_de_buffer(buffer);
    una_interfaz->puerto_kernel = extraer_string_de_buffer(buffer);
    una_interfaz->ip_memoria = extraer_string_de_buffer(buffer);
    una_interfaz->puerto_memoria = extraer_string_de_buffer(buffer);

    log_info(io_logger, "PID: %d - Operacion: IO_STDOUT_WRITE - %d, %d", una_interfaz->pid, una_interfaz->direccion, una_interfaz->tamanio);
    // log_info(io_logger, "PID: %d - Consola: %d", una_interfaz->pid, texto);

    // recibir texto de memoria

    free(una_interfaz->pid);
    free(una_interfaz->ip_kernel);
    free(una_interfaz->puerto_kernel);
    free(una_interfaz->ip_memoria);
    free(una_interfaz->puerto_memoria);
    free(una_interfaz);
}

void atender_dialfs(t_buffer *buffer)
{
    //
}