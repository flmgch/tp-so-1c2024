#include "kernel_interfaz.h"

void atender_modulo_interfaz()
{
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(socket_interfaz);
        switch (cod_op)
        {
        case MENSAJE:
            //
            break;
        case PAQUETE:
            //
            break;
        case CREAR_INTERFAZ:
            t_buffer *buffer = recibir_buffer(socket_interfaz);
            char *nombre_interfaz = extraer_string_de_buffer(buffer);
            char *tipo_interfaz = extraer_string_de_buffer(buffer);

            pthread_t thread;
            int *socket_conexion_interfaz = malloc(sizeof(int));
            *socket_conexion_interfaz = accept(socket_escucha, NULL, NULL);

            t_manejo_io *args = malloc(sizeof(t_manejo_io));
            args->nombre = nombre_interfaz;
            args->tipo = tipo_interfaz;
            args->socket = (int) socket_conexion_interfaz;

            pthread_create(&thread, NULL, (void *)crear_interfaz, (void *)args);
            pthread_detach(thread);
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
            break;
        }
    }
};

void crear_interfaz (void* args) {

    t_manejo_io* datos_io = (t_manejo_io*) args;

    int fd_interfaz = datos_io->socket;
    char* nombre_interfaz = datos_io->nombre;
    char* tipo_interfaz = datos_io->tipo;

    t_interfaz_kernel *interfaz = malloc(sizeof(t_interfaz_kernel));
    t_list *cola_block = list_create();

    interfaz->nombre = nombre_interfaz;
    interfaz->tipo = tipo_interfaz;
    interfaz->socket = fd_interfaz;
    interfaz->cola_block_asignada = cola_block;
    pthread_mutex_init(&interfaz->mutex_asignado, NULL);

    pthread_mutex_lock(&mutex_lista_io);
    list_add(lista_io_conectadas, interfaz);
    pthread_mutex_unlock(&mutex_lista_io);

    // ?: QUEDARSE ESPERANDO A RECIBIR SOLICITUDES DE KERNEL
}