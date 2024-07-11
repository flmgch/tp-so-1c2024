#ifndef INSTRUCCIONES_H_
#define INSTRUCCIONES_H_

#include "gestor_cpu.h"

void* obtener_registro(char registro[20]);
void ejecutar_set(char registro[20] , char valor[20] );
void ejecutar_sum(char registro_destino[20] , char registro_origen[20] );
void ejecutar_sub(char registro_destino[20] , char registro_origen[20] );
void ejecutar_jnz(char registro[20] , char instruccion[20] );
void ejecutar_io_gen_sleep(char interfaz[20] , char unidades_de_trabajo[20] );
void ejecutar_exit ();
void ejecutar_resize(char tamanio [20]);
void ejecutar_io_stdin_read(char interfaz[20], char reg_dir_logica[20], char reg_tam [20]);
void ejecutar_io_stdout_write(char interfaz[20], char reg_dir_logica[20], char reg_tam [20]);
void ejecutar_io_fs_create(char interfaz[20], char nombre_fs[20]);
void ejecutar_io_fs_delete(char interfaz[20], char nombre_fs[20]);
void ejecutar_io_fs_truncate(char interfaz[20], char nombre_fs[20], char reg_tam[20]);
void ejecutar_io_fs_write(char interfaz[20], char nombre_archivo[20], char reg_dir_logica[20], char reg_tam[20], char reg_ptr[20]);
void ejecutar_io_fs_read(char interfaz[20], char nombre_archivo[20], char reg_dir_logica[20], char reg_tam[20], char reg_ptr[20]);

#endif