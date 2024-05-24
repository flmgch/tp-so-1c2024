#include "abrir_archivo.h"

void abrir_archivo(const char *file)
{
    FILE *pseudocodiogo = fopen(file, "r");
    if (pseudocodiogo != NULL)
    {
        char instruccion[35];
        lista_de_instrucciones = list_create();
        while (!feof(pseudocodiogo))
        {
            fgets(instruccion, 35, pseudocodiogo);
            log_info(mem_logger, "%s", instruccion);
            list_add_in_index(lista_de_instrucciones, posicion, instruccion);
            posicion++;
        }
    }
    else
    {
        log_error(mem_logger, "No se pudo abrir el archivo");
        exit(1);
    }

    fclose(pseudocodiogo);
};