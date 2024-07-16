#include "procesos.h"

// BUSCAR PROCESO
t_proceso *encontrar_proceso(t_list *lista, uint32_t pid_cpu)
{
    t_proceso *proceso_buscado = malloc(sizeof(t_proceso));

    bool auxiliar_soy_proceso_buscado(void *elemento)
    {
        return soy_proceso_buscado(elemento, pid_cpu);
    }

    proceso_buscado = list_find(lista, auxiliar_soy_proceso_buscado);

    return proceso_buscado;
}

bool soy_proceso_buscado(void *elemento, uint32_t pid_cpu)
{
    t_proceso *proceso_buscado = malloc(sizeof(t_proceso));
    proceso_buscado = (t_proceso *)elemento;
    return proceso_buscado->pid == pid_cpu;
}

int buscar_frame_libre()
{

    for (int i = 0; i < cantidad_marcos; i++)
    {
        pthread_mutex_lock(&mutex_bitmap);
        int test = bitarray_test_bit(bitmap, i);
        // log_info(mem_logger, "%d", test);
        if (test == 0)
        {
            bitarray_set_bit(bitmap, i);
            /*for (int i = 0; i < cantidad_marcos; i++)
            {
                int n = bitarray_test_bit(bitmap, i);
                log_info(mem_logger, "%d ", n);
            }*/
            pthread_mutex_unlock(&mutex_bitmap);
            return i;
        }
        pthread_mutex_unlock(&mutex_bitmap);
    }
}

// OBTENER Y ENVIAR INSTRUCCIONES A CPU

void atender_program_counter(t_buffer *buffer)
{

    uint32_t program_counter = extraer_uint32_de_buffer(buffer);
    uint32_t pid_cpu = extraer_uint32_de_buffer(buffer);
    t_proceso *proceso = malloc(sizeof(t_proceso));

    usleep(1000 * retardo_respuesta);

    pthread_mutex_lock(&mutex_lista_procesos);
    proceso = encontrar_proceso(lista_de_procesos, pid_cpu);
    pthread_mutex_unlock(&mutex_lista_procesos);

    char **instrucciones = proceso->instrucciones;
    char *instruccion = instrucciones[program_counter];
    log_info(mem_logger, "%s", instruccion);
    enviar_instruccion(instruccion);
}

void enviar_instruccion(char *instruccion)
{

    t_buffer *new_buffer = crear_buffer();

    agregar_string_a_buffer(new_buffer, instruccion);

    t_paquete *paquete = crear_super_paquete(RECIBIR_INSTRUCCION, new_buffer);

    enviar_paquete(paquete, socket_cpu);

    eliminar_paquete(paquete);
}

// FINALIZAR PROCESO

void atender_finalizar_proceso(t_buffer *buffer)
{
    t_proceso *proceso_a_eliminar = malloc(sizeof(t_proceso));
    int tam_lista_procesos;
    uint32_t pid = extraer_uint32_de_buffer(buffer);

    usleep(1000 * retardo_respuesta);

    pthread_mutex_lock(&mutex_lista_procesos);
    proceso_a_eliminar = encontrar_proceso(lista_de_procesos, pid);
    tam_lista_procesos = list_size(lista_de_procesos);
    pthread_mutex_unlock(&mutex_lista_procesos);

    int tamanio = proceso_a_eliminar->size;
    log_info(mem_logger, "PID:%d - Tamaño: %d", pid, tamanio);

        pthread_mutex_lock(&mutex_bitmap);
        for (int i = 0; i < tamanio; i++)
        {

            int frame = list_get(proceso_a_eliminar->filas_tabla_paginas, i);
            bitarray_clean_bit(bitmap, frame);
            cantidad_marcos++;
            /*int n = bitarray_test_bit(bitmap, *frame);
            log_info(mem_logger, "%d ", n);*/
        }
        pthread_mutex_unlock(&mutex_bitmap);

        list_destroy(proceso_a_eliminar->filas_tabla_paginas);

        bool auxiliar_no_ser_proceso_x(void *elemento)
        {

            return debe_ser_proceso_x(elemento, pid);
        }

    pthread_mutex_lock(&mutex_lista_procesos);
    list_remove_by_condition(lista_de_procesos, auxiliar_no_ser_proceso_x);
    pthread_mutex_unlock(&mutex_lista_procesos);
    tam_lista_procesos = list_size(lista_de_procesos);
    log_info(mem_logger, "%d", tam_lista_procesos);
    free(proceso_a_eliminar);
}

bool debe_ser_proceso_x(void *elemento, uint32_t pid)
{
    t_proceso *proceso = malloc(sizeof(t_proceso));
    proceso = (t_proceso *)elemento;
    return proceso->pid == pid;
}

// CREAR PROCESO

t_proceso *atender_crear_proceso(t_buffer *buffer)
{
    char *path = extraer_string_de_buffer(buffer);
    u_int32_t pid = extraer_uint32_de_buffer(buffer);

    usleep(1000 * retardo_respuesta);

    t_proceso *proceso = malloc(sizeof(t_proceso));
    proceso->pid = pid;
    proceso->path = path;

    char *auxiliar = malloc(strlen(path_instrucciones) + 1);

    strcpy(auxiliar, path_instrucciones);

    char *archivo = strcat(auxiliar, path);

    proceso->instrucciones = abrir_archivo(archivo);
    proceso->size = 0;
    proceso->filas_tabla_paginas = list_create();

    log_info(mem_logger, "PID:%d - Tamaño: %d", pid, 0);
    free(auxiliar);
    return proceso;
}

char **abrir_archivo(const char *file)
{
    FILE *pseudocodiogo = fopen(file, "r");
    char *instrucciones = NULL;
    if (pseudocodiogo != NULL)
    {

        fseek(pseudocodiogo, 0, SEEK_END);

        int length = ftell(pseudocodiogo);

        fseek(pseudocodiogo, 0, SEEK_SET);

        instrucciones = malloc(length + 1);
        if (instrucciones != NULL)
        {

            fread(instrucciones, 1, length, pseudocodiogo);

            instrucciones[length] = '\0';
        }
        fclose(pseudocodiogo);

        log_info(mem_logger, "%s", instrucciones);
        return string_split(instrucciones, "\n");
    }
    else
    {
        log_error(mem_logger, "No se pudo abrir el archivo");
        exit(1);
    }
}

// ATENDER ACCESO A TABLA DE PAGINAS

void atender_acceso_tabla_paginas(t_buffer *buffer)
{
    int numero_pagina = extraer_int_de_buffer(buffer);
    uint32_t pid = extraer_uint32_de_buffer(buffer);

    usleep(1000 * retardo_respuesta);

    t_proceso *proceso_buscado = malloc(sizeof(t_proceso));

    pthread_mutex_lock(&mutex_lista_procesos);
    proceso_buscado = encontrar_proceso(lista_de_procesos, pid);
    pthread_mutex_unlock(&mutex_lista_procesos);

    /*for (int i = 0; i < 128; i++)
    {
        int n = list_get(proceso_buscado->filas_tabla_paginas, i);
        log_info(mem_logger, "%d", n);
    }*/

        int frame = list_get(proceso_buscado->filas_tabla_paginas, numero_pagina);

    log_info(mem_logger, "PID:%d - Pagina: %d - Frame: %d", pid, numero_pagina, frame);

    t_buffer *new_buffer = crear_buffer();

    agregar_int_a_buffer(new_buffer, frame);

    t_paquete *paquete = crear_super_paquete(ENIVIAR_FRAME, new_buffer);

    enviar_paquete(paquete, socket_cpu);

    eliminar_paquete(paquete);
}

// CAMBIAR TAMANIO PROCESO

void atender_ajustar_tamanio(t_buffer *buffer)
{
    int tamanio_nuevo = extraer_int_de_buffer(buffer);
    uint32_t pid = extraer_uint32_de_buffer(buffer);

    usleep(1000 * retardo_respuesta);

    t_proceso *proceso_a_modificar = malloc(sizeof(t_proceso));

    pthread_mutex_lock(&mutex_lista_procesos);
    proceso_a_modificar = encontrar_proceso(lista_de_procesos, pid);
    pthread_mutex_unlock(&mutex_lista_procesos);
    int paginas_actuales = proceso_a_modificar->size;
    int paginas_futuras = ceil((double)tamanio_nuevo / tamanio_pagina);

    if (paginas_futuras > paginas_actuales)
    {
        log_info(mem_logger, "PID:%d - Tamanio Actual: %d - Tamanio a Ampliar: %d", pid, proceso_a_modificar->size, tamanio_nuevo - tamanio_pagina * proceso_a_modificar->size);
        atender_aumentar_tamanio(proceso_a_modificar, tamanio_nuevo, paginas_actuales, paginas_futuras);
    }
    else if (paginas_futuras < paginas_actuales)
    {
        log_info(mem_logger, "PID:%d - Tamanio Actual: %d - Tamanio a Reducir: %d", pid, proceso_a_modificar->size, tamanio_pagina * proceso_a_modificar->size - tamanio_nuevo);
        atender_reducir_tamanio(proceso_a_modificar, paginas_futuras, paginas_actuales);
    }
}

void atender_aumentar_tamanio(t_proceso *proceso, int new_size, int paginas_actuales, int paginas_futuras)
{
    if (cantidad_de_marcos_libres == 0)
    {
        log_error(mem_logger, "No hay frames suficientes para agregar mas paginas");
        enviar_resultado("Out of Memory");
        return;
    }
    else if (tamanio_memoria < new_size)
    {
        log_error(mem_logger, "No hay espacio de memoria suficiente para agregar mas paginas");
        enviar_resultado("Out of Memory");
        return;
    }
    else
    {
        log_info(mem_logger, "%d", cantidad_de_marcos_libres);
        int paginas_a_agregar = paginas_futuras - paginas_actuales;
        if (paginas_a_agregar <= cantidad_de_marcos_libres)
        {

            for (int i = 0; i < paginas_a_agregar; i++)
            {
                agregar_frames(proceso, i);
            }

            /*for (int i = 0; i < 128; i++)
            {
                int n = list_get(proceso->filas_tabla_paginas, i);
                log_info(mem_logger, "%d", n);
            }*/

            enviar_resultado("Ok");
            proceso->size = paginas_futuras;
            // LO
        }
        else
        {
            log_error(mem_logger, "No hay frames suficientes para agregar mas paginas");
            enviar_resultado("Out of Memory");
            return;
        }
    }
}

void atender_reducir_tamanio(t_proceso *proceso, int paginas_futuras, int paginas_actuales)
{

    int paginas_a_eliminar = paginas_actuales - paginas_futuras;
    while (paginas_a_eliminar > 0)
    {
        quitar_frames(proceso, paginas_actuales);
        paginas_a_eliminar--;
        paginas_actuales--;
    }
    proceso->filas_tabla_paginas = list_take_and_remove(proceso->filas_tabla_paginas, paginas_futuras);
    proceso->size = paginas_futuras;
    enviar_resultado("Ok");
    ///
}

void enviar_resultado(char *resultado)
{
    t_buffer *new_buffer = crear_buffer();

    agregar_string_a_buffer(new_buffer, resultado);

    t_paquete *paquete = crear_super_paquete(RESULTADO_AJUSTE_TAMAÑO, new_buffer);

    enviar_paquete(paquete, socket_cpu);

    eliminar_paquete(paquete);
}

void agregar_frames(t_proceso *proceso, int numero_pagina)
{
    int frame = buscar_frame_libre();
    list_add_in_index(proceso->filas_tabla_paginas, numero_pagina, frame);
    cantidad_de_marcos_libres--;
}

void quitar_frames(t_proceso *proceso, int paginas_actuales)
{
    int frame = list_get(proceso->filas_tabla_paginas, (paginas_actuales - 1));
    pthread_mutex_lock(&mutex_bitmap);
    bitarray_clean_bit(bitmap, frame);
    pthread_mutex_unlock(&mutex_bitmap);
    cantidad_de_marcos_libres++;
}

// TAMAÑO DE PAGINA

void enviar_tamnio_pagina()
{
    t_buffer *new_buffer = crear_buffer();

    agregar_int_a_buffer(new_buffer, tamanio_pagina);

    t_paquete *paquete = crear_super_paquete(RECIBIR_TAMANIO_PAGINAS, new_buffer);

    enviar_paquete(paquete, socket_cpu);

    eliminar_paquete(paquete);
}
