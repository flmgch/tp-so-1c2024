

void pasar_a_ready(t_pcb *pcb)
{
    pthread_mutex_lock(&mutex_lista_ready);
    list_add(lista_ready, pcb);
    pthread_mutex_unlock(&mutex_lista_ready);
    log_debug(logger, "Paso a READY el proceso %d", pcb->id);
}

void ingresarANew(t_pcb *pcb)
{
    pthread_mutex_lock(&mutex_estado_new);
    queue_push(estado_new, (void *)pcb);
    pthread_mutex_unlock(&mutex_estado_new);

    log_info(logger, "Se agrega el proceso:%d a new", pcb->id);
    sem_post(&sem_hay_pcb_esperando_ready);
}

void Aready()
{
    while (1)
    {
        sem_wait(&sem_hay_pcb_esperando_ready);

        sem_wait(&sem_multiprogramacion);
        log_info(logger, "Grado de multiprogramación permite agregar proceso a ready\n");

        t_pcb *pcb = obtenerSiguienteAready();

        addEstadoReady(pcb);
        log_info(logger, "Se elimino el proceso %d de New y se agrego a Ready", pcb->id);

        sem_post(&sem_ready);
    }
}

t_pcb *obtenerSiguienteAready()
{
    pthread_mutex_lock(&mutex_estado_new);
    t_pcb *pcb = queue_pop(estado_new);
    pthread_mutex_unlock(&mutex_estado_new);
    return pcb;
}