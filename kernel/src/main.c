#include <stdlib.h>
#include <stdio.h>
#include <utils/hello.h>

int main(int argc, char* argv[]) {
    t_log* logger;
    t_config* config;
    char* puerto_escucha,*ip_memoria,*puerto_memoria,*ip_cpu,*dispatch,*interrupt;
    decir_hola("Kernel");

    logger=iniciar_logger("Kernel.log",LOG_LEVEL_DEBUG);
       log_info(logger,"Soy log");

     config=iniciar_config("./Kernel.config");
       puerto_escucha=config_get_string_value(config,"PUERTO_ESCUCHA");
       ip_memoria=config_get_string_value(config,"IP_MEMORIA");
       puerto_memoria=config_get_string_value(config,"PUERTO_MEMORIA");
       ip_cpu=config_get_string_value(config,"IP_CPU");
       dispatch=config_get_string_value(config,"PUERTO_CPU_DISPATCH");
       interrupt=config_get_string_value(config,"PUERTO_CPU_INTERRUPT");
        log_info(logger,"%s,%s,%s,%s,%s,%s",puerto_escucha,ip_memoria,puerto_memoria,ip_cpu,dispatch,interrupt);

     int socket_Escucha_De_Interfaz=iniciar_escucha(puerto_escucha);
       log_info(logger,"El kernel esta en escucha");
     esperar_conexion(socket_Escucha_De_Interfaz);
       log_info(logger,"El kernel acepto a la interfaz");

       int socket_Conexion_A_CPU_Dispatch=crear_conexion(ip_cpu,dispatch);
         log_info(logger,"El Kernel se conecto con el CPU");

       int socket_Conexion_A_CPU_Interrupt=crear_conexion(ip_cpu,interrupt);
         log_info(logger,"El Kernel se conecto con el CPU");

       int socket_Conexion_A_Memoria=crear_conexion(ip_memoria,puerto_memoria);
        log_info(logger,"El Kernel se conecto con la memoria");
     

     liberar_conexion(socket_Conexion_A_CPU_Interrupt);
     liberar_conexion(socket_Escucha_De_Interfaz);
     liberar_conexion(socket_Conexion_A_Memoria);
     liberar_conexion(socket_Conexion_A_CPU_Dispatch);
     terminar_programa(logger,config);
    return 0;
}

