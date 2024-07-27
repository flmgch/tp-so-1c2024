#include "interfaces.h"

// VARIABLE GLOBAL PARA EL LOG DE STDOUT
u_int32_t pid_stdin;
u_int32_t pid_stdout;

void atender_generica(t_buffer *buffer)
{
    io_generica *una_io = malloc(sizeof(io_generica));
    una_io->pid = extraer_int_de_buffer(buffer);
    una_io->unidades_trabajo = extraer_uint32_de_buffer(buffer);
    una_io->tiempo_unidad_trabajo = tiempo_unidad_trabajo; 

    log_info(io_logger, "PID: %d - Operacion: GEN_SLEEP ", una_io->pid);

    usleep(una_io->unidades_trabajo * una_io->tiempo_unidad_trabajo * 1000);

    // Le aviso a Kernel que ya termine la operacion
    t_buffer *un_buffer = crear_buffer();
    agregar_int_a_buffer(un_buffer, una_io->pid);
    t_paquete *paquete = crear_super_paquete(FIN_INSTRUCCION_INTERFAZ, un_buffer);
    enviar_paquete(paquete, socket_kernel);

    log_info(io_logger, "Operacion GEN_SLEEP finalizada");

    eliminar_paquete(paquete);
    free(una_io);
}

void atender_stdin(t_buffer *buffer)
{
    io_stdin *una_io = malloc(sizeof(io_stdin));
    una_io->pid = extraer_uint32_de_buffer(buffer);
    una_io->lista_direcciones = extraer_lista_direcciones_de_buffer(buffer);
    una_io->tamanio_total = extraer_uint32_de_buffer(buffer);

    sem_wait(&sem_stdin);

    pid_stdout = una_io->pid;

    log_info(io_logger, "PID: %d - Operacion: STDIN_READ", una_io->pid);
        
    char *texto = malloc(una_io->tamanio_total+1);
    void *texto_aux = malloc(una_io->tamanio_total);

    printf("> Ingrese un texto de %d caracteres: ", una_io->tamanio_total);

    fgets(texto, una_io->tamanio_total+1, stdin);
    texto[una_io->tamanio_total] = NULL;

    memcpy(texto_aux, texto, una_io->tamanio_total);

    char *aux = malloc(20);
    memcpy(aux, texto_aux, 20);
    log_info(io_logger, "Envio de IO: %s", aux);

    // Le pido a Memoria que escriba el texto a partir de las direcciones
    t_buffer *buffer_memo = crear_buffer();
    agregar_uint32_a_buffer(buffer_memo, una_io->pid);
    agregar_a_buffer(buffer_memo, texto_aux, una_io->tamanio_total);
    agregar_lista_direcciones_a_buffer(buffer_memo, una_io->lista_direcciones);

    t_paquete *paquete_memo = crear_super_paquete(ACCESO_ESPACIO_USUARIO_ESCRITURA, buffer_memo);
    enviar_paquete(paquete_memo, socket_memoria);

    eliminar_paquete(paquete_memo);
    free(texto);
    free(texto_aux);
    free(una_io);
}

void confirmar_escritura(t_buffer *buffer)
{
    // Le aviso a Kernel que ya termine la operacion
    t_buffer *buffer_kernel = crear_buffer();
    agregar_uint32_a_buffer(buffer_kernel, pid_stdin);
    t_paquete *paquete_kernel = crear_super_paquete(FIN_INSTRUCCION_INTERFAZ, buffer_kernel);
    enviar_paquete(paquete_kernel, socket_kernel);

    log_info(io_logger, "Operacion STDIN_READ finalizada");

    eliminar_paquete(paquete_kernel);
}

void atender_stdout(t_buffer *buffer)
{
    io_stdout *una_io = malloc(sizeof(io_stdout));
    una_io->pid = extraer_uint32_de_buffer(buffer);
    una_io->lista_direcciones = extraer_lista_direcciones_de_buffer(buffer);
    una_io->tamanio_total = extraer_int_de_buffer(buffer);

    sem_wait(&sem_stdout);

    pid_stdout = una_io->pid;

    log_info(io_logger, "PID: %d - Operacion: STDOUT_WRITE", una_io->pid);

    // Le pido a Memoria que lea el texto a partir de las direcciones
    t_buffer *buffer_memo = crear_buffer();
    agregar_uint32_a_buffer(buffer_memo, una_io->pid);
    agregar_lista_direcciones_a_buffer(buffer_memo, una_io->lista_direcciones);
    agregar_uint32_a_buffer(buffer_memo, una_io->tamanio_total);
    t_paquete *paquete_memo = crear_super_paquete(ACCESO_ESPACIO_USUARIO_LECTURA, buffer_memo);
    enviar_paquete(paquete_memo, socket_memoria);

    eliminar_paquete(paquete_memo);
    free(una_io);
}

void imprimir_resultado_lectura(t_buffer *buffer)
{
    u_int32_t tamanio_total = extraer_uint32_de_buffer(buffer);
    void *texto_aux = extraer_de_buffer(buffer);

    char *texto = malloc(tamanio_total+1);
    memcpy(texto, texto_aux, tamanio_total);
    // Le agrego el caracter nulo 
    texto[tamanio_total] = '\0';

    log_info(io_logger, "PID: %d - Consola: %s\n", pid_stdout, texto);

    // Le aviso a Kernel que ya termine la operacion
    t_buffer *buffer_kernel = crear_buffer();
    agregar_uint32_a_buffer(buffer_kernel, pid_stdout);
    t_paquete *paquete_kernel = crear_super_paquete(FIN_INSTRUCCION_INTERFAZ, buffer_kernel);
    enviar_paquete(paquete_kernel, socket_kernel);

    log_info(io_logger, "Operacion STDOUT_WRITE finalizada");

    eliminar_paquete(paquete_kernel);
}
