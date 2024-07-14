#ifndef KERNEL_INTERFAZ_
#define KERNEL_INTERFAZ_

#include "gestor_kernel.h"

void atender_modulo_interfaz(void* socket);
void crear_interfaz (char* nombre_interfaz, char* tipo_interfaz, int fd_interfaz);

#endif