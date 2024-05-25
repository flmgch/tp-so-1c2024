#include "abrir_archivo.h"

t_list *abrir_archivo(const char *file)
{
    FILE *pseudocodiogo = fopen(file, "r");
    if (pseudocodiogo != NULL)
    {
        int posicion = 0;
        char instruccion[35];
        t_list *lista_de_instrucciones = list_create();
        while (!feof(pseudocodiogo))
        {
            fgets(instruccion, 35, pseudocodiogo);
            log_info(mem_logger, "%s", instruccion);
            list_add_in_index(lista_de_instrucciones, posicion, instruccion);
            posicion++;
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