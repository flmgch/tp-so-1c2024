#include "espacio_usuario.h"

void escribir_memoria(t_buffer *buffer, int socket)
{
    uint32_t pid = extraer_uint32_de_buffer(buffer);
    void *valor = extraer_de_buffer(buffer);
    uint32_t direccion_fisica = extraer_uint32_de_buffer(buffer);
    int tamanio = extraer_int_de_buffer(buffer);

    log_info(mem_logger, "PID: %d - Accion: Escribir - Direccion Fisica: %d - Tamanio: %d", pid, direccion_fisica, tamanio);

    pthread_mutex_lock(&mutex_espacio_usuario);
    memset(espacio_usuario, 0, tamanio_memoria);
    memcpy(espacio_usuario + direccion_fisica, valor, tamanio);
    pthread_mutex_unlock(&mutex_espacio_usuario);

    t_buffer *new_buffer = crear_buffer();

    agregar_string_a_buffer(new_buffer, "escritura realizada corectamente");

    t_paquete *paquete = crear_super_paquete(RESULTADO_ESCRITURA, new_buffer);

    enviar_paquete(paquete, socket);

    eliminar_paquete(paquete);
}

void leer_memoria(t_buffer *buffer, int socket)
{
    uint32_t pid = extraer_uint32_de_buffer(buffer);
    uint32_t direccion_fisica = extraer_uint32_de_buffer(buffer);
    int tamanio = extraer_int_de_buffer(buffer);
    void *valor = malloc(tamanio);

    log_info(mem_logger, "PID: %d - Accion: Escribir - Direccion Fisica: %d - Tamanio: %d", pid, direccion_fisica, tamanio);

    pthread_mutex_lock(&mutex_espacio_usuario);
    memset(espacio_usuario, 0, tamanio_memoria);
    memcpy(valor, espacio_usuario + direccion_fisica, tamanio);
    pthread_mutex_unlock(&mutex_espacio_usuario);

    t_buffer *new_buffer = crear_buffer();

    agregar_a_buffer(new_buffer, valor, tamanio);

    t_paquete *paquete = crear_super_paquete(RESULTADO_LECTURA, new_buffer);

    enviar_paquete(paquete, socket);

    eliminar_paquete(paquete);
}
