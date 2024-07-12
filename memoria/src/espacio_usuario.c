#include "espacio_usuario.h"

void escribir_memoria(t_buffer *buffer)
{
    int pid = extraer_int_de_buffer(buffer);
    void *valor = extraer_de_buffer(buffer);
    t_list *lista_direcciones = extraer_lista_de_buffer(buffer);
    int cant_pag_a_escribir=list_size(lista_direcciones);

    pthread_mutex_lock(&mutex_espacio_usuario);
    for (int i = 0; i < cant_pag_a_escribir; i++)
    {
        t_direccion_fisica* direccion=list_get(lista_direcciones,i);
        log_info(mem_logger, "PID: %d - Accion: Escribir - Direccion Fisica: %d - Tamanio: %d", pid, direccion->direccion_fisica, direccion->tamanio_dato);
        memset(espacio_usuario, 0, tamanio_memoria);
        memcpy(espacio_usuario + direccion->direccion_fisica, valor,direccion->tamanio_dato);
        free(direccion);
    }
    pthread_mutex_unlock(&mutex_espacio_usuario);
}

void leer_memoria(t_buffer *buffer, int socket)
{   
    int pid=extraer_int_de_buffer(buffer);
    t_list *lista_direcciones=extraer_lista_de_buffer(buffer);
    char *valor = malloc(20);
    int cant_pag_a_leer=list_size(lista_direcciones);
    int tam_total=0;

    pthread_mutex_lock(&mutex_espacio_usuario);
    for (int i = 0; i < cant_pag_a_leer; i++)
    {
        t_direccion_fisica* direccion=list_get(lista_direcciones,i);
        log_info(mem_logger, "PID: %d - Accion: Escribir - Direccion Fisica: %d - Tamanio: %d", pid, direccion->direccion_fisica, direccion->tamanio_dato);
        memset(espacio_usuario, 0, tamanio_memoria);
        memcpy(valor, espacio_usuario + direccion->direccion_fisica, direccion->tamanio_dato);
        tam_total+=direccion->tamanio_dato;
        free(direccion);
    }
    pthread_mutex_unlock(&mutex_espacio_usuario);

    t_buffer *new_buffer = crear_buffer();

    agregar_string_a_buffer(new_buffer, valor);

    t_paquete *paquete = crear_super_paquete(RESULTADO_LECTURA, new_buffer);

    enviar_paquete(paquete, socket);

    destruir_buffer(new_buffer);
    eliminar_paquete(paquete);
}


