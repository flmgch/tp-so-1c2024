#include "inicializar_kernel.h"

void inicializar_kernel()
{
    inicializar_logs();
    inicializar_configs();
    inicializar_semaforos();
}

void inicializar_logs()
{
    kernel_logger = iniciar_logger("Kernel.log", LOG_LEVEL_DEBUG);
};

void inicializar_configs()
{
    kernel_config = iniciar_config("./Kernel.config");

    // INICIALIZAR VARIABLES DE CONEXIÓN
    puerto_escucha = config_get_string_value(kernel_config, "PUERTO_ESCUCHA");
    ip_memoria = config_get_string_value(kernel_config, "IP_MEMORIA");
    puerto_memoria = config_get_string_value(kernel_config, "PUERTO_MEMORIA");
    ip_cpu = config_get_string_value(kernel_config, "IP_CPU");
    dispatch = config_get_string_value(kernel_config, "PUERTO_CPU_DISPATCH");
    interrupt = config_get_string_value(kernel_config, "PUERTO_CPU_INTERRUPT");
    // INICIALIZAR VARIABLES DE KERNEL
    algoritmo_planificacion = config_get_string_value(kernel_config, "ALGORITMO_PLANIFICACION");
    quantum = config_get_int_value (kernel_config, "QUANTUM");
    recursos = config_get_array_value(kernel_config, "RECURSOS");
    char **instancias = string_array_new();
    instancias = config_get_array_value(kernel_config, "INSTANCIAS_RECURSOS");
    instancias_recursos = string_to_int_array(instancias);
    grado_multiprogramacion = config_get_int_value(kernel_config, "GRADO_MULTIPROGRAMACION");
};

void inicializar_semaforos()
{
    pthread_mutex_init(&mutex_pid, NULL);
    pthread_mutex_init(&mutex_cola_new, NULL);
    pthread_mutex_init(&mutex_cola_ready, NULL);
    pthread_mutex_init(&mutex_cola_exec, NULL);
    pthread_mutex_init(&mutex_cola_block, NULL);
    pthread_mutex_init(&mutex_cola_block_io, NULL);
    pthread_mutex_init(&mutex_cola_block_fs, NULL);
    pthread_mutex_init(&mutex_cola_exit, NULL);
    sem_init(&sem_multiprogramacion, 0, grado_multiprogramacion);
    sem_init(&sem_new, 0, 0);
    sem_init(&sem_ready, 0, 0);
    sem_init(&sem_exec, 0, 1);
    sem_init(&sem_block_return, 0, 0);
    sem_init(&sem_exit, 0, 0);
}

t_list *inicializar_recursos()
{
    t_list *lista = list_create();
    int cantidad_recursos = string_array_size(recursos);
    for (int i = 0; i < cantidad_recursos; i++)
    {
        char *string = recursos[i];
        t_recurso *recurso = malloc(sizeof(t_recurso));
        recurso->recurso = malloc(sizeof(char) * strlen(string) + 1);
        strcpy(recurso->recurso, string);
        t_list *cola_block = list_create();
        recurso->id = i;
        recurso->instancias = instancias_recursos[i];
        recurso->cola_block_asignada = cola_block;
        pthread_mutex_init(&recurso->mutex_asignado, NULL);
        list_add(lista, recurso);
    }
    return lista;
}

int *string_to_int_array(char **array_de_strings)
{
    int count = string_array_size(array_de_strings);
    int *numbers = malloc(sizeof(int) * count);
    for (int i = 0; i < count; i++)
    {
        int num = atoi(array_de_strings[i]);
        numbers[i] = num;
    }
    return numbers;
}