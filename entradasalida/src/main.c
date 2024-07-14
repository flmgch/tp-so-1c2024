#include <main.h>

int main(int argc, char *argv[])
{
  // Pasar por argumento: ./bin/entradasalida nombreinterfaz nombrearchivo.config
  char *nombre_interfaz = argv[1];
  char *archivo_configuracion = argv[2];

  if (argc < 3)
  {
    fprintf(stderr, "Uso: %s <nombre_interfaz> <archivo_configuracion>\n", argv[0]);
    return EXIT_FAILURE;
  }

  // INICIALIZAR IO
  inicializar_io(nombre_interfaz, archivo_configuracion);

  // INICIALIZAR CONEXIONES
  socket_memoria = crear_conexion(ip_memoria, puerto_memoria, "Memoria", io_logger);
  handshake_cliente(socket_memoria, io_logger);
  socket_kernel = crear_conexion(ip_kernel, puerto_kernel, "Kernel", io_logger);
  handshake_cliente(socket_kernel, io_logger);

  // ENVIAR NOMBRE Y TIPO DE INTERFAZ A KERNEL
  t_buffer *un_buffer = crear_buffer();
  agregar_string_a_buffer(un_buffer, nombre_interfaz);
  agregar_string_a_buffer(un_buffer, tipo_interfaz);
  t_paquete *paquete = crear_super_paquete(CREAR_INTERFAZ, un_buffer);
  enviar_paquete(paquete, socket_kernel);

  // ATENDER KERNEL
  pthread_t hilo_kernel;
  pthread_create(&hilo_kernel, NULL, (void *)atender_kernel, NULL);
  pthread_detach(hilo_kernel);

  // ATENDER MEMORIA
  pthread_t hilo_memoria;
  pthread_create(&hilo_memoria, NULL, (void *)atender_memoria, NULL);
  pthread_join(hilo_memoria, NULL);

  // sleep(1000);
  // close(socket_kernel);
  // close(socket_memoria);
  // terminar_programa(io_logger, io_config);
  return 0;
}