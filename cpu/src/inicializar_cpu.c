#include "inicializar_cpu.h"

void inicializar_cpu()
{
    inicializar_logger();
    inicializar_config();
    inicializar_variables();
}

void inicializar_logger()
{
    cpu_logger = iniciar_logger("CPU.log", LOG_LEVEL_DEBUG);
}

void inicializar_config()
{
    cpu_config = iniciar_config("./CPU.config");

    // INICIALIZAR VARIABLES GLOBALES
    cantidad_entradas_tlb = config_get_int_value(cpu_config, "CANTIDAD_ENTRADAS_TLB");
    algoritmo_tlb = config_get_string_value(cpu_config, "ALGORITMO_TLB");

    // INICIALIZAR VARIABLES DE CONEXION
    ip_memoria = config_get_string_value(cpu_config, "IP_MEMORIA");
    puerto_memoria = config_get_string_value(cpu_config, "PUERTO_MEMORIA");
    puerto_dispatch = config_get_string_value(cpu_config, "PUERTO_ESCUCHA_DISPATCH");
    puerto_interrupt = config_get_string_value(cpu_config, "PUERTO_ESCUCHA_INTERRUPT");
}

void inicializar_variables(){
	// registros = inicializar_registros();
	flag_execute = true;
}

void inicializar_tlb() {
    tlb = (tlb_entrada_t*)malloc(cantidad_entradas_tlb * sizeof(tlb_entrada_t));
    tlb_proximo_reemplazo = 0; 
    tlb_entradas = 0;
    contador_acceso = 0; //para el lru
}

void inicializar_semaforos()
{
    sem_init(&sem_instruccion, 0, 0);
    sem_init(&sem_resize, 0, 0);
    sem_init(&sem_resultado_escritura, 0, 0);
    sem_init(&sem_resultado_lectura, 0, 0);
}

void borrar_semaforos()
{
    sem_destroy(&sem_instruccion);
    sem_destroy(&sem_resize);
    sem_destroy(&sem_resultado_escritura);
    sem_destroy(&sem_resultado_lectura);
}