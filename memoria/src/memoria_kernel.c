#include "memoria_kernel.h"

void atender_kernel()
{

    bool control_key = 1;
    while (control_key)
    {
        t_buffer *un_buffer = crear_buffer();
        int cod_op = recibir_operacion(socket_kernel);
        usleep(retardo_respuesta);
        switch (cod_op)
        {
        case MENSAJE:
            //
            break;
        case PAQUETE:
            //
            break;
        case CREAR_PROCESO:
           t_buffer *buffer_crear_proceso = crear_buffer();
           buffer_crear_proceso  = recibir_buffer(socket_kernel);
           t_proceso *proceso = atender_crear_proceso(buffer_crear_proceso);
           pthread_mutex_lock(&mutex_lista_procesos);
           list_add(lista_de_procesos, proceso);
           free(proceso);
           pthread_mutex_unlock(&mutex_lista_procesos);
           destruir_buffer(buffer_crear_proceso);
           break;
        case FINALIZAR_PROCESO:
            un_buffer = recibir_buffer(socket_kernel);
            atender_finalizar_proceso(un_buffer);
            break;
        case -1:
            log_error(mem_logger, "Se desconecto kernel");
            control_key = 0;
            break;
        default:
            log_warning(mem_logger, "Operacion desconocida de kernel");
            break;
        }
        destruir_buffer(un_buffer);
    }
};