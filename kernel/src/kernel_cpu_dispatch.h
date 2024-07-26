#ifndef KERNEL_CPU_DISPATCH_
#define KERNEL_CPU_DISPATCH_

#include "gestor_kernel.h"

void atender_cpu_dispatch();
void procesar_cambio_estado(t_pcb *pcb, estado_proceso estado_nuevo);
void atender_wait(t_pcb *pcb, char *recurso);
void atender_signal(t_pcb *pcb, char *recurso);
t_recurso *buscar_recurso(char *recurso);
void atender_io_gen_sleep(t_pcb *pcb, char* nombre_interfaz, u_int32_t unidades);
void manejar_io_gen_sleep(void *parametros);
t_interfaz_kernel* buscar_interfaz(char* nombre_interfaz);
bool operacion_valida(t_interfaz_kernel* interfaz, op_code operacion);
void atender_io_stdin_read(t_pcb* pcb, char* nombre_interfaz, t_list* direcciones_fisicas, u_int32_t tamanio);
void manejar_stdin_read(void *parametros);
void atender_io_stdout_write(t_pcb* pcb, char* nombre_interfaz, t_list* direcciones_fisicas, u_int32_t tamanio);
void manejar_stdout_write(void *parametros);
void atender_io_fs_create(t_pcb *pcb, char *nombre_interfaz, char *nombre_archivo);
void manejar_fs_create(void *parametros);
void atender_io_fs_delete(t_pcb *pcb, char *nombre_interfaz, char *nombre_archivo);
void manejar_fs_delete(void *parametros);
bool chequear_quantum(t_pcb* pcb);
void atender_io_fs_truncate(t_pcb *pcb, char *nombre_interfaz, char *nombre_archivo, u_int32_t tamanio);
void manejar_fs_truncate(void *parametros);
void atender_io_fs_write(t_pcb *pcb, char *nombre_interfaz, char *nombre_archivo, t_list* lista_direcciones, u_int32_t tamanio, u_int32_t puntero);
void manejar_fs_write(void *parametros);
void atender_io_fs_read(t_pcb *pcb, char *nombre_interfaz, char *nombre_archivo, t_list* lista_direcciones, u_int32_t tamanio, u_int32_t puntero);
void manejar_fs_read(void *parametros);
#endif