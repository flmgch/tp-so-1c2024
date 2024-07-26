#include "cpu_memoria.h"

void atender_memoria()
{
    bool control_key = 1;
    while (control_key)
    {
        t_buffer *un_buffer;
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
            destruir_buffer(un_buffer);
            break;
        case RECIBIR_TAMANIO_PAGINAS:
            un_buffer = recibir_buffer(socket_memoria);
            tamanio_pagina = extraer_int_de_buffer(un_buffer);
            log_info(cpu_logger, "Recibido el tamanio de pag.");
            destruir_buffer(un_buffer);
            break;
        case RESULTADO_AJUSTE_TAMAÑO:
            un_buffer = recibir_buffer(socket_memoria);
            char *resultado = extraer_string_de_buffer(un_buffer);
            procesar_resultado_resize(resultado);
            sem_post(&sem_resize);
            free(resultado);
            destruir_buffer(un_buffer);
            break;
        case RESULTADO_LECTURA:
            un_buffer = recibir_buffer(socket_memoria);
            int tam = extraer_int_de_buffer(un_buffer);
            void *aux_resultado = extraer_de_buffer(un_buffer);
            
            if (strcmp(condicion, "Uint8") == 0)
            {
                uint8_t resultado;
                memcpy(&resultado, aux_resultado, sizeof(uint8_t));
                reg_aux=malloc(5);
                sprintf(reg_aux, "%u", resultado);
            }
            else if (strcmp(condicion, "Uint32") == 0)
            {
                uint32_t resultado;
                memcpy(&resultado, aux_resultado, sizeof(uint32_t));
                reg_aux=malloc(5);
                sprintf(reg_aux, "%u", resultado);
            }
            else
            {
                memcpy(reg_aux, aux_resultado, tam);
                reg_aux[tam] = '\0';
            }

            free(aux_resultado);
            sem_post(&sem_resultado_lectura);
            destruir_buffer(un_buffer);
            break;
        case RESULTADO_ESCRITURA:
            un_buffer = recibir_buffer(socket_memoria);
            char *mensaje = extraer_string_de_buffer(un_buffer);
            log_info(cpu_logger, "%s", mensaje);
            destruir_buffer(un_buffer);
            free(mensaje);
            break;
        case ENIVIAR_FRAME:
            un_buffer = recibir_buffer(socket_memoria);
            aux_marco = extraer_int_de_buffer(un_buffer);
            sem_post(&sem_consultar_memoria);
            destruir_buffer(un_buffer);
            break;
        case -1:
            log_error(cpu_logger, "Se desconecto memoria");
            control_key = 0;
            break;
        default:
            log_warning(cpu_logger, "Operacion desconocida de memoria");
            close(socket_memoria);
            control_key = 0;
            break;
        }
    }
};