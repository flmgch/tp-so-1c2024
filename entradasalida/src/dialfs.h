#ifndef DIALFS_H
#define DIALFS_H

#include "gestor_io.h"

void iniciar_bitmap();
void iniciar_bloques_datos();
void iniciar_metadatas();
void iniciar_sistema_archivos();
void atender_fs_create(t_buffer *buffer);
void atender_fs_delete(t_buffer *buffer);
void atender_fs_truncate(t_buffer *buffer);
void atender_fs_write(t_buffer *buffer);
void atender_fs_read(t_buffer *buffer);

#endif