#include <utils/hello.h>

void decir_hola(char* quien) {
    printf("Hola desde %s!!\n", quien);
}

t_log* iniciar_logger(char* nombreLog,t_log_level level){
	t_log* nuevo_logger;
       nuevo_logger=log_create(nombreLog,"Tp_0",1,level);
        if (nuevo_logger==NULL)
		{
			 printf("No se creo el logger\n");
			exit (1);
		}
	return nuevo_logger;
}

t_config* iniciar_config(char* nombreConfig){
	t_config* nuevo_config;
    nuevo_config=config_create(nombreConfig);
	if (nuevo_config==NULL){
		 printf("No se pudo crear  el config\n");
		exit(2);
	}
	return nuevo_config;
}

void terminar_programa(t_log* logger, t_config* config){

    if(logger!=NULL){
      log_destroy(logger);
    }

    if(config!=NULL){
      config_destroy(config);  
    }
}

int crear_conexion(char *ip, char* puerto){
	struct addrinfo hints;
	struct addrinfo *server_info;
    int errores=0;
    
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;


    errores = getaddrinfo(ip, puerto, &hints, &server_info);
      
      if (errores < 0) {
      fprintf(stderr, "Error en getaddrinfo: %s\n", gai_strerror(errores));
      exit(1);
      }

	int socketDeConexion = socket(server_info->ai_family,server_info->ai_socktype,server_info->ai_protocol);
           
      if (socketDeConexion == -1) {
        perror("Error en socket");
        exit(1);
       }

	errores=connect(socketDeConexion, server_info->ai_addr, server_info->ai_addrlen);

      if (errores < 0) {
      fprintf(stderr, "Error en Conecion: %s\n", gai_strerror(errores));
      exit(1);
      }

	freeaddrinfo(server_info);

	return socketDeConexion;
}


int iniciar_escucha(char* PUERTO){

	struct addrinfo hints, *servinfo;
    int errores=0;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	errores=getaddrinfo(NULL, PUERTO, &hints, &servinfo);

    if (errores < 0) {
      fprintf(stderr, "Error en getaddrinfo: %s\n", gai_strerror(errores));
      exit(1);
      }  

	int socketEscucha = socket(servinfo->ai_family,servinfo->ai_socktype,servinfo->ai_protocol);

      if (socketEscucha == -1) {
        perror("Error en socket");
        exit(1);
       }

	errores=bind(socketEscucha, servinfo->ai_addr, servinfo->ai_addrlen);

      if (errores < 0) {
      fprintf(stderr, "Error en Bind: %s\n", gai_strerror(errores));
      exit(1);
      }

    errores=listen(socketEscucha, SOMAXCONN);

      if (errores < 0) {
      fprintf(stderr, "Error en Listen: %s\n", gai_strerror(errores));
      exit(1);
      }

	freeaddrinfo(servinfo);

	return socketEscucha;
}

int esperar_conexion(int socket_conexion){   
	int socket_cliente = accept(socket_conexion, NULL, NULL);

	if (socket_cliente < 0) {
      fprintf(stderr, "Error en Aceptacion: %s\n", strerror(socket_cliente));
      exit(1);
      }

	return socket_conexion;
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
