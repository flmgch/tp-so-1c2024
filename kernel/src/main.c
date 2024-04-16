#include <stdlib.h>
#include <stdio.h>
#include <utils/hello.h>

int main(int argc, char* argv[]) {
    t_log* logger;
    t_config* config;
    char* puerto_escucha;
    decir_hola("Kernel");

    logger=iniciar_logger("Kernel.log",LOG_LEVEL_DEBUG);
       log_info(logger,"Soy log");

     config=iniciar_config("./Kernel.config");
       puerto_escucha=config_get_string_value(config,"PUERTO_ESCUCHA");
       log_info(logger,"%s",puerto_escucha);

     int socketEscuchaDeInterfaz=iniciar_escucha(puerto_escucha);
       log_info(logger,"El kernel esta en escucha");
     esperar_conexion(socketEscuchaDeInterfaz);
       log_info(logger,"El kernel acepto a la interfaz");

     liberar_conexion(socketEscuchaDeInterfaz);
     terminar_programa(logger,config);
    return 0;
}
