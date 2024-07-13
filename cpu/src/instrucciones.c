#include "instrucciones.h"

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
        log_info(cpu_logger, "%d", pcb->registros_cpu->ax);
    } else if (strcmp(registro, "BX") == 0) {
        pcb->registros_cpu->bx = (uint8_t)atoi(valor);
        log_info(cpu_logger, "%d", pcb->registros_cpu->bx);
    } else if (strcmp(registro, "CX") == 0) {
        pcb->registros_cpu->cx = (uint8_t)atoi(valor);
        log_info(cpu_logger, "%d", pcb->registros_cpu->cx);
    } else if (strcmp(registro, "DX") == 0) {
        pcb->registros_cpu->dx = (uint8_t)atoi(valor);
        log_info(cpu_logger, "%d", pcb->registros_cpu->dx);
    } else if (strcmp(registro, "EAX") == 0) {
        pcb->registros_cpu->eax = (uint32_t)atoi(valor);
        log_info(cpu_logger, "%d", pcb->registros_cpu->eax);
    } else if (strcmp(registro, "EBX") == 0) {
        pcb->registros_cpu->ebx = (uint32_t)atoi(valor);
        log_info(cpu_logger, "%d", pcb->registros_cpu->ebx);
    } else if (strcmp(registro, "ECX") == 0) {
        pcb->registros_cpu->ecx = (uint32_t)atoi(valor);
        log_info(cpu_logger, "%d", pcb->registros_cpu->ecx);
    } else if (strcmp(registro, "EDX") == 0) {
        pcb->registros_cpu->edx = (uint32_t)atoi(valor);
        log_info(cpu_logger, "%d", pcb->registros_cpu->edx);
    } else if (strcmp(registro, "SI") == 0) {
        pcb->registros_cpu->si = (uint32_t)atoi(valor);
        log_info(cpu_logger, "%d", pcb->registros_cpu->si);
    } else if (strcmp(registro, "DI") == 0) {
        pcb->registros_cpu->di = (uint32_t)atoi(valor);
        log_info(cpu_logger, "%d", pcb->registros_cpu->di);
    } else {
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
                log_info(cpu_logger, "%d", pcb->program_counter);
            }
        } else {
            if (*(uint32_t*)puntero_registro != 0) {
                pcb->program_counter = nuevo_ip;
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
    agregar_uint32_a_buffer(un_buffer, atoi(unidades_de_trabajo));
    agregar_string_a_buffer(un_buffer, interfaz);

    t_paquete *paquete = crear_super_paquete(OP_IO_GEN_SLEEP, un_buffer);
    enviar_paquete(paquete, socket_kernel_dispatch);
    eliminar_paquete(paquete);
    destruir_buffer(un_buffer);
}

//EXIT
void ejecutar_exit (){
    t_buffer *buffer = crear_buffer();
    agregar_pcb_a_buffer(buffer, pcb);
    agregar_cop_a_buffer(buffer, CAMBIAR_ESTADO);
    agregar_estado_a_buffer(buffer, FINISH_EXIT);
    t_paquete *paquete = crear_super_paquete(ENVIO_PCB, buffer);
    enviar_paquete(paquete, socket_kernel_dispatch);
    eliminar_paquete(paquete);
    destruir_buffer(buffer);
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
    void* dir_logica = obtener_registro(reg_dir_logica); //
    uint32_t direccion_logica = *(uint32_t*)dir_logica;
    int tam_segun;
    t_list *direcciones;
    //datos=&reg_datos;
    if (reg_datos != NULL) {
        if (strcmp(reg_datos, "AX") == 0 || strcmp(reg_datos, "BX") == 0 ||
            strcmp(reg_datos, "CX") == 0 || strcmp(reg_datos, "DX") == 0) {
            tam_segun = sizeof(uint8_t);
            direcciones = separar_en_paginas(direccion_logica, tam_segun);
        }
        else
        {
            tam_segun = sizeof(uint32_t);
            direcciones = separar_en_paginas(direccion_logica, tam_segun);
        }
        }

        char *valor = malloc(tam_segun);
        memset(valor, 0, tam_segun);
        int tam_lista_direcciones = list_size(direcciones);
        int tam_final = 0;
        for (int i = 0; i < tam_lista_direcciones; i++)
        {
            t_buffer *un_buffer = crear_buffer();
            t_direccion_fisica *direccion = list_get(direcciones, i);
            agregar_uint32_a_buffer(un_buffer, pcb->pid);
            agregar_uint32_a_buffer(un_buffer, direccion->direccion_fisica);
            agregar_int_a_buffer(un_buffer, direccion->tamanio_dato);
            t_paquete *paquete = crear_super_paquete(ACCESO_ESPACIO_USUARIO_LECTURA, un_buffer);
            enviar_paquete(paquete, socket_memoria);
            eliminar_paquete(paquete);
            sem_wait(&sem_resultado_lectura);
            t_buffer *otro_buffer = recibir_buffer(socket_memoria);
            void *aux_dato = extraer_de_buffer(otro_buffer);
            memcpy(valor + tam_final, aux_dato, direccion->tamanio_dato);
            tam_final += direccion->tamanio_dato;
        }
        memset(valor, 0, tam_segun);
        ejecutar_set(reg_datos, valor);

        // t_list* direcciones_fisicas = separar_en_paginas (direccion_logica,  tam_segun_reg);

        /*t_buffer *un_buffer = crear_buffer();
        agregar_uint32_a_buffer(un_buffer, pcb->pid);
        // agregar_int_a_buffer(un_buffer,tam_segun_reg);
        agregar_lista_a_buffer(un_buffer, direcciones_fisicas);
        t_paquete *paquete = crear_super_paquete(ACCESO_ESPACIO_USUARIO_LECTURA, un_buffer);
        enviar_paquete(paquete, socket_memoria);
        eliminar_paquete(paquete);

        sem_wait(&sem_resultado);

        t_buffer *otro_buffer = recibir_buffer(socket_memoria);
        void *aux_dato = extraer_de_buffer(otro_buffer);
        char *dato = (char *)aux_dato;
        ejecutar_set(reg_datos, dato);*/
}

/*void recibir_dato(void* dato_recibido){
    char* dato=*(char* ) dato_recibido;
    ejecutar_set(*datos,* dato);
}*/

//MOV OUT
void ejecutar_mov_out(char reg_destino[20], char reg_datos[20]){
    void* dir_logica = obtener_registro(reg_destino); //
    uint32_t direccion_logica = *(uint32_t*)dir_logica;
    t_list *direcciones;

    void *valor = obtener_registro(reg_datos);
    int tam_segun;

    if (reg_datos != NULL)
    {
        if (strcmp(reg_datos, "AX") == 0 || strcmp(reg_datos, "BX") == 0 ||
            strcmp(reg_datos, "CX") == 0 || strcmp(reg_datos, "DX") == 0)
        {
            tam_segun = sizeof(uint8_t);
            direcciones = separar_en_paginas(direccion_logica, tam_segun);
        }
        else
        {
            tam_segun = sizeof(uint32_t);
            direcciones = separar_en_paginas(direccion_logica, tam_segun);
        }
        int tam_lista_direcciones = list_size(direcciones);

        for (int i = 0; i < tam_lista_direcciones; i++)
        {
            t_buffer *un_buffer = crear_buffer();
            t_direccion_fisica *direccion = list_get(direcciones, i);
            agregar_uint32_a_buffer(un_buffer, pcb->pid);
            agregar_a_buffer(un_buffer, valor, tam_segun);
            agregar_uint32_a_buffer(un_buffer, direccion->direccion_fisica);
            agregar_int_a_buffer(un_buffer, direccion->tamanio_dato);
            t_paquete *paquete = crear_super_paquete(ACCESO_ESPACIO_USUARIO_ESCRITURA, un_buffer);
            enviar_paquete(paquete, socket_memoria);
            eliminar_paquete(paquete);
            sem_wait(&sem_resultado_escritura);
        }
    }
}
//COPY STRING
void ejecutar_copy_string(char ch_tamanio[20])
{

    int tamanio = atoi(ch_tamanio);
    char *valor = malloc(tamanio);
    memset(valor, 0, tamanio);
    t_list *direcciones_origen = separar_en_paginas(pcb->registros_cpu->si);
    t_list *direcciones_destino = separar_en_paginas(pcb->registros_cpu->di);

    int tam_lista_direcciones_origen = list_size(direcciones_origen);
    int tam_lista_direcciones_destino = list_size(direcciones_destino);
    int tam_final = 0;
    for (int i = 0; i < tam_lista_direcciones_origen; i++)
    {
        t_buffer *un_buffer = crear_buffer();
        t_direccion_fisica *direccion = list_get(direcciones_origen, i);
        agregar_uint32_a_buffer(un_buffer, pcb->pid);
        agregar_uint32_a_buffer(un_buffer, direccion->direccion_fisica);
        agregar_int_a_buffer(un_buffer, direccion->tamanio_dato);
        t_paquete *paquete = crear_super_paquete(ACCESO_ESPACIO_USUARIO_LECTURA, un_buffer);
        enviar_paquete(paquete, socket_memoria);
        eliminar_paquete(paquete);
        sem_wait(&sem_resultado_lectura);
        t_buffer *otro_buffer = recibir_buffer(socket_memoria);
        void *aux_dato = extraer_de_buffer(otro_buffer);
        memcpy(valor + tam_final, aux_dato, direccion->tamanio_dato);
        tam_final += direccion->tamanio_dato;
    }
    memset(valor, 0, tamanio);

    for (int i = 0; i < tam_lista_direcciones_destino; i++)
    {
        t_buffer *un_buffer = crear_buffer();
        t_direccion_fisica *direccion = list_get(direcciones_destino, i);
        agregar_uint32_a_buffer(un_buffer, pcb->pid);
        agregar_string - a_buffer(un_buffer, valor);
        agregar_uint32_a_buffer(un_buffer, direccion->direccion_fisica);
        agregar_int_a_buffer(un_buffer, direccion->tamanio_dato);
        t_paquete *paquete = crear_super_paquete(ACCESO_ESPACIO_USUARIO_ESCRITURA, un_buffer);
        enviar_paquete(paquete, socket_memoria);
        eliminar_paquete(paquete);
        sem_wait(&sem_resultado_escritura);
    }
}

//IO_STDIN_READ
void ejecutar_io_stdin_read(char interfaz[20], char reg_dir_logica[20], char reg_tam [20]){
    void* dir_logica = obtener_registro(reg_dir_logica); //las dl son de 32
    uint32_t direccion_logica = *(uint32_t*)dir_logica; //hago void* -> uint32_t*; y *(uint32_t*) para obtener el valor almacenado en la dirección apuntada
    void* dir_tam = obtener_registro(reg_tam);
    t_list* direcciones;
    t_buffer *un_buffer = crear_buffer();
    agregar_string_a_buffer(un_buffer,interfaz);
   

    if (dir_tam != NULL) {
        if (strcmp(reg_tam, "AX") == 0 || strcmp(reg_tam, "BX") == 0 ||
            strcmp(reg_tam, "CX") == 0 || strcmp(reg_tam, "DX") == 0) {
                uint8_t tamanio = *(uint8_t*) dir_tam;
                direcciones = separar_en_paginas (direccion_logica,*(int*)tamanio);
        } else {
                uint32_t tamanio = *(uint32_t*) dir_tam;
               direcciones = separar_en_paginas (direccion_logica,*(int*)tamanio);
            } 
        }

        agregar_lista_a_buffer(un_buffer,direcciones);
        t_paquete *paquete = crear_super_paquete(OP_IO_STDIN_READ, un_buffer);
        enviar_paquete(paquete, socket_kernel_dispatch);
        eliminar_paquete(paquete);
}

//IO_STDOUT_WRITE
void ejecutar_io_stdout_write(char interfaz[20], char reg_dir_logica[20], char reg_tam [20]){
    void* dir_logica = obtener_registro(reg_dir_logica); //las dl son de 32
    uint32_t direccion_logica = *(uint32_t*)dir_logica; //hago void* -> uint32_t*; y *(uint32_t*) para obtener el valor almacenado en la dirección apuntada
    t_list* direcciones;
    t_buffer *un_buffer = crear_buffer();
    agregar_string_a_buffer(un_buffer,interfaz);


    void* dir_tam = obtener_registro(reg_tam); //direccion del registro de nombre en reg_tam
    if (dir_tam != NULL) {
        if (strcmp(reg_tam, "AX") == 0 || strcmp(reg_tam, "BX") == 0 ||
            strcmp(reg_tam, "CX") == 0 || strcmp(reg_tam, "DX") == 0) {
                uint8_t tamanio = *(uint8_t*) dir_tam; //leo el contenido de dir_tam (es la direccion del registro referido por reg_tam)
                direcciones = separar_en_paginas (direccion_logica,*(int*)tamanio); 

        } else {
                uint32_t tamanio = *(uint32_t*) dir_tam;
                direcciones = separar_en_paginas (direccion_logica,*(int*)tamanio);
            } 
        }
        
        agregar_lista_a_buffer(un_buffer,direcciones);
        t_paquete *paquete = crear_super_paquete(OP_IO_STDOUT_WRITE, un_buffer);
        enviar_paquete(paquete, socket_kernel_dispatch);
        eliminar_paquete(paquete);
}

//IO_FS_CREATE
void ejecutar_io_fs_create(char interfaz[20], char nombre_fs[20]){
    t_buffer *otro_buffer = crear_buffer();
    agregar_string_a_buffer(otro_buffer,interfaz);
    agregar_string_a_buffer(otro_buffer,nombre_fs);
    t_paquete *paquete = crear_super_paquete(OP_IO_FS_CREATE, otro_buffer);
    enviar_paquete(paquete, socket_kernel_dispatch);
    eliminar_paquete(paquete);
}

//IO_FS_DELETE
void ejecutar_io_fs_delete(char interfaz[20], char nombre_fs[20]){
    t_buffer *un_buffer = crear_buffer();
    agregar_string_a_buffer(un_buffer,interfaz);
    agregar_string_a_buffer(un_buffer,nombre_fs);
    t_paquete *paquete = crear_super_paquete(OP_IO_FS_CREATE, un_buffer);
    enviar_paquete(paquete, socket_kernel_dispatch);
    eliminar_paquete(paquete);
}

//IO_FS_TRUNCATE
void ejecutar_io_fs_truncate(char interfaz[20], char nombre_fs[20], char reg_tam[20]){
    t_buffer *otro_buffer = crear_buffer();
    agregar_string_a_buffer(otro_buffer,interfaz);
    agregar_string_a_buffer(otro_buffer,nombre_fs);

    void* dir_tam = obtener_registro(reg_tam);
    if (dir_tam != NULL) {
        if (strcmp(reg_tam, "AX") == 0 || strcmp(reg_tam, "BX") == 0 ||
            strcmp(reg_tam, "CX") == 0 || strcmp(reg_tam, "DX") == 0) {
                uint8_t tamanio = *(uint8_t*) dir_tam;
                agregar_uint8_a_buffer(otro_buffer, tamanio);

        } else {
                uint32_t tamanio = *(uint32_t*) dir_tam;
                agregar_uint32_a_buffer(otro_buffer, tamanio);
            } 
        }

    t_paquete *paquete = crear_super_paquete(OP_IO_FS_TRUNCATE, otro_buffer);
    enviar_paquete(paquete, socket_kernel_dispatch);
    eliminar_paquete(paquete);
}

//IO_FS_WRITE
void ejecutar_io_fs_write(char interfaz[20], char nombre_archivo[20], char reg_dir_logica[20], char reg_tam[20], char reg_ptr[20]){
    //creo buffer
    t_buffer *un_buffer = crear_buffer();

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
    if (dir_tam != NULL) {
        if (strcmp(reg_tam, "AX") == 0 || strcmp(reg_tam, "BX") == 0 ||
            strcmp(reg_tam, "CX") == 0 || strcmp(reg_tam, "DX") == 0) {
                //si es uno de los registros anteriores, el int a agregar va a ser de uint8
                //creo la variable de dicho tamanio, q va a tener el contenido apuntado por dir_tam
                uint8_t tamanio = *(uint8_t*) dir_tam;
                direcciones_fisicas = separar_en_paginas (direccion_logica,*(int*)tamanio);

        } else {
                //si no es uno de los registros anteriores, es de los otros, cuyo tamanio es 32
                //creo la variable de tipo uint32,q va a tener el contenido apuntado por dir_tam
                uint32_t tamanio = *(uint32_t*) dir_tam;
                direcciones_fisicas = separar_en_paginas (direccion_logica,*(int*)tamanio);

            } 
        }

    agregar_lista_a_buffer(un_buffer,direcciones_fisicas);
    //obtengo dir del registro
    void* dir_ptr = obtener_registro(reg_ptr);
    if (dir_ptr != NULL) {
        if (strcmp(reg_tam, "AX") == 0 || strcmp(reg_tam, "BX") == 0 ||
            strcmp(reg_tam, "CX") == 0 || strcmp(reg_tam, "DX") == 0) {
                //obtengo el contenido de la direccion, y la agrego al buffer
                uint8_t puntero = *(uint8_t*) dir_ptr;
                agregar_uint8_a_buffer(un_buffer, puntero);
        } else {
                //idem
                uint32_t puntero = *(uint32_t*) dir_ptr;
                agregar_uint32_a_buffer(un_buffer, puntero);
            } 
        }

    t_paquete *paquete = crear_super_paquete(OP_IO_FS_WRITE, un_buffer);
    enviar_paquete(paquete, socket_kernel_dispatch);
    eliminar_paquete(paquete);
}


//IO_FS_READ
void ejecutar_io_fs_read(char interfaz[20], char nombre_archivo[20], char reg_dir_logica[20],char reg_tam[20], char reg_ptr[20]){
    t_buffer *un_buffer = crear_buffer();
    agregar_string_a_buffer(un_buffer,interfaz);
    agregar_string_a_buffer(un_buffer,nombre_archivo);
    uint32_t* direcciones_fisicas;
    void* dir_logica = obtener_registro(reg_dir_logica); 
    uint32_t direccion_logica = *(uint32_t*)dir_logica; 
    //uint32_t direccion_fisica =  traducir_direccion_logica(direccion_logica);
    //agregar_uint32_a_buffer(un_buffer, direccion_fisica);

    void* dir_tam = obtener_registro(reg_tam);
    if (dir_tam != NULL) {
        if (strcmp(reg_tam, "AX") == 0 || strcmp(reg_tam, "BX") == 0 ||
            strcmp(reg_tam, "CX") == 0 || strcmp(reg_tam, "DX") == 0) {
                uint8_t tamanio = *(uint8_t*) dir_tam;
                direcciones_fisicas = separar_en_paginas (direccion_logica,*(int*)tamanio);

        } else {
                uint32_t tamanio = *(uint32_t*) dir_tam;
                direcciones_fisicas = separar_en_paginas (direccion_logica,*(int*)tamanio);
            } 
        }

    agregar_lista_a_buffer(un_buffer,direcciones_fisicas);
    
    void* dir_ptr = obtener_registro(reg_ptr);
    if (dir_ptr != NULL) {
        if (strcmp(reg_tam, "AX") == 0 || strcmp(reg_tam, "BX") == 0 ||
            strcmp(reg_tam, "CX") == 0 || strcmp(reg_tam, "DX") == 0) {
                uint8_t puntero = *(uint8_t*) dir_ptr;
                agregar_uint8_a_buffer(un_buffer, puntero);

        } else {
                uint32_t puntero = *(uint32_t*) dir_ptr;
                agregar_uint32_a_buffer(un_buffer, puntero);
            } 
        }

    t_paquete *paquete = crear_super_paquete(OP_IO_FS_READ, un_buffer);
    enviar_paquete(paquete, socket_kernel_dispatch);
    eliminar_paquete(paquete);
}


