#include "consultar_memoria.h"

int consultar_memoria(uint32_t pid,int numero_pagina){
    t_buffer *un_buffer = crear_buffer(); 
    agregar_uint32_a_buffer(un_buffer, pid);
    agregar_int_a_buffer(un_buffer,numero_pagina);
    t_paquete *paquete = crear_super_paquete(ACCESO_TABLA_PAGINAS, un_buffer);
    enviar_paquete(paquete, socket_memoria);
    eliminar_paquete(paquete);
    free(un_buffer);
    
    t_buffer *otro_buffer;
    int marco;
    int cod_op = recibir_operacion(socket_memoria);
    if (cod_op == ENIVIAR_FRAME){
        otro_buffer=recibir_buffer(socket_memoria);
        marco = recibir_marco(otro_buffer);
    }
    else{
        log_warning(cpu_logger, "Operacion desconocida: No es un marco");
        exit(EXIT_FAILURE);
    }
    destruir_buffer(otro_buffer);

    return marco;

}

int recibir_marco(t_buffer *un_buffer){
    int marco = extraer_int_de_buffer(un_buffer);
    return marco;
}