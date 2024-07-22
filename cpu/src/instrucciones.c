#include "instrucciones.h"

void* obtener_registro(char registro[20]) {
    if (strcmp(registro, "PC") == 0)
    {
        return &(pcb->registros_cpu->pc);
    }
    else if (strcmp(registro, "AX") == 0)
    {
        return &(pcb->registros_cpu->ax);
    }
    else if (strcmp(registro, "BX") == 0)
    {
        return &(pcb->registros_cpu->bx);
    }
    else if (strcmp(registro, "CX") == 0)
    {
        return &(pcb->registros_cpu->cx);
    }
    else if (strcmp(registro, "DX") == 0)
    {
        return &(pcb->registros_cpu->dx);
    }
    else if (strcmp(registro, "EAX") == 0)
    {
        return &(pcb->registros_cpu->eax);
    }
    else if (strcmp(registro, "EBX") == 0)
    {
        return &(pcb->registros_cpu->ebx);
    }
    else if (strcmp(registro, "ECX") == 0)
    {
        return &(pcb->registros_cpu->ecx);
    }
    else if (strcmp(registro, "EDX") == 0)
    {
        return &(pcb->registros_cpu->edx);
    }
    else if (strcmp(registro, "SI") == 0)
    {
        return &(pcb->registros_cpu->si);
    }
    else if (strcmp(registro, "DI") == 0)
    {
        return &(pcb->registros_cpu->di);
    }
    else
    {
        return NULL;
    }
}

//ATOI PARA CHAR[]->INT
void ejecutar_set(char registro[20], char valor[20]) {
    if (strcmp(registro, "PC") == 0)
    {
        pcb->registros_cpu->pc = (uint32_t)atoi(valor);
        pc_modificado = true;
    }
    else if (strcmp(registro, "AX") == 0)
    {
        pcb->registros_cpu->ax = (uint8_t)atoi(valor);
        log_info(cpu_logger, "%d", pcb->registros_cpu->ax);
    }
    else if (strcmp(registro, "BX") == 0)
    {
        pcb->registros_cpu->bx = (uint8_t)atoi(valor);
        log_info(cpu_logger, "%d", pcb->registros_cpu->bx);
    }
    else if (strcmp(registro, "CX") == 0)
    {
        pcb->registros_cpu->cx = (uint8_t)atoi(valor);
        log_info(cpu_logger, "%d", pcb->registros_cpu->cx);
    }
    else if (strcmp(registro, "DX") == 0)
    {
        pcb->registros_cpu->dx = (uint8_t)atoi(valor);
        log_info(cpu_logger, "%d", pcb->registros_cpu->dx);
    }
    else if (strcmp(registro, "EAX") == 0)
    {
        pcb->registros_cpu->eax = (uint32_t)atoi(valor);
        log_info(cpu_logger, "%d", pcb->registros_cpu->eax);
    }
    else if (strcmp(registro, "EBX") == 0)
    {
        pcb->registros_cpu->ebx = (uint32_t)atoi(valor);
        log_info(cpu_logger, "%d", pcb->registros_cpu->ebx);
    }
    else if (strcmp(registro, "ECX") == 0)
    {
        pcb->registros_cpu->ecx = (uint32_t)atoi(valor);
        log_info(cpu_logger, "%d", pcb->registros_cpu->ecx);
    }
    else if (strcmp(registro, "EDX") == 0)
    {
        pcb->registros_cpu->edx = (uint32_t)atoi(valor);
        log_info(cpu_logger, "%d", pcb->registros_cpu->edx);
    }
    else if (strcmp(registro, "SI") == 0)
    {
        pcb->registros_cpu->si = (uint32_t)atoi(valor);
        log_info(cpu_logger, "%d", pcb->registros_cpu->si);
    }
    else if (strcmp(registro, "DI") == 0)
    {
        pcb->registros_cpu->di = (uint32_t)atoi(valor);
        log_info(cpu_logger, "%d", pcb->registros_cpu->di);
    }
    else
    {
        log_error(cpu_logger, "No se hallo el registro");
    }
}

//SUM
void ejecutar_sum(char registro_destino[20] , char registro_origen[20] ){
    void* puntero_destino = obtener_registro(registro_destino); //void* : direccion de ALGO, 
    void* puntero_origen = obtener_registro(registro_origen);


    if (puntero_destino != NULL && puntero_origen != NULL) {
        //determino si los registros son de 1 o 4 bytes
        if (strcmp(registro_destino, "AX") == 0 || strcmp(registro_destino, "BX") == 0 ||
            strcmp(registro_destino, "CX") == 0 || strcmp(registro_destino, "DX") == 0) {
            *(uint8_t*)puntero_destino += *(uint8_t*)puntero_origen;
            log_info(cpu_logger, "%d", pcb->registros_cpu->ax);
        } else {
            *(uint32_t*)puntero_destino += *(uint32_t*)puntero_origen;
        }
    } else {
        log_error(cpu_logger, "Registro invalido");
    }
}


//SUB
void ejecutar_sub(char registro_destino[20] , char registro_origen[20] ){
        void* puntero_destino = obtener_registro(registro_destino);
    void* puntero_origen = obtener_registro(registro_origen);


    if (puntero_destino != NULL && puntero_origen != NULL) {
        //determino si los registros son de 1 o 4 bytes
        if (strcmp(registro_destino, "AX") == 0 || strcmp(registro_destino, "BX") == 0 ||
            strcmp(registro_destino, "CX") == 0 || strcmp(registro_destino, "DX") == 0) {
            *(uint8_t*)puntero_destino -= *(uint8_t*)puntero_origen;
            log_info(cpu_logger, "%d", pcb->registros_cpu->cx);
        } else {
            *(uint32_t*)puntero_destino -= *(uint32_t*)puntero_origen;
        }
    } else {
        log_error(cpu_logger, "Registro invalido");
    }
}


//JNZ
void ejecutar_jnz(char registro[20] , char instruccion[20]){
     void* puntero_registro = obtener_registro(registro);
     uint32_t nuevo_ip = atoi (instruccion);

      if (puntero_registro != NULL) {
        if (strcmp(registro, "AX") == 0 || strcmp(registro, "BX") == 0 ||
            strcmp(registro, "CX") == 0 || strcmp(registro, "DX") == 0) {
            if (*(uint8_t*)puntero_registro != 0) {
                pcb->program_counter = nuevo_ip;
                pcb->registros_cpu->pc = nuevo_ip;
                pc_modificado = true;
                log_info(cpu_logger, "%d", pcb->program_counter);
            }
        } else {
            if (*(uint32_t*)puntero_registro != 0) {
                pcb->program_counter = nuevo_ip;
                pcb->registros_cpu->pc = nuevo_ip;
                pc_modificado = true;
                log_info(cpu_logger, "%d", pcb->program_counter);
            }
        }
    } else {
        log_error(cpu_logger, "Registro invalido");
    }
}

//IO_GEN_SLEEP
void ejecutar_io_gen_sleep(char interfaz[20] , char unidades_de_trabajo[20] ){
    t_buffer *un_buffer = crear_buffer();
    agregar_pcb_a_buffer(un_buffer, pcb);
    agregar_cop_a_buffer(un_buffer, OP_IO_GEN_SLEEP);
    agregar_string_a_buffer(un_buffer, interfaz);
    agregar_uint32_a_buffer(un_buffer, atoi(unidades_de_trabajo));
    
    t_paquete *paquete = crear_super_paquete(ENVIO_PCB, un_buffer);
    enviar_paquete(paquete, socket_kernel_dispatch);
    eliminar_paquete(paquete);
}

//EXIT
void ejecutar_exit (){
    t_buffer *buffer = crear_buffer();
    pcb->motivo_exit = SUCCESS;
    agregar_pcb_a_buffer(buffer, pcb);
    agregar_cop_a_buffer(buffer, CAMBIAR_ESTADO);
    agregar_estado_a_buffer(buffer, FINISH_EXIT);
    t_paquete *paquete = crear_super_paquete(ENVIO_PCB, buffer);
    enviar_paquete(paquete, socket_kernel_dispatch);
    eliminar_paquete(paquete);
}


//RESIZE
void ejecutar_resize(char tamanio [20]){
    t_buffer *un_buffer = crear_buffer();
    agregar_int_a_buffer(un_buffer, atoi(tamanio));
    agregar_uint32_a_buffer(un_buffer, pcb->pid);

    t_paquete *paquete = crear_super_paquete(AJUSTAR_TAMANIO, un_buffer);
    enviar_paquete(paquete, socket_memoria);

    eliminar_paquete(paquete);
}

void procesar_resultado_resize(char* resultado){
    if (strcmp(resultado, "Out of Memory") == 0)
    {
        pcb->motivo_exit = OUT_OF_MEMORY;
        t_buffer *buffer = crear_buffer();
        agregar_pcb_a_buffer(buffer, pcb);
        agregar_cop_a_buffer(buffer, CAMBIAR_ESTADO);
        agregar_estado_a_buffer(buffer, FINISH_ERROR);

        t_paquete *paquete = crear_super_paquete(ENVIO_PCB, buffer);
        enviar_paquete(paquete, socket_kernel_dispatch);

        eliminar_paquete(paquete);
    }
    else if (strcmp(resultado, "Ok") == 0)
    {
        log_info(cpu_logger, "RESIZE exitoso");
        aux_resize = 1;
        //
    }
    else
    {
        log_error(cpu_logger, "Registro invalido");
    }
}

//MOV IN
void ejecutar_mov_in(char reg_dir_logica[20],char reg_datos[20]){
    uint32_t *dir_logica = obtener_registro(reg_dir_logica);
    t_buffer *un_buffer = crear_buffer();
    int tam_segun;
    t_list *direcciones;
    condicion = malloc(15);

    //datos=&reg_datos;

    agregar_uint32_a_buffer(un_buffer, pcb->pid);
    if (reg_datos != NULL) {
        if (strcmp(reg_datos, "AX") == 0 || strcmp(reg_datos, "BX") == 0 ||
            strcmp(reg_datos, "CX") == 0 || strcmp(reg_datos, "DX") == 0) {
            tam_segun = sizeof(uint8_t);
            reg_aux = malloc(tam_segun);
            condicion = "Uint8";
            // agregar_string_a_buffer(un_buffer, condicion);
        }
        else
        {
            tam_segun = sizeof(uint32_t);
            reg_aux = malloc(tam_segun);
            condicion = "Uint32";
            // agregar_string_a_buffer(un_buffer, condicion);
        }
        }

        direcciones = separar_en_paginas(*dir_logica, tam_segun);

        // t_direccion_fisica *d = list_get(direcciones, 0);

        /*int l = list_size(direcciones);

        log_info(cpu_logger, "DF: %d tam: %d pag: %d", d->direccion_fisica, d->tamanio_dato, l);*/

        // agregar_int_a_buffer(un_buffer,tam_segun_reg);
        agregar_lista_direcciones_a_buffer(un_buffer, direcciones);
        agregar_int_a_buffer(un_buffer, tam_segun);

        t_paquete *paquete = crear_super_paquete(ACCESO_ESPACIO_USUARIO_LECTURA, un_buffer);
        enviar_paquete(paquete, socket_memoria);
        eliminar_paquete(paquete);

        sem_wait(&sem_resultado_lectura);

        ejecutar_set(reg_datos, reg_aux);
        if (strcmp(condicion, "Uint8") == 0)
        {
            uint8_t *aux_resultado = obtener_registro(reg_datos);
            log_info(cpu_logger, "Estado registro al final: %d", *aux_resultado);
        }
        else
        {
            uint32_t *aux_resultado = obtener_registro(reg_datos);
            log_info(cpu_logger, "Estado registro al final: %d", *aux_resultado);
        }
        free(reg_aux);
}

/*void recibir_dato(void* dato_recibido){
    char* dato=*(char* ) dato_recibido;
    ejecutar_set(*datos,* dato);
}*/

//MOV OUT
void ejecutar_mov_out(char reg_destino[20], char reg_datos[20]){
    uint32_t *dir_logica = obtener_registro(reg_destino); //
    t_list *direcciones;
    void *valor;
    int tam_segun;

    t_buffer *un_buffer = crear_buffer();
    agregar_uint32_a_buffer(un_buffer, pcb->pid);

    if (reg_datos != NULL)
    {
        if (strcmp(reg_datos, "AX") == 0 || strcmp(reg_datos, "BX") == 0 ||
            strcmp(reg_datos, "CX") == 0 || strcmp(reg_datos, "DX") == 0)
        {
            tam_segun = sizeof(uint8_t);
            uint8_t *aux_valor = obtener_registro(reg_datos);
            valor = malloc(tam_segun);
            memcpy(valor, aux_valor, tam_segun);
            // agregar_string_a_buffer(un_buffer, "Uint8");
            // agregar_int_a_buffer(un_buffer, tam_segun);
            agregar_a_buffer(un_buffer, valor, tam_segun);
        }
        else
        {
            tam_segun = sizeof(uint32_t);
            uint32_t *aux_valor = obtener_registro(reg_datos);
            valor = malloc(tam_segun);
            memcpy(valor, aux_valor, tam_segun);
            // agregar_string_a_buffer(un_buffer, "Uint32");
            // agregar_int_a_buffer(un_buffer, tam_segun);
            agregar_a_buffer(un_buffer, valor, tam_segun);
        }
    }

    direcciones = separar_en_paginas(*dir_logica, tam_segun);

    /*t_direccion_fisica *d = list_get(direcciones, 0);

    log_info(cpu_logger, "DF: %d tam: %d", d->direccion_fisica, d->tamanio_dato);*/
    agregar_lista_direcciones_a_buffer(un_buffer, direcciones);

    t_paquete *paquete = crear_super_paquete(ACCESO_ESPACIO_USUARIO_ESCRITURA, un_buffer);
    enviar_paquete(paquete, socket_memoria);
    eliminar_paquete(paquete);
}
// COPY STRING
void ejecutar_copy_string(char ch_tamanio[20])
{

    int tamanio = atoi(ch_tamanio);
    reg_aux = malloc(tamanio);
    void *valor = malloc(tamanio);
    condicion = "String";
    t_list *direcciones_origen = separar_en_paginas(pcb->registros_cpu->si, tamanio);
    t_list *direcciones_destino = separar_en_paginas(pcb->registros_cpu->di, tamanio);

    t_buffer *un_buffer = crear_buffer();
    agregar_uint32_a_buffer(un_buffer, pcb->pid);
    agregar_lista_direcciones_a_buffer(un_buffer, direcciones_origen);
    agregar_int_a_buffer(un_buffer, tamanio);

    t_paquete *paquete1 = crear_super_paquete(ACCESO_ESPACIO_USUARIO_LECTURA, un_buffer);
    enviar_paquete(paquete1, socket_memoria);
    eliminar_paquete(paquete1);

    sem_wait(&sem_resultado_lectura);
    log_info(cpu_logger, "String leido: %s", reg_aux);
    memcpy(valor, reg_aux, tamanio);

    t_buffer *otro_buffer = crear_buffer();
    agregar_uint32_a_buffer(otro_buffer, pcb->pid);
    agregar_a_buffer(otro_buffer, valor, tamanio);
    agregar_lista_direcciones_a_buffer(otro_buffer, direcciones_destino);

    t_paquete *paquete2 = crear_super_paquete(ACCESO_ESPACIO_USUARIO_ESCRITURA, otro_buffer);
    enviar_paquete(paquete2, socket_memoria);
    eliminar_paquete(paquete2);

    free(reg_aux);
}

//IO_STDIN_READ
void ejecutar_io_stdin_read(char interfaz[20], char reg_dir_logica[20], char reg_tam [20]){
    void* dir_logica = obtener_registro(reg_dir_logica); //las dl son de 32
    uint32_t direccion_logica = *(uint32_t*)dir_logica; //hago void* -> uint32_t*; y *(uint32_t*) para obtener el valor almacenado en la dirección apuntada
    void* dir_tam = obtener_registro(reg_tam);
    t_list* direcciones;
    t_buffer *un_buffer = crear_buffer();
    agregar_pcb_a_buffer(un_buffer, pcb);
    agregar_cop_a_buffer(un_buffer, OP_IO_STDIN_READ);
    agregar_string_a_buffer(un_buffer,interfaz);

    // ? Capaz esto hace falta, pero pensamos que unificarlo a u_int32 es suficiente.
    // if (dir_tam != NULL) {
    //     if (strcmp(reg_tam, "AX") == 0 || strcmp(reg_tam, "BX") == 0 ||
    //         strcmp(reg_tam, "CX") == 0 || strcmp(reg_tam, "DX") == 0) {
    //             uint8_t tamanio = *(uint8_t*) dir_tam;
    //             agregar_string_a_buffer(buffer, "Uint8");
    //             agregar_uint8_a_buffer(buffer, tamanio);
    //             direcciones = separar_en_paginas(direccion_logica, tamanio);
    //     } else {
    //             uint32_t tamanio = *(uint32_t*) dir_tam;
    //             agregar_string_a_buffer(buffer, "Uint32");
    //             agregar_uint32_a_buffer(buffer, tamanio);
    //             direcciones = separar_en_paginas(direccion_logica, tamanio);
    //         } 
    //     }

    u_int32_t tamanio = *(uint32_t*) dir_tam;
    direcciones = separar_en_paginas(direccion_logica, tamanio);
    agregar_lista_direcciones_a_buffer(un_buffer,direcciones);
    agregar_uint32_a_buffer(un_buffer, tamanio);
    t_paquete *paquete = crear_super_paquete(ENVIO_PCB, un_buffer);
    enviar_paquete(paquete, socket_kernel_dispatch);
    eliminar_paquete(paquete);
}

//IO_STDOUT_WRITE
void ejecutar_io_stdout_write(char interfaz[20], char reg_dir_logica[20], char reg_tam [20]){
    void* dir_logica = obtener_registro(reg_dir_logica); //las dl son de 32
    uint32_t direccion_logica = *(uint32_t*)dir_logica; //hago void* -> uint32_t*; y *(uint32_t*) para obtener el valor almacenado en la dirección apuntada
    void* dir_tam = obtener_registro(reg_tam);
    t_list* direcciones;
    t_buffer *un_buffer = crear_buffer();
    agregar_pcb_a_buffer(un_buffer, pcb);
    agregar_cop_a_buffer(un_buffer, OP_IO_STDOUT_WRITE);
    agregar_string_a_buffer(un_buffer,interfaz);


// ? Capaz esto hace falta, pero pensamos que unificarlo a u_int32 es suficiente.
    // if (dir_tam != NULL) {
    //     if (strcmp(reg_tam, "AX") == 0 || strcmp(reg_tam, "BX") == 0 ||
    //         strcmp(reg_tam, "CX") == 0 || strcmp(reg_tam, "DX") == 0) {
    //             uint8_t tamanio = *(uint8_t*) dir_tam;
    //             agregar_string_a_buffer(buffer, "Uint8");
    //             agregar_uint8_a_buffer(buffer, tamanio);
    //             direcciones = separar_en_paginas(direccion_logica, tamanio);
    //     } else {
    //             uint32_t tamanio = *(uint32_t*) dir_tam;
    //             agregar_string_a_buffer(buffer, "Uint32");
    //             agregar_uint32_a_buffer(buffer, tamanio);
    //             direcciones = separar_en_paginas(direccion_logica, tamanio);
    //         } 
    //     }

    u_int32_t tamanio = *(uint32_t*) dir_tam;
    direcciones = separar_en_paginas(direccion_logica, tamanio);
    agregar_lista_direcciones_a_buffer(un_buffer,direcciones);
    agregar_uint32_a_buffer(un_buffer, tamanio);
    t_paquete *paquete = crear_super_paquete(ENVIO_PCB, un_buffer);
    enviar_paquete(paquete, socket_kernel_dispatch);
    eliminar_paquete(paquete);
}

//IO_FS_CREATE
void ejecutar_io_fs_create(char interfaz[20], char nombre_archivo[20]){
    t_buffer *otro_buffer = crear_buffer();
    agregar_pcb_a_buffer(otro_buffer, pcb);
    agregar_cop_a_buffer(otro_buffer, OP_IO_FS_CREATE);
    agregar_string_a_buffer(otro_buffer,interfaz);
    agregar_string_a_buffer(otro_buffer,nombre_archivo);
    t_paquete *paquete = crear_super_paquete(ENVIO_PCB, otro_buffer);
    enviar_paquete(paquete, socket_kernel_dispatch);
    eliminar_paquete(paquete);
}

//IO_FS_DELETE
void ejecutar_io_fs_delete(char interfaz[20], char nombre_archivo[20]){
    t_buffer *un_buffer = crear_buffer();
    agregar_pcb_a_buffer(un_buffer, pcb);
    agregar_cop_a_buffer(un_buffer, OP_IO_FS_DELETE);
    agregar_string_a_buffer(un_buffer,interfaz);
    agregar_string_a_buffer(un_buffer,nombre_archivo);
    t_paquete *paquete = crear_super_paquete(ENVIO_PCB, un_buffer);
    enviar_paquete(paquete, socket_kernel_dispatch);
    eliminar_paquete(paquete);
}

//IO_FS_TRUNCATE
void ejecutar_io_fs_truncate(char interfaz[20], char nombre_archivo[20], char reg_tam[20]){
    t_buffer *otro_buffer = crear_buffer();
    agregar_pcb_a_buffer(otro_buffer, pcb);
    agregar_cop_a_buffer(otro_buffer, OP_IO_FS_TRUNCATE);
    agregar_string_a_buffer(otro_buffer,interfaz);
    agregar_string_a_buffer(otro_buffer,nombre_archivo);

    void* dir_tam = obtener_registro(reg_tam);
    // if (dir_tam != NULL) {
    //     if (strcmp(reg_tam, "AX") == 0 || strcmp(reg_tam, "BX") == 0 ||
    //         strcmp(reg_tam, "CX") == 0 || strcmp(reg_tam, "DX") == 0) {
    //             uint8_t tamanio = *(uint8_t*) dir_tam;
    //             agregar_string_a_buffer(otro_buffer, "Uint8");
    //             agregar_uint8_a_buffer(otro_buffer, tamanio);

    //     } else {
    //             uint32_t tamanio = *(uint32_t*) dir_tam;
    //             agregar_string_a_buffer(otro_buffer, "Uint32");
    //             agregar_uint32_a_buffer(otro_buffer, tamanio);
    //         }
    //     }
    uint32_t tamanio = *(uint32_t *)dir_tam;
    agregar_uint32_a_buffer(otro_buffer, tamanio);

    t_paquete *paquete = crear_super_paquete(ENVIO_PCB, otro_buffer);
    enviar_paquete(paquete, socket_kernel_dispatch);
    eliminar_paquete(paquete);
}

//IO_FS_WRITE
void ejecutar_io_fs_write(char interfaz[20], char nombre_archivo[20], char reg_dir_logica[20], char reg_tam[20], char reg_ptr[20]){
    //creo buffer
    t_buffer *un_buffer = crear_buffer();

    // agrego pcb y codigo de operacion
    agregar_pcb_a_buffer(un_buffer, pcb);
    agregar_cop_a_buffer(un_buffer, OP_IO_FS_WRITE);

    //agrego al buffer la interfaz y nombre del archivo
    agregar_string_a_buffer(un_buffer,interfaz);
    agregar_string_a_buffer(un_buffer,nombre_archivo);
    
     //obtnego dir_logica
    void* dir_logica = obtener_registro(reg_dir_logica); 
    uint32_t direccion_logica = *(uint32_t*)dir_logica; 
    t_list* direcciones_fisicas;


    //el tamanio del int a agregar depende del tamanio del registro
    //obtengo puntero al registro
    void* dir_tam = obtener_registro(reg_tam);
    // if (dir_tam != NULL) {
    //     if (strcmp(reg_tam, "AX") == 0 || strcmp(reg_tam, "BX") == 0 ||
    //         strcmp(reg_tam, "CX") == 0 || strcmp(reg_tam, "DX") == 0) {
    //             //si es uno de los registros anteriores, el int a agregar va a ser de uint8
    //             //creo la variable de dicho tamanio, q va a tener el contenido apuntado por dir_tam
    //             uint8_t tamanio = *(uint8_t*) dir_tam;
    //             direcciones_fisicas = separar_en_paginas(direccion_logica, tamanio);
    //     } else {
    //             //si no es uno de los registros anteriores, es de los otros, cuyo tamanio es 32
    //             //creo la variable de tipo uint32,q va a tener el contenido apuntado por dir_tam
    //             uint32_t tamanio = *(uint32_t*) dir_tam;
    //             direcciones_fisicas = separar_en_paginas(direccion_logica, tamanio);
    //         }
    //     }
    uint32_t tamanio = *(uint32_t *)dir_tam;
    direcciones_fisicas = separar_en_paginas(direccion_logica, tamanio);

    agregar_lista_direcciones_a_buffer(un_buffer,direcciones_fisicas);
    agregar_uint32_a_buffer(un_buffer, tamanio);
    //obtengo dir del registro
    void* dir_ptr = obtener_registro(reg_ptr);
    // if (dir_ptr != NULL) {
    //     if (strcmp(reg_tam, "AX") == 0 || strcmp(reg_tam, "BX") == 0 ||
    //         strcmp(reg_tam, "CX") == 0 || strcmp(reg_tam, "DX") == 0) {
    //             //obtengo el contenido de la direccion, y la agrego al buffer
    //             uint8_t puntero = *(uint8_t*) dir_ptr;
    //             agregar_string_a_buffer(un_buffer, "Uint8");
    //             agregar_uint8_a_buffer(un_buffer, puntero);
    //     } else {
    //             //idem
    //             uint32_t puntero = *(uint32_t*) dir_ptr;
    //             agregar_string_a_buffer(un_buffer, "Uint32");
    //             agregar_uint32_a_buffer(un_buffer, puntero);
    //         }
    //     }
    uint32_t puntero = *(uint32_t *)dir_ptr;
    agregar_uint32_a_buffer(un_buffer, puntero);

    t_paquete *paquete = crear_super_paquete(ENVIO_PCB, un_buffer);
    enviar_paquete(paquete, socket_kernel_dispatch);
    eliminar_paquete(paquete);
}


//IO_FS_READ
void ejecutar_io_fs_read(char interfaz[20], char nombre_archivo[20], char reg_dir_logica[20],char reg_tam[20], char reg_ptr[20]){
    t_buffer *un_buffer = crear_buffer();
    agregar_pcb_a_buffer(un_buffer, pcb);
    agregar_cop_a_buffer(un_buffer, OP_IO_FS_READ);
    agregar_string_a_buffer(un_buffer,interfaz);
    agregar_string_a_buffer(un_buffer,nombre_archivo);
    t_list *direcciones_fisicas;
    void* dir_logica = obtener_registro(reg_dir_logica); 
    uint32_t direccion_logica = *(uint32_t*)dir_logica; 
    //uint32_t direccion_fisica =  traducir_direccion_logica(direccion_logica);
    //agregar_uint32_a_buffer(un_buffer, direccion_fisica);

    void* dir_tam = obtener_registro(reg_tam);
    // if (dir_tam != NULL) {
    //     if (strcmp(reg_tam, "AX") == 0 || strcmp(reg_tam, "BX") == 0 ||
    //         strcmp(reg_tam, "CX") == 0 || strcmp(reg_tam, "DX") == 0) {
    //             uint8_t tamanio = *(uint8_t*) dir_tam;
    //             direcciones_fisicas = separar_en_paginas(direccion_logica, tamanio);
    //     } else {
    //             uint32_t tamanio = *(uint32_t*) dir_tam;
    //             direcciones_fisicas = separar_en_paginas(direccion_logica, tamanio);
    //         }
    //     }
    uint32_t tamanio = *(uint32_t *)dir_tam;
    direcciones_fisicas = separar_en_paginas(direccion_logica, tamanio);
    agregar_lista_direcciones_a_buffer(un_buffer, direcciones_fisicas);
    agregar_uint32_a_buffer(un_buffer, tamanio);

    void *dir_ptr = obtener_registro(reg_ptr);
    // if (dir_ptr != NULL)
    // {
    //     if (strcmp(reg_tam, "AX") == 0 || strcmp(reg_tam, "BX") == 0 ||
    //         strcmp(reg_tam, "CX") == 0 || strcmp(reg_tam, "DX") == 0)
    //     {
    //         uint8_t puntero = *(uint8_t *)dir_ptr;
    //         agregar_string_a_buffer(un_buffer, "Uint8");
    //         agregar_uint8_a_buffer(un_buffer, puntero);
    //     }
    //     else
    //     {
    //         uint32_t puntero = *(uint32_t *)dir_ptr;
    //         agregar_string_a_buffer(un_buffer, "Uint32");
    //         agregar_uint32_a_buffer(un_buffer, puntero);
    //     }
    // }
    uint32_t puntero = *(uint32_t *)dir_ptr;
    agregar_uint32_a_buffer(un_buffer, puntero);

    t_paquete *paquete = crear_super_paquete(ENVIO_PCB, un_buffer);
    enviar_paquete(paquete, socket_kernel_dispatch);
    eliminar_paquete(paquete);
}

// WAIT
void ejecutar_wait(char recurso[20])
{
    t_buffer *un_buffer = crear_buffer();
    agregar_pcb_a_buffer(un_buffer, pcb);
    agregar_cop_a_buffer(un_buffer, ATENDER_WAIT);
    agregar_string_a_buffer(un_buffer, recurso);
    t_paquete *paquete = crear_super_paquete(ENVIO_PCB, un_buffer);
    enviar_paquete(paquete, socket_kernel_dispatch);
    eliminar_paquete(paquete);
}

// SIGNAL
void ejecutar_signal(char recurso[20])
{
    t_buffer *un_buffer = crear_buffer();
    agregar_pcb_a_buffer(un_buffer, pcb);
    agregar_cop_a_buffer(un_buffer, ATENDER_SIGNAL);
    agregar_string_a_buffer(un_buffer, recurso);
    t_paquete *paquete = crear_super_paquete(ENVIO_PCB, un_buffer);
    enviar_paquete(paquete, socket_kernel_dispatch);
    eliminar_paquete(paquete);
}