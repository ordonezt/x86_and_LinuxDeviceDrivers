#ifndef __MAIN_H
#define __MAIN_H

#include "../inc/systick.h"

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

#define PERIODO_TAREA_1     COUNT_500ms
#define PERIODO_TAREA_2     COUNT_100ms
#define PERIODO_TAREA_3     COUNT_200ms

void scheduler(contexto_tarea_t contexto_tarea_anterior);
void ir_a_dormir(void);
void cambiar_contexto(contexto_tarea_t *tarea_futura);
void paginacion_encender(void);
void paginacion_apagar(void);

#endif /*__MAIN_H*/