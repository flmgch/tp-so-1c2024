#include "planificador.h"

void planificar() {
    planificar_largo_plazo();
    log_info(kernel_logger, "La planificacion de LARGO PLAZO ha sido iniciada");
    planificar_corto_plazo();
    log_info(kernel_logger, "La planificacion de CORTO PLAZO ha sido iniciada");
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
        sem_wait(&sem_exec);
        sem_wait(&sem_planif_exec);
        t_pcb *pcb = pcb_segun_algoritmo();
        dispatch_pcb(pcb);
        sem_post(&sem_planif_exec);
    }
}

t_pcb *pcb_segun_algoritmo()
{
    if (strcasecmp(algoritmo_planificacion, "FIFO") == 0)
    {
        sem_wait(&sem_ready);
        return remover_pcb(cola_ready, &mutex_cola_ready);
    }
    else if (strcasecmp(algoritmo_planificacion, "RR") == 0)
    {
        sem_wait(&sem_ready);
        return obtener_pcb_RR();
    }
    else if (strcasecmp(algoritmo_planificacion, "VRR") == 0)
    {
        return obtener_pcb_VRR();
    }
    else
    {
        log_error(kernel_logger, "No se reconocio el algoritmo de planificacion");
        exit(EXIT_FAILURE);
    }

    return NULL;
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
    pcb->motivo_exit = NONE_EXIT; // ESTO ES NECESARIO POR SI UN PROCESO FUE DESALOJADO POR FIN DE QUANTUM ANTES
    log_info(kernel_logger, "El proceso %d se pone en ejecucion", pcb->pid);
    agregar_pcb(cola_execute, pcb, &mutex_cola_exec);
    enviar_pcb(pcb, socket_conexion_cpu_dispatch);
    if (strcmp(algoritmo_planificacion, "VRR") == 0)
    {
        tiempo_exec = temporal_create();
    }
}

t_pcb *obtener_pcb_RR() {
    pthread_create(&hilo_quantum, NULL, (void*)manejar_quantum, NULL);
    pthread_detach(hilo_quantum);
    return remover_pcb(cola_ready, &mutex_cola_ready);
};

void manejar_quantum(void *quantum_remanente_ptr)
{   
    int* q_rem_ptr = (int *)quantum_remanente_ptr;
    if(q_rem_ptr != NULL && (strcmp(algoritmo_planificacion, "VRR") == 0)) {
        int quantum_remanente = *q_rem_ptr;
        usleep(quantum_remanente * 1000); // ESPERO EL TIEMPO DEL QUANTUM REMANENTE EN MS
        op_code codigo = INT_FIN_QUANTUM;
        send(socket_conexion_cpu_interrupt, &codigo, sizeof(int), 0);
        pthread_cancel(hilo_quantum);
        return;
    } else {
    usleep(quantum * 1000); // ESPERO EL TIEMPO DEL CONFIG EN MS
    op_code codigo = INT_FIN_QUANTUM;
    send(socket_conexion_cpu_interrupt, &codigo, sizeof(int), 0);
    pthread_cancel(hilo_quantum);
    return;
    }
}

// void manejar_quantum_remanente(void *quantum_remanente_ptr)
// {
//     int quantum_remanente = *(int *)quantum_remanente_ptr;
//     while (1)
//     {
//         usleep(quantum_remanente * 1000); // ESPERO EL TIEMPO DEL QUANTUM REMANENTE EN MS
//         t_buffer *buffer_vacio = crear_buffer();
//         agregar_int_a_buffer(buffer_vacio, 1);
//         t_paquete *paquete = crear_super_paquete(INT_FIN_QUANTUM, buffer_vacio);
//         enviar_paquete(paquete, socket_conexion_cpu_interrupt);
//         eliminar_paquete(paquete);
//         pthread_cancel(hilo_quantum_remanente);
//         return;
//     }
// }

t_pcb *obtener_pcb_VRR()
{
    if (!list_is_empty(cola_ready_prioridad))
    {
        sem_wait(&sem_ready_prioridad);
        t_pcb *pcb = remover_pcb(cola_ready_prioridad, &mutex_cola_ready_prioridad);
        int *quantum_remanente_ptr = malloc(sizeof(int));
        *quantum_remanente_ptr = pcb->quantum_remanente;
        pthread_create(&hilo_quantum, NULL, (void *)manejar_quantum, (void *)quantum_remanente_ptr);
        pthread_detach(hilo_quantum);
        return pcb;
    }
    else
    {
        sem_wait(&sem_ready);
        pthread_create(&hilo_quantum, NULL, (void *)manejar_quantum, NULL);
        pthread_detach(hilo_quantum);
        return remover_pcb(cola_ready, &mutex_cola_ready);
    }
};

void planificar_largo_plazo()
{
    pthread_t hilo_new;
    pthread_t hilo_exit;
    pthread_t hilo_block;
    pthread_create(&hilo_exit, NULL, (void *)exit_pcb, NULL);
    pthread_create(&hilo_new, NULL, (void *)new_pcb, NULL);
    pthread_create(&hilo_block, NULL, (void *)block_pcb, NULL);
    pthread_detach(hilo_exit);
    pthread_detach(hilo_new);
    pthread_detach(hilo_block);
}

void exit_pcb()
{
    while (1)
    {
        sem_wait(&sem_exit);
        t_pcb *pcb = remover_pcb(cola_exit, &mutex_cola_exit);
        char *motivo = motivo_exit_to_string(pcb->motivo_exit);
        log_info(kernel_logger, "Finaliza el proceso %d - Motivo: %s", pcb->pid, motivo);

        // ENVIAR PID A MEMORIA
        t_buffer *buffer = crear_buffer();
        agregar_uint32_a_buffer(buffer, pcb->pid);
        t_paquete *paquete = crear_super_paquete(FINALIZAR_PROCESO, buffer);
        enviar_paquete(paquete, socket_conexion_memoria);
        free(pcb);
        free(buffer);
        free(paquete);
        sem_post(&sem_multiprogramacion);
    }
}

void pasar_a_ready(t_pcb *pcb)
{
    sem_wait(&sem_planif_ready);
    pthread_mutex_lock(&mutex_cola_ready);
    cambiar_estado(pcb, READY);
    list_add(cola_ready, pcb);
    pthread_mutex_unlock(&mutex_cola_ready);
    sem_post(&sem_planif_ready);
}

void new_pcb()
{
    while (1)
    {
        sem_wait(&sem_new);
        sem_wait(&sem_planif_new);
        t_pcb *pcb = remover_pcb(cola_new, &mutex_cola_new);
        sem_wait(&sem_multiprogramacion);
        pasar_a_ready(pcb);
        sem_post(&sem_ready);
        sem_post(&sem_planif_new);
    }
}


void block_pcb()
{
    while (1)
    {
        sem_wait(&sem_block_return);
        sem_wait(&sem_planif_block);
        t_pcb *pcb = remover_pcb(cola_block, &mutex_cola_block);
        pcb->motivo_block = NONE_BLOCK;
        if (pcb->quantum_remanente > 0 && (strcmp(algoritmo_planificacion, "VRR") == 0))
        {
            pasar_a_ready_prioridad(pcb);
            sem_post(&sem_ready_prioridad);
        }
        else
        {
            pasar_a_ready(pcb);
            sem_post(&sem_ready);
        }
        sem_post(&sem_planif_block);
    }
}

void pasar_a_ready_prioridad(t_pcb *pcb)
{

    sem_wait(&sem_planif_ready_prioridad);
    pthread_mutex_lock(&mutex_cola_ready_prioridad);
    cambiar_estado(pcb, READY_PRIORIDAD);
    list_add(cola_ready_prioridad, pcb);
    pthread_mutex_unlock(&mutex_cola_ready_prioridad);
    sem_post(&sem_planif_ready_prioridad);
}