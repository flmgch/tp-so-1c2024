#include "instrucciones.h"

void* obtener_registro(char registro[20]) {
    if (strcmp(registro, "PC") == 0) {
        return &(pcb->program_counter);
    } else if (strcmp(registro, "AX") == 0) {
        return &(pcb->registros_cpu->ax);
    } else if (strcmp(registro, "BX") == 0) {
        //log_info(cpu_logger, "Seteado de: %d", pcb->registros_cpu->bx);
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
    if(strcmp(registro, "PC") == 0) {
        pcb->program_counter = atoi(valor);
        // log_info(cpu_logger, "Seteado de: %d", pcb->program_counter);
    } else if (strcmp(registro, "AX") == 0) {
        pcb->registros_cpu->ax = (uint8_t)atoi(valor);
        // log_info(cpu_logger, "Seteado de: %d", pcb->registros_cpu->ax);
    } else if (strcmp(registro, "BX") == 0) {
        pcb->registros_cpu->bx = (uint8_t)atoi(valor);
        // log_info(cpu_logger, "Seteado de: %d", pcb->registros_cpu->bx);
    } else if (strcmp(registro, "CX") == 0) {
        pcb->registros_cpu->cx = (uint8_t)atoi(valor);
        // log_info(cpu_logger, "Seteado de: %d", pcb->registros_cpu->cx);
    } else if (strcmp(registro, "DX") == 0) {
        pcb->registros_cpu->dx = (uint8_t)atoi(valor);
        // log_info(cpu_logger, "Seteado de: %d", pcb->registros_cpu->dx);
    } else if (strcmp(registro, "EAX") == 0) {
        pcb->registros_cpu->eax = (uint32_t)atoi(valor);
        // log_info(cpu_logger, "Seteado de: %d", pcb->registros_cpu->eax);
    } else if (strcmp(registro, "EBX") == 0) {
        pcb->registros_cpu->ebx = (uint32_t)atoi(valor);
        // log_info(cpu_logger, "Seteado de: %d", pcb->registros_cpu->ebx);
    } else if (strcmp(registro, "ECX") == 0) {
        pcb->registros_cpu->ecx = (uint32_t)atoi(valor);
        // log_info(cpu_logger, "Seteado de: %d", pcb->registros_cpu->ecx);
    } else if (strcmp(registro, "EDX") == 0) {
        pcb->registros_cpu->edx = (uint32_t)atoi(valor);
        // log_info(cpu_logger, "Seteado de: %d", pcb->registros_cpu->edx);
    } else if (strcmp(registro, "SI") == 0) {
        pcb->registros_cpu->si = (uint32_t)atoi(valor);
        // log_info(cpu_logger, "Seteado de: %d", pcb->registros_cpu->si);
    } else if (strcmp(registro, "DI") == 0) {
        pcb->registros_cpu->di = (uint32_t)atoi(valor);
        // log_info(cpu_logger, "Seteado de: %d", pcb->registros_cpu->di);
    } else {
        // log_error(cpu_logger, "No se hallo el registro");
    }
    //
}

//SUM
void ejecutar_sum(char registro_destino[20] , char registro_origen[20] ){
    void* puntero_destino = obtener_registro(registro_destino); //void* : direccion de ALGO, 
    void* puntero_origen = obtener_registro(registro_origen);


    uint32_t destino=0;
    uint32_t origen=0;

    if (puntero_destino != NULL && puntero_origen != NULL) {
        //determino si los registros son de 1 o 4 bytes
        if (strcmp(registro_destino, "AX") == 0 || strcmp(registro_destino, "BX") == 0 ||
            strcmp(registro_destino, "CX") == 0 || strcmp(registro_destino, "DX") == 0) {
            uint8_t valor;
            memcpy(&valor,puntero_destino,sizeof(uint8_t));
            destino=valor;
        } else {
            memcpy(&destino,puntero_destino,sizeof(uint8_t));
        }

        if (strcmp(registro_origen, "AX") == 0 || strcmp(registro_origen, "BX") == 0 ||
            strcmp(registro_origen, "CX") == 0 || strcmp(registro_origen, "DX") == 0) {
            uint8_t valor=0;
            memcpy(&valor,puntero_origen,sizeof(uint8_t));
            origen=valor;
        } else {
            memcpy(&origen,puntero_origen,sizeof(uint8_t));
        }

        destino+=origen;
        char* resultado_suma=malloc(20);
        sprintf(resultado_suma, "%u", destino);
        ejecutar_set(registro_destino,resultado_suma);
        log_info(cpu_logger, "Resultado suma: %d", destino);
        free(resultado_suma);
    } else {
        log_error(cpu_logger, "Registro invalido");
    }
}


//SUB
void ejecutar_sub(char registro_destino[20] , char registro_origen[20] ){
        void* puntero_destino = obtener_registro(registro_destino);
    void* puntero_origen = obtener_registro(registro_origen);
    uint32_t destino=0;
    uint32_t origen=0;

    if (puntero_destino != NULL && puntero_origen != NULL) {
        //determino si los registros son de 1 o 4 bytes
        if (strcmp(registro_destino, "AX") == 0 || strcmp(registro_destino, "BX") == 0 ||
            strcmp(registro_destino, "CX") == 0 || strcmp(registro_destino, "DX") == 0) {
            uint8_t valor=0;
            memcpy(&valor,puntero_destino,sizeof(uint8_t));
            destino=valor;
        } else {
            memcpy(&destino,puntero_destino,sizeof(uint8_t));
        }

        if (strcmp(registro_origen, "AX") == 0 || strcmp(registro_origen, "BX") == 0 ||
            strcmp(registro_origen, "CX") == 0 || strcmp(registro_origen, "DX") == 0) {
            uint8_t valor=0;
            memcpy(&valor,puntero_origen,sizeof(uint8_t));
            origen=valor;
        } else {
            memcpy(&origen,puntero_origen,sizeof(uint8_t));
        }

        destino-=origen;
        char* resultado_resta=malloc(20);
        sprintf(resultado_resta, "%u", destino);
        ejecutar_set(registro_destino,resultado_resta);
        log_info(cpu_logger, "Resultado resta: %d", destino);
        free(resultado_resta);
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
                log_info(cpu_logger, "PC: %d", pcb->program_counter);
            }
        } else {
            if (*(uint32_t*)puntero_registro != 0) {
                pcb->program_counter = nuevo_ip;
                log_info(cpu_logger, "PC: %d", pcb->program_counter);
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
void ejecutar_mov_in(char reg_datos[20],char reg_dir_logica[20]){
    //void *direccion_logica = obtener_registro(reg_dir_logica);
    uint32_t dir_logica;

    if (strcmp(reg_dir_logica, "AX") == 0 || strcmp(reg_dir_logica, "BX") == 0 ||
        strcmp(reg_dir_logica, "CX") == 0 || strcmp(reg_dir_logica, "DX") == 0) {
        uint8_t* aux_dir = obtener_registro(reg_dir_logica);
        dir_logica=*aux_dir ;
        } 
        else {
        uint32_t* aux_dir = obtener_registro(reg_dir_logica);
        dir_logica=*aux_dir ;
        }
    //memcpy(&dir_logica, direccion_logica, sizeof(uint32_t));
    t_buffer *un_buffer = crear_buffer();
    int tam_segun;

    //datos=&reg_datos;

    agregar_uint32_a_buffer(un_buffer, pcb->pid);
    if (reg_datos != NULL) {
        if (strcmp(reg_datos, "AX") == 0 || strcmp(reg_datos, "BX") == 0 ||
            strcmp(reg_datos, "CX") == 0 || strcmp(reg_datos, "DX") == 0) {
            tam_segun = sizeof(uint8_t);
            condicion = strdup("Uint8");
            // agregar_string_a_buffer(un_buffer, condicion);
        }
        else
        {
            tam_segun = sizeof(uint32_t);
            condicion = strdup("Uint32");
            // agregar_string_a_buffer(un_buffer, condicion);
        }
        }

        t_list *direcciones = separar_en_paginas(dir_logica, tam_segun);

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

        t_direccion_fisica primera_direccion = *(t_direccion_fisica *)list_get(direcciones, 0);
        log_info(cpu_logger, "PID: %d - Acción: Leer- Dirección Física: %d - Valor: %s", pcb->pid, primera_direccion.direccion_fisica, reg_aux);
        free(reg_aux);
        free(condicion);
        list_destroy_and_destroy_elements(direcciones,(void*)free);
}

/*void recibir_dato(void* dato_recibido){
    char* dato=*(char* ) dato_recibido;
    ejecutar_set(*datos,* dato);
}*/

//MOV OUT
void ejecutar_mov_out(char reg_destino[20], char reg_datos[20]){
    //void *direccion_logica = obtener_registro(reg_destino);
    uint32_t dir_logica;

    if (strcmp(reg_destino, "AX") == 0 || strcmp(reg_destino, "BX") == 0 ||
        strcmp(reg_destino, "CX") == 0 || strcmp(reg_destino, "DX") == 0) {
        uint8_t* aux_dir = obtener_registro(reg_destino);
        dir_logica=*aux_dir ;
        } 
        else {
        uint32_t* aux_dir = obtener_registro(reg_destino);
        dir_logica=*aux_dir ;
        }
    //memcpy(&dir_logica, direccion_logica, sizeof(uint32_t)); //
    
    void *valor;
    int tam_segun;

    t_buffer *un_buffer = crear_buffer();
    agregar_uint32_a_buffer(un_buffer, pcb->pid);
    t_list *direcciones;

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
            direcciones = separar_en_paginas(dir_logica, tam_segun);
            t_direccion_fisica primera_direccion = *(t_direccion_fisica *)list_get(direcciones, 0);
            log_info(cpu_logger, "PID: %d - Acción: Escribir- Dirección Física: %d - Valor: %d", pcb->pid, primera_direccion.direccion_fisica, *aux_valor);
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
            direcciones = separar_en_paginas(dir_logica, tam_segun);
            t_direccion_fisica primera_direccion = *(t_direccion_fisica *)list_get(direcciones, 0);
            log_info(cpu_logger, "PID: %d - Acción: Escribir- Dirección Física: %d - Valor: %d", pcb->pid, primera_direccion.direccion_fisica, *aux_valor);
        }
    }

    /*t_direccion_fisica *d = list_get(direcciones, 0);

    log_info(cpu_logger, "DF: %d tam: %d", d->direccion_fisica, d->tamanio_dato);*/
    agregar_lista_direcciones_a_buffer(un_buffer, direcciones);

    t_paquete *paquete = crear_super_paquete(ACCESO_ESPACIO_USUARIO_ESCRITURA, un_buffer);
    enviar_paquete(paquete, socket_memoria);
    eliminar_paquete(paquete);
    free(valor);
    list_destroy_and_destroy_elements(direcciones,(void*)free);
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
    t_direccion_fisica primera_direccion_origen = *(t_direccion_fisica *)list_get(direcciones_origen, 0);
    log_info(cpu_logger, "PID: %d - Acción: Leer- Dirección Física: %d - Valor: %s", pcb->pid, primera_direccion_origen.direccion_fisica, reg_aux);
    memcpy(valor, reg_aux, tamanio);
    t_direccion_fisica primera_direccion_destino = *(t_direccion_fisica *)list_get(direcciones_destino, 0);
    log_info(cpu_logger, "PID: %d - Acción: Escribir- Dirección Física: %d - Valor: %s", pcb->pid, primera_direccion_destino.direccion_fisica, reg_aux);

    t_buffer *otro_buffer = crear_buffer();
    agregar_uint32_a_buffer(otro_buffer, pcb->pid);
    agregar_a_buffer(otro_buffer, valor, tamanio);
    agregar_lista_direcciones_a_buffer(otro_buffer, direcciones_destino);

    t_paquete *paquete2 = crear_super_paquete(ACCESO_ESPACIO_USUARIO_ESCRITURA, otro_buffer);
    enviar_paquete(paquete2, socket_memoria);
    eliminar_paquete(paquete2);

    free(reg_aux);
    list_destroy_and_destroy_elements(direcciones_destino,(void*)free);
    list_destroy_and_destroy_elements(direcciones_origen,(void*)free);
}

//IO_STDIN_READ
void ejecutar_io_stdin_read(char interfaz[20], char reg_dir_logica[20], char reg_tam [20]){
    //void *direccion_logica = obtener_registro(reg_dir_logica);
    uint32_t dir_logica;

    if (strcmp(reg_dir_logica, "AX") == 0 || strcmp(reg_dir_logica, "BX") == 0 ||
        strcmp(reg_dir_logica, "CX") == 0 || strcmp(reg_dir_logica, "DX") == 0) {
        uint8_t* aux_dir = obtener_registro(reg_dir_logica);
        dir_logica=*aux_dir ;
        } 
        else {
        uint32_t* aux_dir= obtener_registro(reg_dir_logica);
        dir_logica=*aux_dir ;
        }
    //memcpy(&dir_logica, direccion_logica, sizeof(uint32_t)); // hago void* -> uint32_t*; y *(uint32_t*) para obtener el valor almacenado en la dirección apuntada

    //void *direccion_tamaño = obtener_registro(reg_tam);
    uint32_t tamaño;
    //memcpy(&tamaño, direccion_tamaño, sizeof(uint32_t));

    
    t_buffer *un_buffer = crear_buffer();
    agregar_pcb_a_buffer(un_buffer, pcb);
    agregar_cop_a_buffer(un_buffer, OP_IO_STDIN_READ);
    agregar_string_a_buffer(un_buffer,interfaz);

    // ? Capaz esto hace falta, pero pensamos que unificarlo a u_int32 es suficiente.
    
        if (strcmp(reg_tam, "AX") == 0 || strcmp(reg_tam, "BX") == 0 ||
            strcmp(reg_tam, "CX") == 0 || strcmp(reg_tam, "DX") == 0) {
         uint8_t* aux_tamaño = obtener_registro(reg_tam);
         tamaño=*aux_tamaño;
        } 
        else {
         uint32_t* aux_tamaño = obtener_registro(reg_tam);
        tamaño=*aux_tamaño;
            } 


    t_list* direcciones= separar_en_paginas(dir_logica, tamaño);

    agregar_lista_direcciones_a_buffer(un_buffer,direcciones);
    agregar_uint32_a_buffer(un_buffer, tamaño);
    t_paquete *paquete = crear_super_paquete(ENVIO_PCB, un_buffer);
    enviar_paquete(paquete, socket_kernel_dispatch);
    eliminar_paquete(paquete);
    list_destroy_and_destroy_elements(direcciones,(void*)free);
}

//IO_STDOUT_WRITE
void ejecutar_io_stdout_write(char interfaz[20], char reg_dir_logica[20], char reg_tam [20]){
    //void *direccion_logica = obtener_registro(reg_dir_logica);
    uint32_t dir_logica;

    if (strcmp(reg_dir_logica, "AX") == 0 || strcmp(reg_dir_logica, "BX") == 0 ||
        strcmp(reg_dir_logica, "CX") == 0 || strcmp(reg_dir_logica, "DX") == 0) {
        uint8_t* aux_dir = obtener_registro(reg_dir_logica);
        dir_logica=*aux_dir ;
        } 
        else {
        uint32_t* aux_dir = obtener_registro(reg_dir_logica);
        dir_logica=*aux_dir ;
        }
    //memcpy(&dir_logica, direccion_logica, sizeof(uint32_t)); // hago void* -> uint32_t*; y *(uint32_t*) para obtener el valor almacenado en la dirección apuntada

    //void *direccion_tamaño = obtener_registro(reg_tam);
    uint32_t tamaño;
    //memcpy(&tamaño, direccion_tamaño, sizeof(uint32_t));

  
    t_buffer *un_buffer = crear_buffer();
    agregar_pcb_a_buffer(un_buffer, pcb);
    agregar_cop_a_buffer(un_buffer, OP_IO_STDOUT_WRITE);
    agregar_string_a_buffer(un_buffer,interfaz);


// ? Capaz esto hace falta, pero pensamos que unificarlo a u_int32 es suficiente.
     if (strcmp(reg_tam, "AX") == 0 || strcmp(reg_tam, "BX") == 0 ||
         strcmp(reg_tam, "CX") == 0 || strcmp(reg_tam, "DX") == 0) {
        uint8_t* aux_tamaño = obtener_registro(reg_tam);
        tamaño=*aux_tamaño;
        } 
        else {
        uint32_t* aux_tamaño = obtener_registro(reg_tam);
        tamaño=*aux_tamaño;
        } 

    t_list* direcciones = separar_en_paginas(dir_logica, tamaño);
    agregar_lista_direcciones_a_buffer(un_buffer,direcciones);
    agregar_uint32_a_buffer(un_buffer, tamaño);
    t_paquete *paquete = crear_super_paquete(ENVIO_PCB, un_buffer);
    enviar_paquete(paquete, socket_kernel_dispatch);
    eliminar_paquete(paquete);
    list_destroy_and_destroy_elements(direcciones,(void*)free);
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

    //void *direccion_tamaño = obtener_registro(reg_tam);
    uint32_t tamaño;
    //memcpy(&tamaño, direccion_tamaño, sizeof(uint32_t));
    if (strcmp(reg_tam, "AX") == 0 || strcmp(reg_tam, "BX") == 0 ||
        strcmp(reg_tam, "CX") == 0 || strcmp(reg_tam, "DX") == 0) {
        uint8_t* aux_tamaño = obtener_registro(reg_tam);
        tamaño=*aux_tamaño;
        } 
    else {
         uint32_t* aux_tamaño = obtener_registro(reg_tam);
         tamaño=*aux_tamaño;
         } 

    agregar_uint32_a_buffer(otro_buffer, tamaño);

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
    //void *direccion_logica = obtener_registro(reg_dir_logica);
    uint32_t dir_logica;

    if (strcmp(reg_dir_logica, "AX") == 0 || strcmp(reg_dir_logica, "BX") == 0 ||
        strcmp(reg_dir_logica, "CX") == 0 || strcmp(reg_dir_logica, "DX") == 0) {
        uint8_t* aux_dir = obtener_registro(reg_dir_logica);
        dir_logica=*aux_dir ;
        } 
        else {
        uint32_t* aux_dir = obtener_registro(reg_dir_logica);
        dir_logica=*aux_dir ;
        }
    uint32_t tamaño;
    //memcpy(&tamaño, direccion_tamaño, sizeof(uint32_t));

    if (strcmp(reg_tam, "AX") == 0 || strcmp(reg_tam, "BX") == 0 ||
         strcmp(reg_tam, "CX") == 0 || strcmp(reg_tam, "DX") == 0) {
        uint8_t* aux_tamaño = obtener_registro(reg_tam);
        tamaño=*aux_tamaño;
        } 
        else {
        uint32_t* aux_tamaño = obtener_registro(reg_tam);
        tamaño=*aux_tamaño;
        } 

    t_list *direcciones_fisicas = separar_en_paginas(dir_logica, tamaño);

    agregar_lista_direcciones_a_buffer(un_buffer,direcciones_fisicas);
    agregar_uint32_a_buffer(un_buffer, tamaño);
    //obtengo dir del registro
    //void* dir_ptr = obtener_registro(reg_ptr);
    uint32_t puntero;
    //memcpy(&puntero, dir_ptr, sizeof(uint32_t));
  
         if (strcmp(reg_ptr, "AX") == 0 || strcmp(reg_ptr, "BX") == 0 ||
             strcmp(reg_ptr, "CX") == 0 || strcmp(reg_ptr, "DX") == 0) {
                 //obtengo el contenido de la direccion, y la agrego al buffer
                 uint8_t* aux_ptr = obtener_registro(reg_ptr);
                 puntero=*aux_ptr;
            } 
        else {
               uint32_t* aux_ptr = obtener_registro(reg_ptr);
               puntero=*aux_ptr;
            }

    agregar_uint32_a_buffer(un_buffer, puntero);

    t_paquete *paquete = crear_super_paquete(ENVIO_PCB, un_buffer);
    enviar_paquete(paquete, socket_kernel_dispatch);
    eliminar_paquete(paquete);
    list_destroy_and_destroy_elements(direcciones_fisicas,(void*)free);
}


//IO_FS_READ
void ejecutar_io_fs_read(char interfaz[20], char nombre_archivo[20], char reg_dir_logica[20],char reg_tam[20], char reg_ptr[20]){
    t_buffer *un_buffer = crear_buffer();
    agregar_pcb_a_buffer(un_buffer, pcb);
    agregar_cop_a_buffer(un_buffer, OP_IO_FS_READ);
    agregar_string_a_buffer(un_buffer,interfaz);
    agregar_string_a_buffer(un_buffer,nombre_archivo);

    uint32_t dir_logica;

    if (strcmp(reg_dir_logica, "AX") == 0 || strcmp(reg_dir_logica, "BX") == 0 ||
        strcmp(reg_dir_logica, "CX") == 0 || strcmp(reg_dir_logica, "DX") == 0) {
        uint8_t* aux_dir = obtener_registro(reg_dir_logica);
        dir_logica=*aux_dir ;
        } 
        else {
        uint32_t* aux_dir = obtener_registro(reg_dir_logica);
        dir_logica=*aux_dir ;
        }

    //void *direccion_tamaño = obtener_registro(reg_tam);
    uint32_t tamaño;
    //memcpy(&tamaño, direccion_tamaño, sizeof(uint32_t));
    if (strcmp(reg_tam, "AX") == 0 || strcmp(reg_tam, "BX") == 0 ||
         strcmp(reg_tam, "CX") == 0 || strcmp(reg_tam, "DX") == 0) {
        uint8_t* aux_tamaño = obtener_registro(reg_tam);
        tamaño=*aux_tamaño;
        } 
        else {
        uint32_t* aux_tamaño = obtener_registro(reg_tam);
        tamaño=*aux_tamaño;
        } 

   t_list *direcciones_fisicas = separar_en_paginas(dir_logica, tamaño);

    agregar_lista_direcciones_a_buffer(un_buffer, direcciones_fisicas);
    agregar_uint32_a_buffer(un_buffer, tamaño);

    //void *dir_ptr = obtener_registro(reg_ptr);
    uint32_t puntero;
    //memcpy(&puntero, dir_ptr, sizeof(uint32_t));
    if (strcmp(reg_ptr, "AX") == 0 || strcmp(reg_ptr, "BX") == 0 ||
        strcmp(reg_ptr, "CX") == 0 || strcmp(reg_ptr, "DX") == 0) {
        uint8_t* aux_ptr = obtener_registro(reg_ptr);
        puntero=*aux_ptr;
        } 
        else {
        uint32_t* aux_ptr = obtener_registro(reg_ptr);
        puntero=*aux_ptr;
        }
    agregar_uint32_a_buffer(un_buffer, puntero);

    t_paquete *paquete = crear_super_paquete(ENVIO_PCB, un_buffer);
    enviar_paquete(paquete, socket_kernel_dispatch);
    eliminar_paquete(paquete);
    list_destroy_and_destroy_elements(direcciones_fisicas,(void*)free);
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