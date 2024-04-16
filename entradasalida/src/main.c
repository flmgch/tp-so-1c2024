#include "main.h"

int main(int argc, char* argv[]) {
    t_log* logger;
    t_config* config;
    char* ip_kernel,*puerto_kernel;
    decir_hola("una Interfaz de Entrada/Salida");

     logger=iniciar_logger("EntradaYSalida.log",LOG_LEVEL_DEBUG);
       log_info(logger,"Soy log");

     config=iniciar_config("./EntradaYSalida.config");
       ip_kernel=config_get_string_value(config,"IP_KERNEL");
       puerto_kernel=config_get_string_value(config,"PUERTO_KERNEL");
       log_info(logger,"%s%s",puerto_kernel,ip_kernel);

     int socketConexionAKernel=crear_conexion(ip_kernel,puerto_kernel);
       log_info(logger,"La interfaz se conecto con el kernel");

     liberar_conexion(socketConexionAKernel);
     terminar_programa(logger,config);
    return 0;
}
