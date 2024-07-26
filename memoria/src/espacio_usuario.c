#include "espacio_usuario.h"

void escribir_memoria(t_buffer *buffer, int socket)
{
    u_int32_t pid = extraer_uint32_de_buffer(buffer);
    void *valor = extraer_de_buffer(buffer);
    t_list *lista_direcciones = extraer_lista_direcciones_de_buffer(buffer);

    int cant_pag_a_escribir = list_size(lista_direcciones);
    int tam_actual = 0;

    char* aux=malloc(20);
    memcpy(aux,valor,20);
    log_info(mem_logger,"Recibo de IO: %s",aux);

    pthread_mutex_lock(&mutex_espacio_usuario);

    for (int i = 0; i < cant_pag_a_escribir; i++)
    {
        t_direccion_fisica *direccion = list_get(lista_direcciones, i);
        usleep(1000 * retardo_respuesta);
        log_info(mem_logger, "PID: %d - Accion: Escribir - Direccion Fisica: %d - Tamanio: %d", pid, direccion->direccion_fisica, direccion->tamanio_dato);
        memcpy(espacio_usuario + direccion->direccion_fisica, valor + tam_actual, direccion->tamanio_dato);
        tam_actual += direccion->tamanio_dato;
    }

    pthread_mutex_unlock(&mutex_espacio_usuario);

    free(valor);

    list_destroy_and_destroy_elements(lista_direcciones, (void *)free);

    t_buffer *new_buffer = crear_buffer();
    agregar_string_a_buffer(new_buffer, "escritura realizada correctamente");
    t_paquete *paquete = crear_super_paquete(RESULTADO_ESCRITURA, new_buffer);
    enviar_paquete(paquete, socket);
    eliminar_paquete(paquete);
}

void leer_memoria(t_buffer *buffer, int socket)
{
    u_int32_t pid = extraer_uint32_de_buffer(buffer);
    t_list *lista_direcciones = extraer_lista_direcciones_de_buffer(buffer);
    u_int32_t tam_total = extraer_int_de_buffer(buffer);
    void *valor = malloc(tam_total);

    int cant_pag_a_leer = list_size(lista_direcciones);
    int tam_actual = 0;

    pthread_mutex_lock(&mutex_espacio_usuario);

    for (int i = 0; i < cant_pag_a_leer; i++)
    {
        t_direccion_fisica *direccion = list_get(lista_direcciones, i);
        usleep(1000 * retardo_respuesta);
        log_info(mem_logger, "PID: %d - Accion: Leer - Direccion Fisica: %d - Tamanio: %d", pid, direccion->direccion_fisica, direccion->tamanio_dato);
        memcpy(valor + tam_actual, espacio_usuario + direccion->direccion_fisica, direccion->tamanio_dato);
        tam_actual += direccion->tamanio_dato;
    }

    pthread_mutex_unlock(&mutex_espacio_usuario);

    t_buffer *new_buffer = crear_buffer();
    agregar_uint32_a_buffer(new_buffer, tam_total);
    agregar_a_buffer(new_buffer, valor, tam_total);
    t_paquete *paquete = crear_super_paquete(RESULTADO_LECTURA, new_buffer);
    enviar_paquete(paquete, socket);
    eliminar_paquete(paquete);

    free(valor);
    list_destroy_and_destroy_elements(lista_direcciones, (void *)free);
}