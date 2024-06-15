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

void buscar_frame_libre(int frame)
{

    for (int i = 0; i < cantidad_marcos; i++)
    {
        pthread_mutex_lock(&mutex_bitmap);
        if (bitarray_test_bit(bitmap, i))
        {
            bitarray_set_bit(bitmap, i);
            frame = i;
            return;
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

    destruir_buffer(new_buffer);
    eliminar_paquete(paquete);
}

// FINALIZAR PROCESO

void atender_finalizar_proceso(t_buffer *buffer)
{
    t_proceso *proceso_a_eliminar = malloc(sizeof(t_proceso));
    int *frame;
    int tamanio_lista_procesos;
    uint32_t pid = extraer_uint32_de_buffer(buffer);

    pthread_mutex_lock(&mutex_lista_procesos);
    proceso_a_eliminar = encontrar_proceso(lista_de_procesos, pid);
    tamanio_lista_procesos = list_size(lista_de_procesos);
    pthread_mutex_unlock(&mutex_lista_procesos);

    int tamanio = proceso_a_eliminar->size / tamanio_pagina;
    log_info(mem_logger, "PID:%d - Tamaño: %d", pid, tamanio);

    pthread_mutex_lock(&mutex_bitmap);
    for (int i = 0; i < tamanio_lista_procesos; i++)
    {
        frame = list_get(proceso_a_eliminar->filas_tabla_paginas, i);
        bitarray_clean_bit(bitmap, *frame);
        cantidad_marcos++;
    }
    pthread_mutex_unlock(&mutex_bitmap);

    list_destroy_and_destroy_elements(proceso_a_eliminar->filas_tabla_paginas, eliminar_lista);

    bool auxiliar_no_ser_proceso_x(void *elemento)
    {

        return no_ser_proceso_x(elemento, pid);
    }

    pthread_mutex_lock(&mutex_lista_procesos);
    list_filter(lista_de_procesos, auxiliar_no_ser_proceso_x);
    pthread_mutex_unlock(&mutex_lista_procesos);

    free(proceso_a_eliminar);
}

bool no_ser_proceso_x(void *elemento, uint32_t pid)
{
    t_proceso *proceso = malloc(sizeof(t_proceso));
    proceso = (t_proceso *)elemento;
    return proceso->pid != pid;
}

// CREAR PROCESO

t_proceso *atender_crear_proceso(t_buffer *buffer)
{
    char *path = extraer_string_de_buffer(buffer);
    u_int32_t pid = extraer_uint32_de_buffer(buffer);
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
    t_proceso *proceso_buscado = malloc(sizeof(t_proceso));

    pthread_mutex_lock(&mutex_lista_procesos);
    proceso_buscado = encontrar_proceso(lista_de_procesos, pid);
    pthread_mutex_unlock(&mutex_lista_procesos);

    int *frame = list_get(proceso_buscado->filas_tabla_paginas, numero_pagina);

    log_info(mem_logger, "PID:%d - Pagina: %d - Frame: %d", pid, numero_pagina, *frame);

    t_buffer *new_buffer = crear_buffer();

    agregar_int_a_buffer(new_buffer, *frame);

    t_paquete *paquete = crear_super_paquete(ENIVIAR_FRAME, new_buffer);

    enviar_paquete(paquete, socket_cpu);

    destruir_buffer(new_buffer);
    eliminar_paquete(paquete);
}

// CAMBIAR TAMANIO PROCESO

void atender_ajustar_tamanio(t_buffer *buffer)
{
    int tamanio_nuevo = extraer_int_de_buffer(buffer);
    uint32_t pid = extraer_uint32_de_buffer(buffer);
    t_proceso *proceso_a_modificar = malloc(sizeof(t_proceso));
    pthread_mutex_lock(&mutex_lista_procesos);
    proceso_a_modificar = encontrar_proceso(lista_de_procesos, pid);
    pthread_mutex_unlock(&mutex_lista_procesos);
    int paginas_actuales = proceso_a_modificar->size / tamanio_pagina;
    int paginas_futuras = ceil((double)tamanio_nuevo / tamanio_pagina);

    if (paginas_futuras > paginas_actuales)
    {
        atender_aumentar_tamanio(proceso_a_modificar, tamanio_nuevo, paginas_actuales, paginas_futuras);
        log_info(mem_logger, "PID:%d - Tamanio Actual: %d - Tamanio a Ampliar: %d", pid, proceso_a_modificar->size, proceso_a_modificar->size - tamanio_nuevo);
    }
    else if (paginas_futuras < paginas_actuales)
    {
        atender_reducir_tamanio(proceso_a_modificar, paginas_futuras, paginas_actuales);
        log_info(mem_logger, "PID:%d - Tamanio Actual: %d - Tamanio a Reducir: %d", pid, proceso_a_modificar->size, tamanio_nuevo - proceso_a_modificar->size);
    }

    proceso_a_modificar->size = paginas_futuras * tamanio_pagina;
}

void atender_aumentar_tamanio(t_proceso *proceso, int new_size, int paginas_actuales, int paginas_futuras)
{
    if (cantidad_de_marcos_libres == 0)
    {
        log_error(mem_logger, "No hay frames suficientes para agregar mas paginas");
        enviar_resultado("Out of Memory");
    }
    else if (tamanio_memoria < new_size)
    {
        log_error(mem_logger, "No hay espacio de memoria suficiente para agregar mas paginas");
        enviar_resultado("Out of Memory");
    }
    else
    {

        int paginas_a_agregar = paginas_futuras - paginas_actuales;
        if (paginas_a_agregar <= cantidad_de_marcos_libres)
        {
            int *frame = malloc(sizeof(int));
            for (int i = 0; i < paginas_a_agregar; i++)
            {
                buscar_frame_libre(*frame);
                list_add(proceso->filas_tabla_paginas, frame);
                cantidad_de_marcos_libres--;
            }
            enviar_resultado("Ok");
            free(frame);
        }
        else
        {
            log_error(mem_logger, "No hay frames suficientes para agregar mas paginas");
            enviar_resultado("Out of Memory");
        }
    }
}

void atender_reducir_tamanio(t_proceso *proceso, int paginas_futuras, int paginas_actuales)
{

    int paginas_a_eliminar = paginas_actuales - paginas_futuras;
    while (paginas_a_eliminar > 0)
    {
        int *frame = list_get(proceso->filas_tabla_paginas, (paginas_actuales - 1));
        bitarray_clean_bit(bitmap, *frame);
        cantidad_de_marcos_libres++;
        paginas_a_eliminar--;
        paginas_actuales--;
        free(frame);
    }
    proceso->filas_tabla_paginas = list_take(proceso->filas_tabla_paginas, paginas_futuras);
}

void enviar_resultado(char *resultado)
{
    t_buffer *new_buffer = crear_buffer();

    agregar_string_a_buffer(new_buffer, resultado);

    t_paquete *paquete = crear_super_paquete(RESULTADO_AJUSTE_TAMAÑO, new_buffer);

    enviar_paquete(paquete, socket_cpu);

    free(resultado);
    destruir_buffer(new_buffer);
    eliminar_paquete(paquete);
}

void enviar_tamnio_pagina()
{
    t_buffer *new_buffer = crear_buffer();

    agregar_int_a_buffer(new_buffer, tamanio_pagina);

    t_paquete *paquete = crear_super_paquete(RECIBIR_TAMANIO_PAGINAS, new_buffer);

    enviar_paquete(paquete, socket_cpu);

    destruir_buffer(new_buffer);
    eliminar_paquete(paquete);
}