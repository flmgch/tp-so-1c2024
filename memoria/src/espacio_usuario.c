#include "espacio_usuario.h"

void escribir_memoria(t_buffer *buffer)
{
    int pid = extraer_int_de_buffer(buffer);
    int direccion = extraer_int_de_buffer(buffer);
    int tamanio = extraer_int_de_buffer(buffer);
    void *valor = extraer_de_buffer(buffer);

    log_info(mem_logger, "PID: %d - Accion: Escribir - Direccion Fisica: %d - Tamanio: %d", pid, direccion, tamanio);

    pthread_mutex_lock(&mutex_espacio_usuario);
    memset(espacio_usuario, 0, tamanio_memoria);
    memcpy(espacio_usuario + direccion, valor, tamanio);
    pthread_mutex_unlock(&mutex_espacio_usuario);
}

void leer_memoria(t_buffer *buffer, int socket)
{
    int pid = extraer_int_de_buffer(buffer);
    int direccion = extraer_int_de_buffer(buffer);
    int tamanio = extraer_int_de_buffer(buffer);
    void *valor = malloc(tamanio);
    log_info(mem_logger, "PID: %d - Accion: Lectura - Direccion Fisica: %d - Tamanio: %d", pid, direccion, tamanio);

    pthread_mutex_lock(&mutex_espacio_usuario);
    memset(espacio_usuario, 0, tamanio_memoria);
    memcpy(valor, espacio_usuario + direccion, tamanio);
    pthread_mutex_unlock(&mutex_espacio_usuario);

    t_buffer *new_buffer = crear_buffer();

    agregar_a_buffer(new_buffer, valor, tamanio);

    t_paquete *paquete = crear_super_paquete(RESULTADO_LECTURA, new_buffer);

    enviar_paquete(paquete, socket);

    destruir_buffer(new_buffer);
    eliminar_paquete(paquete);
}
