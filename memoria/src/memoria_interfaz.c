#include "memoria_interfaz.h"

void atender_interfaz()
{

    bool control_key = 1;
    while (control_key)
    {
        t_buffer *un_buffer = crear_buffer();
        int cod_op = recibir_operacion(socket_interfaz);
        usleep(retardo_respuesta);
        switch (cod_op)
        {
        case MENSAJE:
            //
            break;
        case PAQUETE:
            //
            break;
        case -1:
            log_error(mem_logger, "Se desconecto interfaz");
            control_key = 0;
            break;
        default:
            log_warning(mem_logger, "Operacion desconocida de interfaz");
            break;
        }
        destruir_buffer(un_buffer);
    }
};