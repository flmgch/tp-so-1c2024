#include "interfaces.h"

void atender_generica(t_buffer *buffer)
{
    io_generica *una_io = malloc(sizeof(io_generica));
    una_io->pid = extraer_uint32_de_buffer(buffer);
    una_io->unidades_trabajo = extraer_int_de_buffer(buffer);
    una_io->tiempo_unidad_trabajo = tiempo_unidad_trabajo; 

    log_info(io_logger, "PID: %d - Operacion: SLEEP ", una_io->pid);

    usleep(una_io->unidades_trabajo * una_io->tiempo_unidad_trabajo * 1000);

    // Le aviso a Kernel que ya termine la operacion
    t_buffer *un_buffer = crear_buffer();
    agregar_uint32_a_buffer(un_buffer, una_io->pid);
    t_paquete *paquete = crear_super_paquete(GENERICA_FINALIZADA, un_buffer);
    enviar_paquete(paquete, socket_kernel);

    destruir_buffer(un_buffer);
    eliminar_paquete(paquete);
    free(una_io);
}

void atender_stdin(t_buffer *buffer)
{
    io_stdin *una_io = malloc(sizeof(io_stdin));
    una_io->pid = extraer_uint32_de_buffer(buffer);
    una_io->lista_direcciones = extraer_lista_de_buffer(buffer);
    una_io->tamanio_total = extraer_int_de_buffer(buffer);

    log_info(io_logger, "PID: %d - Operacion: READ", una_io->pid);
        
    char *texto = malloc(una_io->tamanio_total);
    while (1)
    {
        printf("> Ingrese un texto de %d caracteres: ", una_io->tamanio_total);
        fgets(texto, una_io->tamanio_total, stdin);

        int longitud_texto = string_length(texto);
        if (longitud_texto < una_io->tamanio_total) 
        {
            printf("> Intente nuevamente: ");
        } 
        else 
        {
            break;
        }
    }

    // Le aviso a memoria para que escriba el texto ingresado
    t_buffer *buffer_memo = crear_buffer();
    agregar_uint32_a_buffer(buffer_memo, una_io->pid);
    agregar_lista_a_buffer(buffer_memo, una_io->lista_direcciones);
    agregar_int_a_buffer(buffer_memo, una_io->tamanio_total);
    agregar_string_a_buffer(buffer_memo, texto);
    t_paquete *paquete_memo = crear_super_paquete(ACCESO_ESPACIO_USUARIO_ESCRITURA, buffer_memo);
    enviar_paquete(paquete_memo, socket_memoria);

    // Le aviso a Kernel que ya termine la operacion
    t_buffer *buffer_kernel = crear_buffer();
    agregar_uint32_a_buffer(buffer_kernel, una_io->pid);
    t_paquete *paquete_kernel = crear_super_paquete(STDIN_FINALIZADA, buffer_kernel);
    enviar_paquete(paquete_kernel, socket_kernel);

    destruir_buffer(buffer_memo);
    destruir_buffer(buffer_kernel);
    eliminar_paquete(paquete_memo);
    eliminar_paquete(paquete_kernel);
    free(texto);
    free(una_io);
}

void atender_stdout(t_buffer *buffer)
{
    io_stdout *una_io = malloc(sizeof(io_stdout));
    una_io->pid = extraer_uint32_de_buffer(buffer);
    una_io->lista_direcciones = extraer_lista_de_buffer(buffer);
    una_io->tamanio_total = extraer_int_de_buffer(buffer);

    log_info(io_logger, "PID: %d - Operacion: WRITE", una_io->pid);

    // le aviso a memoria para que lea el texto pedido
    t_buffer *buffer_memo = crear_buffer();
    agregar_uint32_a_buffer(buffer_memo, una_io->pid);
    agregar_lista_a_buffer(buffer_memo, una_io->lista_direcciones);
    agregar_int_a_buffer(buffer_memo, una_io->tamanio_total);
    t_paquete *paquete_memo = crear_super_paquete(ACCESO_ESPACIO_USUARIO_LECTURA, buffer_memo);
    enviar_paquete(paquete_memo, socket_memoria);

    destruir_buffer(buffer_memo);
    eliminar_paquete(paquete_memo);
    free(una_io);
}

void imprimir_resultado_lectura(t_buffer *buffer)
{
    // AVISAR => Memoria me tiene que devolver el PID que le envie antes !!!!
    uint32_t pid = extraer_uint32_de_buffer(buffer);
    char *valor = extraer_string_de_buffer(buffer);

    // Le agrego el caracter nulo 
    int tamanio = string_length(valor);
    char *valor_con_nulo = malloc(tamanio + 1);
    memcpy(valor_con_nulo, valor, tamanio);
    valor_con_nulo[tamanio] = '\0';

    log_info(io_logger, "PID: %d - Consola: %s", pid, valor_con_nulo);

    // Le aviso a Kernel que ya termine la operacion
    t_buffer *buffer_kernel = crear_buffer();
    agregar_uint32_a_buffer(buffer_kernel, pid);
    t_paquete *paquete_kernel = crear_super_paquete(STDOUT_FINALIZADA, buffer_kernel);
    enviar_paquete(paquete_kernel, socket_kernel);

    destruir_buffer(buffer_kernel);
    eliminar_paquete(paquete_kernel);
}
