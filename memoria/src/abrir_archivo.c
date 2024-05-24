#include "abrir_archivo.h"

void abrir_archivo(const char *file)
{
    FILE *pseudocodiogo = fopen(file, "rb");
    if (pseudocodiogo != NULL)
    {
        char *instruccion;
        lista_de_instrucciones = list_create();
        while ((fread(&instruccion, (strlen(instruccion) + 1), 1, pseudocodiogo)) == 1)
        {
            list_add_in_index(lista_de_instrucciones, contador, instruccion);
            contador++;
        }
    }
    else
    {
        log_error(mem_logger, "No se pudo abrir el archivo");
        exit(1);
    }

    fclose(pseudocodiogo);
};