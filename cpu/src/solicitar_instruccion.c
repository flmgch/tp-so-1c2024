#include "solicitar_instruccion.h"

t_instruccion atender_instrucciones(t_buffer *buffer)
{
    char *inst = extraer_string_de_buffer(buffer);

    t_instruccion instruccion = formular_instruccion(inst);
    
    free(inst);
    return instruccion;
}

cod_instruccion obtener_codigo_instruccion(char *operation)
{
    if (strcmp(operation, "SET") == 0)
        return SET;
    if (strcmp(operation, "MOV_IN") == 0)
        return MOV_IN;
    if (strcmp(operation, "MOV_OUT") == 0)
        return MOV_OUT;
    if (strcmp(operation, "SUM") == 0)
        return SUM;
    if (strcmp(operation, "SUB") == 0)
        return SUB;
    if (strcmp(operation, "JNZ") == 0)
        return JNZ;
    if (strcmp(operation, "RESIZE") == 0)
        return RESIZE;
    if (strcmp(operation, "COPY_STRING") == 0)
        return COPY_STRING;
    if (strcmp(operation, "WAIT") == 0)
        return WAIT;
    if (strcmp(operation, "SIGNAL") == 0)
        return SIGNAL;
    if (strcmp(operation, "IO_GEN_SLEEP") == 0)
        return IO_GEN_SLEEP;
    if (strcmp(operation, "IO_STDIN_READ") == 0)
        return IO_STDIN_READ;
    if (strcmp(operation, "IO_STDOUT_WRITE") == 0)
        return IO_STDOUT_WRITE;
    if (strcmp(operation, "IO_FS_CREATE") == 0)
        return IO_FS_CREATE;
    if (strcmp(operation, "IO_FS_DELETE") == 0)
        return IO_FS_DELETE;
    if (strcmp(operation, "IO_FS_TRUNCATE") == 0)
        return IO_FS_TRUNCATE;
    if (strcmp(operation, "IO_FS_WRITE") == 0)
        return IO_FS_WRITE;
    if (strcmp(operation, "IO_FS_READ") == 0)
        return IO_FS_READ;
    if (strcmp(operation, "EXIT") == 0)
        return EXIT;
    return UNKNOWN;
}

t_instruccion formular_instruccion(char *string)
{
    t_instruccion instruccion;

    memset(&instruccion, 0, sizeof(t_instruccion));
    // armo un array de string con cada palabra de la instruccion en modo string
    char **lineas = string_n_split(string, 5, " ");

    // busco el codigo de instruccion a partir del elemento 0
    instruccion.codigo_instruccion = obtener_codigo_instruccion(lineas[0]);

    int cant_registros = 0;

    for (int i = 1; lineas[i] && i < 6; i++)
    {
        cant_registros++;
    }

    // si existe el parametro, lo copia en la estructura de la instruccion
    (cant_registros != 0) ? strcpy(instruccion.param1, lineas[1]) : strcpy(instruccion.param1, "");
    (cant_registros > 1) ? strcpy(instruccion.param2, lineas[2]) : strcpy(instruccion.param2, "");
    (cant_registros > 2) ? strcpy(instruccion.param3, lineas[3]) : strcpy(instruccion.param3, "");
    if (cant_registros == 5)
    {
        strcpy(instruccion.param4, lineas[4]);
        strcpy(instruccion.param5, lineas[5]);
    }
    else
    {
        strcpy(instruccion.param4, "");
        strcpy(instruccion.param5, "");
    }

    instruccion.param1[sizeof(instruccion.param1) - 1] = '\0';
    instruccion.param2[sizeof(instruccion.param2) - 1] = '\0';
    instruccion.param3[sizeof(instruccion.param3) - 1] = '\0';
    instruccion.param4[sizeof(instruccion.param4) - 1] = '\0';
    instruccion.param5[sizeof(instruccion.param5) - 1] = '\0';

    for (int i = 0; lineas[i] != NULL; i++)
    {
        free(lineas[i]);
    }
    free(lineas);

    return instruccion;
}

void solicitar_instruccion(u_int32_t direccion_instruccion)
{

    // envio direccion instruccion
    t_buffer *un_buffer = crear_buffer();
    agregar_uint32_a_buffer(un_buffer, direccion_instruccion);
    agregar_uint32_a_buffer(un_buffer, pcb->pid);
    t_paquete *paquete = crear_super_paquete(ENVIAR_INSTRUCCIONES, un_buffer);
    enviar_paquete(paquete, socket_memoria);
    eliminar_paquete(paquete);
}