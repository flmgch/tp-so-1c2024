#include "planificador.h"

void planificar() {
    planificar_corto_plazo();
}

void planificar_corto_plazo()
{
    pthread_t hilo_corto_plazo;
    pthread_create(&hilo_corto_plazo, NULL, (void *)execute_pcb, NULL);
    pthread_detach(hilo_corto_plazo);
}

void cambiar_estado(t_pcb *pcb, estado_proceso nuevo_estado)
{
    if (pcb->estado != nuevo_estado)
    {
        char *nuevo_estado_string = strdup(estado_to_string(nuevo_estado));
        char *estado_anterior_string = strdup(estado_to_string(pcb->estado));
        pcb->estado = nuevo_estado;
        log_info(kernel_logger, "PID: %d - Estado Anterior: %s - Estado Actual: %s", pcb->pid, estado_anterior_string, nuevo_estado_string);
        free(estado_anterior_string);
        free(nuevo_estado_string);
    }
}

void execute_pcb()
{
    while (1)
    {
        sem_wait(&sem_ready);
        sem_wait(&sem_exec);
        t_pcb *pcb = pcb_segun_algoritmo();
        dispatch_pcb(pcb);
    }
}

t_pcb *pcb_segun_algoritmo()
{
    if (strcasecmp(algoritmo_planificacion, "FIFO") == 0)
    {
        return remover_pcb(cola_ready, &mutex_cola_ready);
    }
    else if (strcasecmp(algoritmo_planificacion, "RR") == 0)
    {
        // return obtener_pcb_RR();
    }
    else if (strcasecmp(algoritmo_planificacion, "VRR") == 0)
    {
        // return obtener_pcb_VRR();
    }
    else
    {
        log_error(kernel_logger, "No se reconocio el algoritmo de planifacion");
        exit(EXIT_FAILURE);
    }
}

void agregar_pcb(t_list *list, t_pcb *pcb, pthread_mutex_t *mutex)
{
    pthread_mutex_lock(mutex);
    list_add(list, pcb);
    pthread_mutex_unlock(mutex);
}

t_pcb *remover_pcb(t_list *list, pthread_mutex_t *mutex) {
    t_pcb *pcb;
    pthread_mutex_lock(mutex);
    pcb = list_remove(list, 0);
    pthread_mutex_unlock(mutex);
    return pcb;
}

void dispatch_pcb(t_pcb *pcb)
{
    cambiar_estado(pcb, EXEC);
    log_info(kernel_logger, "El proceso %d se pone en ejecucion", pcb->pid);
    agregar_pcb(cola_execute, pcb, &mutex_cola_exec);
    enviar_pcb(pcb, socket_conexion_cpu_dispatch);
}

// t_pcb *obtener_pcb_RR();
// t_pcb *obtener_pcb_VRR();