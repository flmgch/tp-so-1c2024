#include "cpu_memoria.h"

void atender_memoria()
{
    bool control_key = 1;
    while (control_key)
    {
        t_buffer *un_buffer = crear_buffer();
        int cod_op = recibir_operacion(socket_memoria);
        switch (cod_op)
        {
        case MENSAJE:
            //
            break;
        case PAQUETE:

            break;
        case RECIBIR_INSTRUCCION:
            un_buffer = recibir_buffer(socket_memoria);
            instruccion = atender_instrucciones(un_buffer);
            sem_post(&sem_instruccion);
            break;
        case RECIBIR_TAMANIO_PAGINAS:
            un_buffer = recibir_buffer(socket_memoria);
            tamanio_pagina = extraer_string_de_buffer(un_buffer);
            log_info(cpu_logger, "Recibido el tamanio de pag.");
            break;
        case RESULTADO_AJUSTE_TAMAÑO:
            un_buffer = recibir_buffer(socket_memoria);
            char *resultado = extraer_string_de_buffer(un_buffer);
            procesar_resultado_resize(resultado);
            sem_post(&sem_resize);
            break;
        case RESULTADO_LECTURA:
            //un_buffer = recibir_buffer(socket_memoria);
            //char* reg_datos=extraer_string_de_buffer(un_buffer);
            //recibir_dato(reg_datos);
            sem_post(&sem_move_in);
            break;
        case -1:
            log_error(cpu_logger, "Se desconecto memoria");
            control_key = 0;
            break;
        default:
            log_warning(cpu_logger, "Operacion desconocida de memoria");
            break;
        }
        destruir_buffer(un_buffer);
    }
};