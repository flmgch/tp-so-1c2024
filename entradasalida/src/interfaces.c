#include "interfaces.h"

void atender_generica(t_buffer *buffer)
{
    interfaz* una_interfaz = malloc(sizeof(interfaz));
    una_interfaz->pid = extraer_int_de_buffer(buffer);
    una_interfaz->tipo_interfaz = extraer_string_de_buffer(buffer);
    una_interfaz->nombre_interfaz = extraer_string_de_buffer(buffer);
    una_interfaz->archivo_configuracion = extraer_string_de_buffer(buffer);
    una_interfaz->unidades_trabajo = extraer_int_de_buffer(buffer);
    una_interfaz->tiempo_unidad_trabajo = extraer_int_de_buffer(buffer);
    una_interfaz->ip_kernel = extraer_string_de_buffer(buffer);
    una_interfaz->puerto_kernel = extraer_string_de_buffer(buffer);
    // Atender IO_GEN_SLEEP
    log_info(io_logger, "Ejecutando IO_GEN_SLEEP a la interfaz %s por %d unidades de trabajo", una_interfaz->nombre_interfaz, una_interfaz->unidades_trabajo);
    usleep(una_interfaz->unidades_trabajo * una_interfaz->tiempo_unidad_trabajo * 1000);
    log_info(io_logger, "Finalizado IO_GEN_SLEEP a la interfaz %s", una_interfaz->nombre_interfaz);

    free(una_interfaz->tipo_interfaz);
    free(una_interfaz->nombre_interfaz);
    free(una_interfaz->archivo_configuracion);
    free(una_interfaz->ip_kernel);
    free(una_interfaz->puerto_kernel);
}

void atender_stdin(t_buffer *buffer)
{
    //
}

void atender_stdout(t_buffer *buffer)
{
    //
}

void atender_dialfs(t_buffer *buffer)
{
    //
}