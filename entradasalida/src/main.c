#include "main.h"

int main(int argc, char* argv[]) {
    t_log* logger;
    t_config* config;
    char* ip_kernel,*puerto_kernel,*ip_memoria,*puerto_memoria;
    decir_hola("una Interfaz de Entrada/Salida");

     logger=iniciar_logger("EntradaYSalida.log",LOG_LEVEL_DEBUG);
       log_info(logger,"Soy log");

     config=iniciar_config("./EntradaYSalida.config");
       ip_kernel=config_get_string_value(config,"IP_KERNEL");
       puerto_kernel=config_get_string_value(config,"PUERTO_KERNEL");
       ip_memoria=config_get_string_value(config,"IP_MEMORIA");
       puerto_memoria=config_get_string_value(config,"PUERTO_MEMORIA");
       log_info(logger,"%s,%s,%s,%s",puerto_kernel,ip_kernel,ip_memoria,puerto_memoria);

     int socket_Conexion_A_Kernel=crear_conexion(ip_kernel,puerto_kernel);
       log_info(logger,"La interfaz se conecto con el kernel");

     int socket_Conexion_A_Memoria=crear_conexion(ip_kernel,puerto_kernel);
       log_info(logger,"La interfaz se conecto a memoria");

     liberar_conexion(socket_Conexion_A_Kernel);
     liberar_conexion(socket_Conexion_A_Memoria);
     terminar_programa(logger,config);
    return 0;
}
