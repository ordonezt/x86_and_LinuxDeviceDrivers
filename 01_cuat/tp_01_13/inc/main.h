#ifndef __MAIN_H
#define __MAIN_H

#include "../inc/systick.h"
#include "../inc/tablas_sistema.h"

typedef struct{
    uint32_t EAX;
    uint32_t ECX;
    uint32_t EDX;
    uint32_t EBX;
    uint32_t ESP;
    uint32_t EBP;
    uint32_t ESI;
    uint32_t EDI;
    uint32_t ESP0;
    uint32_t SS0;
    uint32_t CR3;
    uint32_t EIP;
    uint32_t EFLAGS;
    uint32_t ES;
    uint32_t CS;
    uint32_t SS;
    uint32_t DS;
    uint32_t FS;
    uint32_t GS;
}TSS_t;

typedef struct{
    uint32_t ES;
    uint32_t GS;
    uint32_t FS;
    uint32_t DS;
    uint32_t SS;
    uint32_t CR3;
    uint32_t EDI;
    uint32_t ESI;
    uint32_t EBP;
    uint32_t ESP;
    uint32_t EBX;
    uint32_t EDX;
    uint32_t ECX;
    uint32_t EAX;
    uint32_t EIP;
    uint32_t CS;
    uint32_t EFLAGS;
}contexto_tarea_t;

typedef struct{
    uint8_t registros[512];// __attribute__ ((aligned(16)));
}contexto_simd_t;

#define PERIODO_TAREA_1     COUNT_500ms
#define PERIODO_TAREA_2     COUNT_100ms
#define PERIODO_TAREA_3     COUNT_200ms

void* get_cr3(void);
void borrar_cr0_ts(void);
void prender_cr0_ts(void);
void habilitar_proteccion_alineacion(void);
uint8_t get_numero_tarea_actual(void);
void guardar_registros_simd(contexto_simd_t *contexto);
void restaurar_registros_simd(contexto_simd_t *contexto);
void scheduler(contexto_tarea_t contexto_tarea_anterior);
void ir_a_dormir(void);
void cambiar_contexto(contexto_tarea_t *tarea_futura, directorio_tabla_paginas_t *dtp);
void paginacion_encender(void);
void paginacion_apagar(void);
void tareas_inicializar(void);
void habilitar_TSS(uint32_t selector_TSS);
uint32_t syscall(syscalls_t tipo, uint32_t arg1, uint32_t arg2, uint32_t arg3);
#endif /*__MAIN_H*/