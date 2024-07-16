#include "interfaz_kernel.h"

void atender_kernel()
{
    bool control_key = 1;
    while (control_key)
    {
        t_buffer *un_buffer = crear_buffer();
        int cod_op = recibir_operacion(socket_kernel);
        switch (cod_op)
        {
        case GENERICA:
            un_buffer = recibir_buffer(socket_kernel);
            atender_generica(un_buffer);
            destruir_buffer(un_buffer);
            break;
        case STDIN:
            un_buffer = recibir_buffer(socket_kernel);
            atender_stdin(un_buffer);
            destruir_buffer(un_buffer);
            break;
        case STDOUT:
            un_buffer = recibir_buffer(socket_kernel);
            atender_stdout(un_buffer);
            destruir_buffer(un_buffer);
            break;
        case FS_CREATE:
            un_buffer = recibir_buffer(socket_kernel);
            char* nombre_archivo = extraer_string_de_buffer(un_buffer);
            atender_fs_create(nombre_archivo);
            destruir_buffer(un_buffer);
            break;
        case FS_DELETE:
            un_buffer = recibir_buffer(socket_kernel);
            atender_fs_delete(un_buffer);
            destruir_buffer(un_buffer);
            break;
        case FS_TRUNCATE:
            un_buffer = recibir_buffer(socket_kernel);
            atender_fs_truncate(un_buffer);
            destruir_buffer(un_buffer);
            break;
        case FS_WRITE:
            un_buffer = recibir_buffer(socket_kernel);
            atender_fs_write(un_buffer);
            destruir_buffer(un_buffer);
            break;
        case FS_READ:
            un_buffer = recibir_buffer(socket_kernel);
            atender_fs_read(un_buffer);
            destruir_buffer(un_buffer);
            break;
        case -1:
            log_error(io_logger, "Se desconecto Kernel");
            control_key = 0;
            break;
        default:
            log_warning(io_logger, "Operacion desconocida de Kernel");
            close(socket_kernel);
            control_key = 0;
            break;
        }
        destruir_buffer(un_buffer);
    }
};