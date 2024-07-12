#include "interfaces.h"

void atender_generica(t_buffer *buffer)
{
    io_generica *una_interfaz = malloc(sizeof(io_generica));
    una_interfaz->pid = extraer_uint32_de_buffer(buffer);
    una_interfaz->unidades_trabajo = extraer_int_de_buffer(buffer);
    una_interfaz->tiempo_unidad_trabajo = extraer_int_de_buffer(buffer);
    una_interfaz->ip_kernel = extraer_string_de_buffer(buffer);
    una_interfaz->puerto_kernel = extraer_string_de_buffer(buffer);

    log_info(io_logger, "PID: %d - Operacion: IO_GEN_SLEEP - %d", una_interfaz->pid, una_interfaz->unidades_trabajo);
    usleep(una_interfaz->unidades_trabajo * una_interfaz->tiempo_unidad_trabajo * 1000);

    // Doy aviso a kernel que termine la operacion
    char *mensaje_confirmacion = "Operacion IO_GEN_SLEEP finalizada";
    enviar_mensaje(mensaje_confirmacion, socket_kernel);

    // free(una_interfaz->unidades_trabajo);
    // free(una_interfaz->tiempo_unidad_trabajo);
    free(una_interfaz->ip_kernel);
    free(una_interfaz->puerto_kernel);
    free(una_interfaz);
}

void atender_stdin(t_buffer *buffer)
{
    io_stdin *una_interfaz = malloc(sizeof(io_stdin));
    una_interfaz->pid = extraer_uint32_de_buffer(buffer);
    una_interfaz->direccion = extraer_uint32_de_buffer(buffer);
    // Kernel puede enviarme varias direcciones con sus respectivos tamaños (una lista) 
    // => MODIFICAR para pedirle a memoria leer cada una por separado
    // Despues debo encargarme de juntar todas las direcciones leidas
    una_interfaz->tamanio = extraer_uint32_de_buffer(buffer);
    una_interfaz->ip_kernel = extraer_string_de_buffer(buffer);
    una_interfaz->puerto_kernel = extraer_string_de_buffer(buffer);
    una_interfaz->ip_memoria = extraer_string_de_buffer(buffer);
    una_interfaz->puerto_memoria = extraer_string_de_buffer(buffer);
    
    log_info(io_logger, "PID: %d - Operacion: IO_STDIN_READ - %d, %d", una_interfaz->pid, una_interfaz->direccion, una_interfaz->tamanio);

    void *texto = malloc(una_interfaz->tamanio);
    printf("> Ingrese un texto: ");
    fgets((char *)texto, una_interfaz->tamanio, stdin);
    
    // Envio la solicitud a memoria para leer el texto de la interfaz
    // => MODIFICAR para pedirle a memoria leer cada direccion por separado
    t_buffer *un_buffer = crear_buffer();
    agregar_uint32_a_buffer(un_buffer, una_interfaz->pid);
    agregar_uint32_a_buffer(un_buffer, una_interfaz->direccion);
    agregar_uint32_a_buffer(un_buffer, una_interfaz->tamanio);
    agregar_a_buffer(un_buffer, texto, una_interfaz->tamanio);
    t_paquete *paquete = crear_super_paquete(ACCESO_ESPACIO_USUARIO_ESCRITURA, un_buffer);
    enviar_paquete(paquete, socket_memoria);

    destruir_buffer(un_buffer);
    eliminar_paquete(paquete);

    free(texto);
    free(una_interfaz->ip_kernel);
    free(una_interfaz->puerto_kernel);
    free(una_interfaz->ip_memoria);
    free(una_interfaz->puerto_memoria);
    free(una_interfaz);
}

void atender_stdout(t_buffer *buffer)
{
    io_stdout *una_interfaz = malloc(sizeof(io_stdout));
    una_interfaz->pid = extraer_uint32_de_buffer(buffer);
    una_interfaz->direccion = extraer_uint32_de_buffer(buffer); // => MODIFICAR ya que Kernel me puede enviar VARIAS direcciones 
    una_interfaz->tamanio = extraer_uint32_de_buffer(buffer);
    una_interfaz->ip_kernel = extraer_string_de_buffer(buffer);
    una_interfaz->puerto_kernel = extraer_string_de_buffer(buffer);
    una_interfaz->ip_memoria = extraer_string_de_buffer(buffer);
    una_interfaz->puerto_memoria = extraer_string_de_buffer(buffer);

    log_info(io_logger, "PID: %d - Operacion: IO_STDOUT_WRITE - %d, %d", una_interfaz->pid, una_interfaz->direccion, una_interfaz->tamanio);

    // Envio la solicitud a memoria para escribir el texto en la interfaz
    // => MODIFICAR: Kernel va a mandarme una lista de direcciones y tengo que enviar la peticion una por una a memoria
    t_buffer *un_buffer = crear_buffer();
    agregar_uint32_a_buffer(un_buffer, una_interfaz->pid);
    agregar_uint32_a_buffer(un_buffer, una_interfaz->direccion);
    agregar_uint32_a_buffer(un_buffer, una_interfaz->tamanio);
    t_paquete *paquete = crear_super_paquete(ACCESO_ESPACIO_USUARIO_ESCRITURA, un_buffer);
    enviar_paquete(paquete, socket_memoria);

    destruir_buffer(un_buffer);
    eliminar_paquete(paquete);

    // Imprimo en la interfaz el resultado que me envio memoria
    // No se si esta bien implementado asi ...? Deberia usar semaforos???????????
    // imprimir_resultado_lectura();

    free(una_interfaz->ip_kernel);
    free(una_interfaz->puerto_kernel);
    free(una_interfaz->ip_memoria);
    free(una_interfaz->puerto_memoria);
    free(una_interfaz);
}

void imprimir_resultado_lectura(t_buffer *buffer)
{
    void *valor = extraer_de_buffer(buffer);
    uint32_t pid = extraer_uint32_de_buffer(buffer);
    // => AVISAR: Memoria me tiene que enviar el PID tambien!!!! 

    // Creo un buffer temporal copiando el valor que me paso memoria para agregarle el caracter nulo
    // Memoria deberia enviarme el tamanio tambien entonces??
    size_t tamanio = buffer->size - sizeof(uint32_t);
    char valor_con_nulo[tamanio + 1];
    memcpy(valor_con_nulo, valor, tamanio);
    valor_con_nulo[tamanio] = '\0';

    log_info(io_logger, "PID: %d - Consola: %s", pid, valor_con_nulo);
}

void atender_dialfs(t_buffer *buffer)
{
    //
}