#include "instrucciones.h"
#include "instrucciones_mem.h"

void* obtener_registro(char registro[20]) {
    if (strcmp(registro, "AX") == 0) {
        return &(pcb->registros_cpu->ax);
    } else if (strcmp(registro, "BX") == 0) {
        return &(pcb->registros_cpu->bx);
    } else if (strcmp(registro, "CX") == 0) {
        return &(pcb->registros_cpu->cx);
    } else if (strcmp(registro, "DX") == 0) {
        return &(pcb->registros_cpu->dx);
    } else if (strcmp(registro, "EAX") == 0) {
        return &(pcb->registros_cpu->eax);
    } else if (strcmp(registro, "EBX") == 0) {
        return &(pcb->registros_cpu->ebx);
    } else if (strcmp(registro, "ECX") == 0) {
        return &(pcb->registros_cpu->ecx);
    } else if (strcmp(registro, "EDX") == 0) {
        return &(pcb->registros_cpu->edx);
    } else if (strcmp(registro, "SI") == 0) {
        return &(pcb->registros_cpu->si);
    } else if (strcmp(registro, "DI") == 0) {
        return &(pcb->registros_cpu->di);
    } else {
        return NULL;
    }
}

//ATOI PARA CHAR[]->INT
void ejecutar_set(char registro[20], char valor[20]) {
    if (strcmp(registro, "AX") == 0) {
        pcb->registros_cpu->ax = (uint8_t)atoi(valor);
    } else if (strcmp(registro, "BX") == 0) {
        pcb->registros_cpu->bx = (uint8_t)atoi(valor);
    } else if (strcmp(registro, "CX") == 0) {
        pcb->registros_cpu->cx = (uint8_t)atoi(valor);
    } else if (strcmp(registro, "DX") == 0) {
        pcb->registros_cpu->dx = (uint8_t)atoi(valor);
    } else if (strcmp(registro, "EAX") == 0) {
        pcb->registros_cpu->eax = (uint32_t)atoi(valor);
    } else if (strcmp(registro, "EBX") == 0) {
        pcb->registros_cpu->ebx = (uint32_t)atoi(valor);
    } else if (strcmp(registro, "ECX") == 0) {
        pcb->registros_cpu->ecx = (uint32_t)atoi(valor);
    } else if (strcmp(registro, "EDX") == 0) {
        pcb->registros_cpu->edx = (uint32_t)atoi(valor);
    } else if (strcmp(registro, "SI") == 0) {
        pcb->registros_cpu->si = (uint32_t)atoi(valor);
    } else if (strcmp(registro, "DI") == 0) {
        pcb->registros_cpu->di = (uint32_t)atoi(valor);
    } else {
        log_error(cpu_logger, "No se hallo el registro");
    } 
}


void ejecutar_sum(char registro_destino[20] , char registro_origen[20] ){
    void* puntero_destino = obtener_registro(registro_destino); //void* : direccion de ALGO, 
    void* puntero_origen = obtener_registro(registro_origen);


    if (puntero_destino != NULL && puntero_origen != NULL) {
        //determino si los registros son de 1 o 4 bytes
        if (strcmp(registro_destino, "AX") == 0 || strcmp(registro_destino, "BX") == 0 ||
            strcmp(registro_destino, "CX") == 0 || strcmp(registro_destino, "DX") == 0) {
            *(uint8_t*)puntero_destino += *(uint8_t*)puntero_origen;
        } else {
            *(uint32_t*)puntero_destino += *(uint32_t*)puntero_origen;
        }
    } else {
        log_error(cpu_logger, "Registro invalido");
    }
}

void ejecutar_sub(char registro_destino[20] , char registro_origen[20] ){
        void* puntero_destino = obtener_registro(registro_destino);
    void* puntero_origen = obtener_registro(registro_origen);


    if (puntero_destino != NULL && puntero_origen != NULL) {
        //determino si los registros son de 1 o 4 bytes
        if (strcmp(registro_destino, "AX") == 0 || strcmp(registro_destino, "BX") == 0 ||
            strcmp(registro_destino, "CX") == 0 || strcmp(registro_destino, "DX") == 0) {
            *(uint8_t*)puntero_destino -= *(uint8_t*)puntero_origen;
        } else {
            *(uint32_t*)puntero_destino -= *(uint32_t*)puntero_origen;
        }
    } else {
        log_error(cpu_logger, "Registro invalido");
    }
}

void ejecutar_jnz(char registro[20] , char instruccion[20]){
     void* puntero_registro = obtener_registro(registro);
     uint32_t nuevo_ip = atoi (instruccion);

      if (puntero_registro != NULL) {
        if (strcmp(registro, "AX") == 0 || strcmp(registro, "BX") == 0 ||
            strcmp(registro, "CX") == 0 || strcmp(registro, "DX") == 0) {
            if (*(uint8_t*)puntero_registro != 0) {
                pcb->program_counter = nuevo_ip;
            }
        } else {
            if (*(uint32_t*)puntero_registro != 0) {
                pcb->program_counter = nuevo_ip;
            }
        }
    } else {
        log_error(cpu_logger, "Registro invalido");
    }
}

void ejecutar_io_gen_sleep(char interfaz[20] , char unidades_de_trabajo[20] ){
    t_buffer *un_buffer = crear_buffer();
    t_paquete *paquete = crear_super_paquete(OP_IO_GEN_SLEEP, un_buffer);
    agregar_uint32_a_buffer(un_buffer, atoi(unidades_de_trabajo));
    agregar_string_a_buffer(un_buffer, interfaz);

    enviar_paquete(paquete, socket_kernel_dispatch);
    eliminar_paquete(paquete);
    destruir_buffer(un_buffer);
}

void ejecutar_exit (){
    enviar_pcb(pcb,socket_kernel_dispatch);
}

