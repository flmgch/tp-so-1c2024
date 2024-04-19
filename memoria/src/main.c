#include <utils/hello.h>
#include <main.h>

int main(int argc, char *argv[])
{

  mem_logger = iniciar_logger("Memoria.log", LOG_LEVEL_DEBUG);
  log_info(mem_logger, "Soy log");

  mem_config = iniciar_config("./Memoria.config");
  puerto_escucha = config_get_string_value(mem_config, "PUERTO_ESCUCHA");
  log_info(mem_logger, "%s", puerto_escucha);

  int socket_escucha = iniciar_escucha(puerto_escucha, "Memoria", mem_logger);
  
  // SOCKETS ENTRANTES
  int socket_cpu = esperar_conexion(socket_escucha, "CPU", mem_logger);
  int socket_kernel = esperar_conexion(socket_escucha, "Kernel", mem_logger);
  int socket_interfaz = esperar_conexion(socket_escucha, "Interfaz", mem_logger);

  close(socket_escucha);
  close(socket_interfaz);
  close(socket_cpu);
  close(socket_kernel);
  terminar_programa(mem_logger, mem_config);
  return 0;
}
