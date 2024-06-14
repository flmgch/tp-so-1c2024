#include "abrir_archivo.h"

t_list *abrir_archivo(const char *file)
{
    FILE *pseudocodiogo = fopen(file, "r");
    if (pseudocodiogo != NULL)
    {
        char instruccion[35];
        t_list *lista_de_instrucciones = list_create();
        while (!feof(pseudocodiogo))
        {
            fgets(instruccion, 35, pseudocodiogo);
            log_info(mem_logger, "%s", instruccion);
            list_add(lista_de_instrucciones, instruccion);
        }
        fclose(pseudocodiogo);
        return lista_de_instrucciones;
    }
    else
    {
        log_error(mem_logger, "No se pudo abrir el archivo");
        exit(1);
    }
};