#include <utils/hello.h>

int main(int argc, char* argv[]) {
    t_log* logger;
    t_config* config;
    char* puerto_escucha;
    decir_hola("Memoria");
   
    logger=iniciar_logger("Memoria.log",LOG_LEVEL_DEBUG);
       log_info(logger,"Soy log");

     config=iniciar_config("./Memoria.config");
       puerto_escucha=config_get_string_value(config,"PUERTO_ESCUCHA");
        log_info(logger,"%s",puerto_escucha);
       

     
     terminar_programa(logger,config);
    return 0;
}

   