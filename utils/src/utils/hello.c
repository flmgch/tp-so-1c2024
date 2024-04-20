#include <utils/hello.h>

void decir_hola(char *quien)
{
  printf("Hola desde %s!!\n", quien);
}

t_log *iniciar_logger(char *nombreLog, t_log_level level)
{
  t_log *nuevo_logger;
  nuevo_logger = log_create(nombreLog, "LOG", 1, level);
  if (nuevo_logger == NULL)
  {
    perror("No se creo el logger\n");
    exit(1);
  }
  return nuevo_logger;
}

t_config *iniciar_config(char *nombreConfig)
{
  t_config *nuevo_config;
  nuevo_config = config_create(nombreConfig);
  if (nuevo_config == NULL)
  {
    perror("No se pudo crear  el config\n");
    exit(2);
  }
  return nuevo_config;
}

void terminar_programa(t_log *logger, t_config *config)
{

  if (logger != NULL)
  {
    log_destroy(logger);
  }

  if (config != NULL)
  {
    config_destroy(config);
  }
}

/* ------------------------------------------ FUNCIONES DEL CLIENTE -------------------------------------- */

int crear_conexion(char *ip, char *puerto, char *mensaje, t_log *log)
{
  struct addrinfo hints;
  struct addrinfo *server_info;
  int errores = 0;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  errores = getaddrinfo(ip, puerto, &hints, &server_info);

  if (errores < 0)
  {
    fprintf(stderr, "Error en getaddrinfo: %s\n", gai_strerror(errores));
    exit(1);
  }

  int socketDeConexion = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

  if (socketDeConexion == -1)
  {
    perror("Error en socket");
    exit(1);
  }

  errores = connect(socketDeConexion, server_info->ai_addr, server_info->ai_addrlen);

  if (errores < 0)
  {
    fprintf(stderr, "Error en Conexion: %s\n", gai_strerror(errores));
    exit(1);
  }

  log_info(log, "Se conecto con: %s", mensaje);

  freeaddrinfo(server_info);

  return socketDeConexion;
}


void *serializar_paquete(t_paquete *paquete, int bytes)
{
  void *magic = malloc(bytes);
  int desplazamiento = 0;

  memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
  desplazamiento += sizeof(int);
  memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
  desplazamiento += sizeof(int);
  memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
  desplazamiento += paquete->buffer->size;

  return magic;
}

void enviar_mensaje(char *mensaje, int socket_cliente)
{
  t_paquete *paquete = malloc(sizeof(t_paquete));

  paquete->codigo_operacion = MENSAJE;
  paquete->buffer = malloc(sizeof(t_buffer));
  paquete->buffer->size = strlen(mensaje) + 1;
  paquete->buffer->stream = malloc(paquete->buffer->size);
  memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

  int bytes = paquete->buffer->size + 2 * sizeof(int);

  void *a_enviar = serializar_paquete(paquete, bytes);

  send(socket_cliente, a_enviar, bytes, 0);

  free(a_enviar);
  eliminar_paquete(paquete);
}

void crear_buffer(t_paquete *paquete)
{
  paquete->buffer = malloc(sizeof(t_buffer));
  paquete->buffer->size = 0;
  paquete->buffer->stream = NULL;
}

t_paquete *crear_paquete(void)
{
  t_paquete *paquete = malloc(sizeof(t_paquete));
  paquete->codigo_operacion = PAQUETE;
  crear_buffer(paquete);
  return paquete;
}

void agregar_a_paquete(t_paquete *paquete, void *valor, int tamanio)
{
  paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

  memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
  memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

  paquete->buffer->size += tamanio + sizeof(int);
}

void enviar_paquete(t_paquete *paquete, int socket_cliente)
{
  int bytes = paquete->buffer->size + 2 * sizeof(int);
  void *a_enviar = serializar_paquete(paquete, bytes);

  send(socket_cliente, a_enviar, bytes, 0);

  free(a_enviar);
}

void eliminar_paquete(t_paquete *paquete)
{
  free(paquete->buffer->stream);
  free(paquete->buffer);
  free(paquete);
}

/* ------------------------------------- FUNCIONES DEL SERVER ------------------------------------------------ */

int iniciar_escucha(char *PUERTO, char *mensaje, t_log *log)
{

  struct addrinfo hints, *servinfo;
  int errores = 0;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  errores = getaddrinfo(NULL, PUERTO, &hints, &servinfo);

  if (errores < 0)
  {
    fprintf(stderr, "Error en getaddrinfo: %s\n", gai_strerror(errores));
    exit(1);
  }

  int socketEscucha = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

  if (socketEscucha == -1)
  {
    perror("Error en socket");
    exit(1);
  }

  errores = bind(socketEscucha, servinfo->ai_addr, servinfo->ai_addrlen);

  if (errores < 0)
  {
    fprintf(stderr, "Error en Bind: %s\n", gai_strerror(errores));
    exit(1);
  }

  errores = listen(socketEscucha, SOMAXCONN);

  if (errores < 0)
  {
    fprintf(stderr, "Error en Listen: %s\n", gai_strerror(errores));
    exit(1);
  }

  log_info(log, "Se puso a escuchar: %s", mensaje);

  freeaddrinfo(servinfo);

  return socketEscucha;
}

int esperar_conexion(int socket_conexion, char *mensaje, t_log *log)
{
  int socket = accept(socket_conexion, NULL, NULL);

  if (socket < 0)
  {
    fprintf(stderr, "Error en Aceptacion: %s\n", strerror(socket));
    exit(1);
  }

  log_info(log, "Se acepto a: %s", mensaje);

  return socket;
}

void handshake_servidor(int socket_conexion)
{
  int32_t handshake;
  int32_t resultOk = 0;
  int32_t resultError = -1;

  recv(socket_conexion, &handshake, sizeof(int32_t), MSG_WAITALL);
  if (handshake == 1)
  {
    send(socket_conexion, &resultOk, sizeof(int32_t), 0);
  }
  else
  {
    send(socket_conexion, &resultError, sizeof(int32_t), 0);
  }
}

void handshake_cliente(int socket_conexion, t_log *log)
{

  int32_t handshake = 1;
  int32_t result;

  send(socket_conexion, &handshake, sizeof(int32_t), 0);
  recv(socket_conexion, &result, sizeof(int32_t), MSG_WAITALL);

  if (result == 0)
  {
    log_info(log, "Handshake OK");
  }
  else
  {
    perror("Error al realizar Hanshake");
    exit(1);
  }
}

int recibir_operacion(int socket_cliente)
{
  int cod_op;
  if (recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
    return cod_op;
  else
  {
    close(socket_cliente);
    return -1;
  }
}

void *recibir_buffer(int *size, int socket_cliente)
{
  void *buffer;

  recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
  buffer = malloc(*size);
  recv(socket_cliente, buffer, *size, MSG_WAITALL);

  return buffer;
}

void recibir_mensaje(int socket_cliente)
{
  int size;
  char *buffer = recibir_buffer(&size, socket_cliente);
  // TODO: MODIFICAR FUNCION PARA QUE RECIBA UN LOGGER Y ESCRIBA EN ESE MISMO
  // log_info(logger, "Me llego el mensaje %s", buffer);
  free(buffer);
}

t_list *recibir_paquete(int socket_cliente)
{
  int size;
  int desplazamiento = 0;
  void *buffer;
  t_list *valores = list_create();
  int tamanio;

  buffer = recibir_buffer(&size, socket_cliente);
  while (desplazamiento < size)
  {
    memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
    desplazamiento += sizeof(int);
    char *valor = malloc(tamanio);
    memcpy(valor, buffer + desplazamiento, tamanio);
    desplazamiento += tamanio;
    list_add(valores, valor);
  }
  free(buffer);
  return valores;
}