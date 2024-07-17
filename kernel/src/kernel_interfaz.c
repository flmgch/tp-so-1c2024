#include "kernel_interfaz.h"
#include "planificador.h"

char *nombre_interfaz;
char *tipo_interfaz;

void atender_modulo_interfaz(void* socket)
{
    int socket_interfaz_conectada = *((int*) socket);
    t_buffer *buffer = crear_buffer();
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
            buffer = recibir_buffer(socket_interfaz_conectada);
            nombre_interfaz = extraer_string_de_buffer(buffer);
            tipo_interfaz = extraer_string_de_buffer(buffer);
            crear_interfaz(nombre_interfaz, tipo_interfaz, socket_interfaz_conectada);
            break;
        case FIN_INSTRUCCION_INTERFAZ:
            buffer = recibir_buffer(socket_interfaz_conectada);
            int pid = extraer_int_de_buffer(buffer);
            t_pcb* pcb = remover_pcb_lista_io(nombre_interfaz);
            agregar_pcb(cola_block, pcb, &mutex_cola_block);
            sem_post(&sem_block_return);
            log_info(kernel_logger, "PID: %d ha finalizado su uso de la interfaz: %s", pid, nombre_interfaz);
            break;
        case -1:
            log_error(kernel_logger, "Se desconecto Interfaz");
            control_key = 0;
            break;
        default:
            log_warning(kernel_logger, "Operacion desconocida de Interfaz");
            log_warning(kernel_logger, "La interfaz %s se ha desconectado", nombre_interfaz);
            list_remove_by_condition(lista_io_conectadas, es_interfaz_buscada); // Saco la IO de la lista para solo tener io CONECTADAS.
            destruir_buffer(buffer);
            close(socket_interfaz_conectada);
            control_key = 0;
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
    log_info(kernel_logger, "Interfaz %s de tipo %s correctamente creada y agregada a la lista de interfaces", interfaz->nombre, interfaz->tipo);
    pthread_mutex_unlock(&mutex_lista_io);

}

bool es_interfaz_buscada(void *data)
{
    t_interfaz_kernel *interfaz = (t_interfaz_kernel *)data;
    return (strcmp(nombre_interfaz, interfaz->nombre) == 0);
}

t_pcb* remover_pcb_lista_io(char* nombre_io) {

    int longitudLista = list_size(lista_io_conectadas);
    t_interfaz_kernel* elemento;

    for (size_t i = 0; i < longitudLista; i++)
    {
        elemento = (t_interfaz_kernel*) list_get(lista_io_conectadas, i);

        if ((strcmp(elemento->nombre, nombre_io) == 0)) {
            t_pcb* pcb = (t_pcb*) list_remove(elemento->cola_block_asignada, 0);
            return pcb;
        }
    }

    return NULL;
    
}