#include "consola.h"
#include "planificador.h"

void inicializar_consola()
{
    log_info(kernel_logger, "CONSOLA INICIALIZADA");
    char *leido;
    leido = readline("> Ingrese un comando: ");

    while (strcmp(leido, "\0") != 0)
    {
        if (!validar_instruccion(leido))
        {
            free(leido);
            leido = readline("> Ingrese un comando: ");
            continue; // Salta y continua con el resto de la iteracion
        }

        atender_instruccion(leido);
        free(leido);
        leido = readline("> Ingrese un comando:  ");
    }
    free(leido);
}

bool validar_instruccion(char *leido)
{
    bool resultado_validacion = false;

    // TODO: VALIDAR PATH, PID y VALOR

    char **comando_consola = string_split(leido, " ");

    if (strcmp(comando_consola[0], "EJECUTAR_SCRIPT") == 0)
    {
        resultado_validacion = true;
    }
    else if (strcmp(comando_consola[0], "INICIAR_PROCESO") == 0)
    {
        resultado_validacion = true;
    }
    else if (strcmp(comando_consola[0], "FINALIZAR_PROCESO") == 0)
    {
        resultado_validacion = true;
    }
    else if (strcmp(comando_consola[0], "INICIAR_PLANIFICACION") == 0)
    {
        if (planif_iniciada == false)
        {
            resultado_validacion = true;
        }
        else
        {
            resultado_validacion = false;
            printf("La planificacion ya ha sido iniciada previamente \n");
        }
    }
    else if (strcmp(comando_consola[0], "DETENER_PLANIFICACION") == 0)
    {
        if (planif_iniciada == true)
        {
            resultado_validacion = true;
        }
        else
        {
            resultado_validacion = false;
            printf("La planificacion ya ha sido pausada previamente \n");
        }
    }
    else if (strcmp(comando_consola[0], "MULTIPROGRAMACION") == 0)
    {
        resultado_validacion = true;
    }
    else if (strcmp(comando_consola[0], "PROCESO_ESTADO") == 0)
    {
        resultado_validacion = true;
    }
    else
    {
        log_error(kernel_logger, "ERROR: Comando NO reconocido");
        resultado_validacion = false;
    }
    string_array_destroy(comando_consola);

    return resultado_validacion;
}

void atender_instruccion(char *leido)
{
    char **comando_consola = string_split(leido, " ");
    t_buffer *un_buffer = crear_buffer();

    if (strcmp(comando_consola[0], "EJECUTAR_SCRIPT") == 0)
    {
        agregar_string_a_buffer(un_buffer, comando_consola[1]); // [path]
    }
    else if (strcmp(comando_consola[0], "INICIAR_PROCESO") == 0)
    {
        agregar_string_a_buffer(un_buffer, comando_consola[1]); // [path]
        t_paquete *paquete = crear_super_paquete(CREAR_PROCESO, un_buffer);
        enviar_paquete(paquete, socket_conexion_memoria);
        eliminar_paquete(paquete);
        // f_iniciar_proceso(un_buffer);
    }
    else if (strcmp(comando_consola[0], "FINALIZAR_PROCESO") == 0)
    {
        agregar_int_a_buffer(un_buffer, (intptr_t)comando_consola[1]); // [pid]
    }
    else if (strcmp(comando_consola[0], "DETENER_PLANIFICACION") == 0)
    {
        //  TODO
        planif_iniciada = false;
        printf("La planificacion ha sido pausada. \n");
    }
    else if (strcmp(comando_consola[0], "INICIAR_PLANIFICACION") == 0)
    {
        //  TODO
        planif_iniciada = true;
        planificar();
        printf("La planificacion ha sido iniciada. \n");
    }
    else if (strcmp(comando_consola[0], "MULTIPROGRAMACION") == 0)
    {
        size_t comando_length = strlen(comando_consola[1] + 1);
        char *comando = malloc(comando_length);
        // TODO: ACTUALIZAR VALOR MAXIMO DEL sem_multiprogramacion cuando se actualiza la var global
        strcpy(comando, comando_consola[1]); 
        grado_multiprogramacion = atoi(comando);
        printf("El grado de multiprogramacion ahora es de %d\n", grado_multiprogramacion);

        free(comando);
    }
    else if (strcmp(comando_consola[0], "PROCESO_ESTADO") == 0)
    {
        //   TODO
    }
}

void iniciar_proceso(t_buffer *buffer)
{
}

int asignar_pid()
{
    int valor_pid;

    pthread_mutex_lock(&mutex_pid);
    valor_pid = identificador_pid;
    identificador_pid++;
    pthread_mutex_unlock(&mutex_pid);

    return valor_pid;
}