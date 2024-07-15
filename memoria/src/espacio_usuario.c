#include "espacio_usuario.h"

void escribir_memoria(t_buffer *buffer, int socket)
{

    int pid = extraer_uint32_de_buffer(buffer);
    char *condicion = extraer_string_de_buffer(buffer);
    int tam_total = extraer_int_de_buffer(buffer);
    void *valor = malloc(tam_total);

    if (strcmp(condicion, "Uint8") == 0)
    {
        uint8_t aux_valor = extraer_uint8_de_buffer(buffer);
        // log_info(mem_logger, "VALOR: %u", aux_valor);
        memcpy(valor, &aux_valor, sizeof(uint8_t));
    }
    else if (strcmp(condicion, "Uint32") == 0)
    {
        uint32_t aux_valor = extraer_uint32_de_buffer(buffer);
        // log_info(mem_logger, "VALOR: %u", aux_valor);
        memcpy(valor, &aux_valor, sizeof(uint32_t));
    }
    else
    {
        char *aux_valor = extraer_string_de_buffer(buffer);
        log_info(mem_logger, "VALOR: %s", aux_valor);
        memcpy(valor, aux_valor, strlen(aux_valor) + 1); // +1 para incluir el carácter nulo
    }

    t_list *lista_direcciones = extraer_lista_direcciones_de_buffer(buffer);
    int cant_pag_a_escribir = list_size(lista_direcciones);

    pthread_mutex_lock(&mutex_espacio_usuario);
    // Limpiar el espacio de usuario al principio
    for (int i = 0; i < cant_pag_a_escribir; i++)
    {
        t_direccion_fisica *direccion = list_get(lista_direcciones, i);
        log_info(mem_logger, "DF: %d tam: %d", direccion->direccion_fisica, direccion->tamanio_dato);
        log_info(mem_logger, "PID: %d - Accion: Escribir - Direccion Fisica: %d - Tamanio: %d", pid, direccion->direccion_fisica, direccion->tamanio_dato);

        memset(espacio_usuario, 0, tamanio_memoria);
        memcpy(espacio_usuario + direccion->direccion_fisica, valor, direccion->tamanio_dato);
        free(direccion);
    }
    pthread_mutex_unlock(&mutex_espacio_usuario);

    free(valor);

    t_buffer *new_buffer = crear_buffer();
    agregar_string_a_buffer(new_buffer, "escritura realizada correctamente");

    t_paquete *paquete = crear_super_paquete(RESULTADO_ESCRITURA, new_buffer);
    enviar_paquete(paquete, socket);
    eliminar_paquete(paquete);
}

void leer_memoria(t_buffer *buffer, int socket)
{
    uint8_t valor1 = 0;
    uint32_t valor2 = 0;

    int pid = extraer_uint32_de_buffer(buffer);
    char *condicion = extraer_string_de_buffer(buffer);
    t_list *lista_direcciones = extraer_lista_direcciones_de_buffer(buffer);
    int tam_total = extraer_int_de_buffer(buffer);

    int cant_pag_a_leer = list_size(lista_direcciones);
    void *valor = malloc(tam_total);
    char *valor3 = malloc(tam_total);

    int tam_actual = 0;

    pthread_mutex_lock(&mutex_espacio_usuario);
    for (int i = 0; i < cant_pag_a_leer; i++)
    {
        t_direccion_fisica *direccion = list_get(lista_direcciones, i);
        log_info(mem_logger, "DF: %d tam: %d", direccion->direccion_fisica, direccion->tamanio_dato);
        log_info(mem_logger, "PID: %d - Accion: Leer - Direccion Fisica: %d - Tamanio: %d", pid, direccion->direccion_fisica, direccion->tamanio_dato);

        memcpy(valor + tam_actual, espacio_usuario + direccion->direccion_fisica, direccion->tamanio_dato);

        if (strcmp(condicion, "Uint8") == 0)
        {
            memcpy(&valor1 + tam_actual, espacio_usuario + direccion->direccion_fisica, sizeof(uint8_t));
            // log_info(mem_logger, "VALOR: %u", valor1);
        }
        else if (strcmp(condicion, "Uint32") == 0)
        {
            memcpy(&valor2 + tam_actual, espacio_usuario + direccion->direccion_fisica, sizeof(uint32_t));
            // log_info(mem_logger, "VALOR: %u", valor2);
        }
        else
        {
            memcpy(valor3 + tam_actual, espacio_usuario + direccion->direccion_fisica, direccion->tamanio_dato);
            // log_info(mem_logger, "VALOR: %s", valor3 + tam_actual);
        }

        tam_actual += direccion->tamanio_dato;
        free(direccion);
    }
    pthread_mutex_unlock(&mutex_espacio_usuario);

    t_buffer *new_buffer = crear_buffer();

    if (strcmp(condicion, "Uint8") == 0)
    {
        char dato[4];
        snprintf(dato, sizeof(dato), "%u", valor1);
        // log_info(mem_logger, "VALOR: %s", dato);
        agregar_string_a_buffer(new_buffer, dato);
    }
    else if (strcmp(condicion, "Uint32") == 0)
    {
        char dato[11];
        snprintf(dato, sizeof(dato), "%u", valor2);
        // log_info(mem_logger, "VALOR: %s", dato);
        agregar_string_a_buffer(new_buffer, dato);
    }
    else
    {
        log_info(mem_logger, "VALOR: %s", valor3);
        agregar_string_a_buffer(new_buffer, valor3);
    }

    t_paquete *paquete = crear_super_paquete(RESULTADO_LECTURA, new_buffer);
    enviar_paquete(paquete, socket);

    eliminar_paquete(paquete);
    free(valor);
    free(valor3);
}