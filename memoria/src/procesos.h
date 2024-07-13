#ifndef PROCESOS
#define PROCESOS

#include "gestor_memoria.h"
#include "inicializar_memoria.h"
#include "espacio_usuario.h"

void enviar_instruccion(char *instruccion);
void atender_program_counter(t_buffer *buffer);
t_proceso *encontrar_proceso(t_list *lista, uint32_t pid_cpu);
bool auxiliar_soy_proceso_buscado(void *elemento);
bool soy_proceso_buscado(void *elemento, uint32_t pid_cpu);
t_proceso *atender_crear_proceso(t_buffer *buffer);
char **abrir_archivo(const char *file);
void atender_acceso_tabla_paginas(t_buffer *buffer);
int buscar_frame_libre();
void enviar_resultado(char *resultado);
void atender_ajustar_tamanio(t_buffer *buffer);
void atender_reducir_tamanio(t_proceso *proceso, int paginas_futuras, int paginas_actuales);
void atender_aumentar_tamanio(t_proceso *proceso, int new_size, int paginas_actuales, int paginas_futuras);
bool debe_ser_proceso_x(void *elemento, uint32_t pid);
bool auxiliar_no_ser_proceso_x(void *elemento);
void enviar_tamnio_pagina();
void atender_finalizar_proceso(t_buffer *buffer);

#endif