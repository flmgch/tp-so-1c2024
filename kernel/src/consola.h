#ifndef CONSOLA_H_
#define CONSOLA_H_

#include "gestor_kernel.h"

void inicializar_consola();
bool validar_instruccion(char *leido);
void atender_instruccion(char *leido);
t_pcb *crear_pcb(int pid);
void inicializar_registros_pcb(t_pcb *pcb);
int asignar_pid();
void iniciar_proceso(char* path);
void ejecutar_script(char *path);
long int tamanio_del_archivo(FILE *archivo);
void ejecutar_instruccion(char** instruccion_separada);
void finalizar_proceso();
bool es_pcb_buscado(void *data);
void detener_planificacion();
void iniciar_planificacion();
void cambiar_grado_multiprogramacion(int valor);
void imprimir_pcb_estado(void* data);
void listar_procesos_por_estado();

#endif