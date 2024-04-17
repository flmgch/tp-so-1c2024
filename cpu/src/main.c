#include "main.h"

int main(int argc, char* argv[]) {
    t_log* logger;
    t_config* config;
    char* dispatch,*interrupt,*ip_memoria,*puerto_memoria;
    decir_hola("CPU");
   
    logger=iniciar_logger("CPU.log",LOG_LEVEL_DEBUG);
       log_info(logger,"Soy log");

     config=iniciar_config("./CPU.config");
       ip_memoria=config_get_string_value(config,"IP_MEMORIA");
       puerto_memoria=config_get_string_value(config,"PUERTO_MEMORIA");
       dispatch=config_get_string_value(config,"PUERTO_ESCUCHA_DISPATCH");
       interrupt=config_get_string_value(config,"PUERTO_ESCUCHA_INTERRUPT");
        log_info(logger,"%s,%s,%s,%s",ip_memoria,puerto_memoria,dispatch,interrupt);


     int socket_CPU_A_Memoria=crear_conexion(ip_memoria,puerto_memoria);
         log_info(logger,"El CPU se conecto con la memoria");

     

     liberar_conexion(socket_CPU_A_Memoria);
     terminar_programa(logger,config);
    return 0;
}
