#include "estados.h"

void pasar_a_ready(t_pcb *pcb)
{
    pthread_mutex_lock(&mutex_cola_ready);
    list_add(cola_ready, pcb);
    pthread_mutex_unlock(&mutex_cola_ready);
    sem_post(&sem_ready);
    log_info(kernel_logger, "Paso a READY el proceso %d", pcb->pid);
}

void ingresarANew(t_pcb *pcb)
{
    pthread_mutex_lock(&mutex_cola_new);
    list_add(cola_new, (void *)pcb);
    pthread_mutex_unlock(&mutex_cola_new);
    log_info(kernel_logger, "Se agrega el proceso:%d a new", pcb->pid);
    sem_post(&sem_new);
}

void Aready()
{
    while (1)
    {
        sem_wait(&sem_ready);

        sem_wait(&sem_multiprogramacion);
        log_info(kernel_logger, "Grado de multiprogramación permite agregar proceso a ready\n");

        t_pcb *pcb = obtenerSiguienteAready();

        // addEstadoReady(pcb);
        log_info(kernel_logger, "Se elimino el proceso %d de New y se agrego a Ready", pcb->pid);

        sem_post(&sem_ready);
    }
}

t_pcb *obtenerSiguienteAready()
{
    pthread_mutex_lock(&mutex_cola_new);
    // TODO EXTRAER PRIMER PCB DE LA COLA_NEW
    // t_pcb *pcb = cola_new[0];
    pthread_mutex_unlock(&mutex_cola_new);
    // return pcb;
}