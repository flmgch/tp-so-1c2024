#include <utils/hello.h>

int main(int argc, char *argv[])
{
  t_log *logger;
  t_config *config;
  char *puerto_escucha;
  decir_hola("Memoria");

  logger = iniciar_logger("Memoria.log", LOG_LEVEL_DEBUG);
  log_info(logger, "Soy log");

  config = iniciar_config("./Memoria.config");
  puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");
  log_info(logger, "%s", puerto_escucha);

  int socket_Escucha = iniciar_escucha(puerto_escucha, "Memoria", logger);
  int socket_CPU = esperar_conexion(socket_Escucha, "CPU", logger);

  int socket_kernel = esperar_conexion(socket_Escucha, "Kernel", logger);

  int socket_interfaz = esperar_conexion(socket_Escucha, "Interfaz", logger);

  close(socket_Escucha);
  close(socket_interfaz);
  close(socket_CPU);
  close(socket_kernel);
  terminar_programa(logger, config);
  return 0;
}
