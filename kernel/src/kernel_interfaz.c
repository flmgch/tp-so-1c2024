#include "kernel_interfaz.h"

void atender_modulo_interfaz(void* socket)
{
    int socket_interfaz_conectada = *((int*) socket);
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(socket_interfaz_conectada);
        switch (cod_op)
        {
        case MENSAJE:
            //
            break;
        case PAQUETE:
            //
            break;
        case CREAR_INTERFAZ:
            log_info(kernel_logger, "Recibi un aviso para CREAR una INTERFAZ");
            t_buffer *buffer = recibir_buffer(socket_interfaz_conectada);
            char *nombre_interfaz = extraer_string_de_buffer(buffer);
            char *tipo_interfaz = extraer_string_de_buffer(buffer);
            crear_interfaz(nombre_interfaz, tipo_interfaz, socket_interfaz_conectada);
            break;
        case FIN_INSTRUCCION_INTERFAZ:
            //
            break;
        case -1:
            log_error(kernel_logger, "Se desconecto Interfaz");
            control_key = 0;
            break;
        default:
            log_warning(kernel_logger, "Operacion desconocida de Interfaz");
            close(socket_interfaz_conectada);
            break;
        }
    }
};

void crear_interfaz (char* nombre_interfaz, char* tipo_interfaz, int fd_interfaz) {

    t_interfaz_kernel *interfaz = malloc(sizeof(t_interfaz_kernel));
    t_list *cola_block = list_create();

    interfaz->nombre = nombre_interfaz;
    interfaz->tipo = tipo_interfaz;
    interfaz->socket = fd_interfaz;
    interfaz->cola_block_asignada = cola_block;
    pthread_mutex_init(&interfaz->mutex_asignado, NULL);

    pthread_mutex_lock(&mutex_lista_io);
    list_add(lista_io_conectadas, interfaz);
    log_info(kernel_logger, "Interfaz %s correctamente creada y agregada a la lista de interfaces", interfaz->nombre);
    pthread_mutex_unlock(&mutex_lista_io);

}