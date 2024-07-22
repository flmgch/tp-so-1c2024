#ifndef DIALFS_H
#define DIALFS_H

#include "gestor_io.h"

void iniciar_bitmap();
void iniciar_bloques_datos();
void cargar_metadatas();
void iniciar_sistema_archivos();
void atender_fs_create(t_buffer *buffer);
void atender_fs_delete(t_buffer *buffer);
void atender_fs_truncate(t_buffer *buffer);
void realizar_compactacion(int *bloque_inicial_trunc, t_config *metadata_config_trunc, char *nombre_archivo_trunc, u_int32_t pid);
void atender_fs_write(t_buffer *buffer);
void atender_fs_read(t_buffer *buffer);

#endif