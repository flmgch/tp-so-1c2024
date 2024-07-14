#include "interfaces.h"

void atender_generica(t_buffer *buffer)
{
    io_generica *una_io = malloc(sizeof(io_generica));
    una_io->pid = extraer_uint32_de_buffer(buffer);
    una_io->unidades_trabajo = extraer_int_de_buffer(buffer);
    una_io->tiempo_unidad_trabajo = tiempo_unidad_trabajo;
    una_io->ip_kernel = ip_kernel;
    una_io->puerto_kernel = puerto_kernel;

    log_info(io_logger, "PID: %d - Operacion: IO_GEN_SLEEP - %d", una_io->pid, una_io->unidades_trabajo);
    usleep(una_io->unidades_trabajo * una_io->tiempo_unidad_trabajo * 1000);

    // Doy aviso a kernel que termine la operacion
    char *mensaje_confirmacion = "Operacion IO_GEN_SLEEP finalizada";
    enviar_mensaje(mensaje_confirmacion, socket_kernel);

    free(una_io);
}

void atender_stdin(t_buffer *buffer)
{
    io_stdin *una_io = malloc(sizeof(io_stdin));
    una_io->pid = extraer_uint32_de_buffer(buffer);
    una_io->direccion = extraer_uint32_de_buffer(buffer);
    // Kernel puede enviarme varias direcciones con sus respectivos tamaños (una lista) 
    // => MODIFICAR para pedirle a memoria leer cada una por separado
    // Despues debo encargarme de juntar todas las direcciones leidas
    una_io->tamanio = extraer_uint32_de_buffer(buffer);
    una_io->ip_kernel = ip_kernel;
    una_io->puerto_kernel = puerto_kernel;
    una_io->ip_memoria = ip_memoria;
    una_io->puerto_memoria = puerto_memoria;
    
    log_info(io_logger, "PID: %d - Operacion: IO_STDIN_READ - %d, %d", una_io->pid, una_io->direccion, una_io->tamanio);

    void *texto = malloc(una_io->tamanio);
    printf("> Ingrese un texto: ");
    fgets((char *)texto, una_io->tamanio, stdin);
    
    // Envio la solicitud a memoria para leer el texto de la interfaz
    // => MODIFICAR para pedirle a memoria leer cada direccion por separado
    t_buffer *un_buffer = crear_buffer();
    agregar_uint32_a_buffer(un_buffer, una_io->pid);
    agregar_uint32_a_buffer(un_buffer, una_io->direccion);
    agregar_uint32_a_buffer(un_buffer, una_io->tamanio);
    agregar_a_buffer(un_buffer, texto, una_io->tamanio);
    t_paquete *paquete = crear_super_paquete(ACCESO_ESPACIO_USUARIO_ESCRITURA, un_buffer);
    enviar_paquete(paquete, socket_memoria);

    destruir_buffer(un_buffer);
    eliminar_paquete(paquete);

    free(una_io);
}

void atender_stdout(t_buffer *buffer)
{
    io_stdout *una_io = malloc(sizeof(io_stdout));
    una_io->pid = extraer_uint32_de_buffer(buffer);
    una_io->direccion = extraer_uint32_de_buffer(buffer); // => MODIFICAR ya que Kernel me va a enviar una lista de direcciones 
    una_io->tamanio = extraer_uint32_de_buffer(buffer);
    una_io->ip_kernel = ip_kernel;
    una_io->puerto_kernel = puerto_kernel;
    una_io->ip_memoria = ip_memoria;
    una_io->puerto_memoria = puerto_memoria;

    log_info(io_logger, "PID: %d - Operacion: IO_STDOUT_WRITE - %d, %d", una_io->pid, una_io->direccion, una_io->tamanio);

    // Envio la solicitud a memoria para escribir el texto en la interfaz
    // => MODIFICAR: Kernel va a mandarme una lista de direcciones y tengo que enviar la peticion una por una a memoria
    t_buffer *un_buffer = crear_buffer();
    agregar_uint32_a_buffer(un_buffer, una_io->pid);
    agregar_uint32_a_buffer(un_buffer, una_io->direccion);
    agregar_uint32_a_buffer(un_buffer, una_io->tamanio);
    t_paquete *paquete = crear_super_paquete(ACCESO_ESPACIO_USUARIO_LECTURA, un_buffer);
    enviar_paquete(paquete, socket_memoria);

    destruir_buffer(un_buffer);
    eliminar_paquete(paquete);

    free(una_io);
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