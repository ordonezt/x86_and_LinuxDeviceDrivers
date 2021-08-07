#include "../../inc/teclado.h"
#include "../../inc/systick.h"
#include "../../inc/main.h"
#include "../../inc/rutinas.h"
#include "../../inc/tarea_1.h"
#include "../../inc/tarea_2.h"
#include "../../inc/tarea_3.h"
#include "../../inc/tarea_4.h"
#include "../../inc/pantalla.h"
//#include "../../inc/tablas_sistema.h"
#include "../../inc/interrupciones.h"

typedef struct{
    uint32_t tarea_previa;
    uint32_t ESP_0;
    uint32_t SS_0;
    uint32_t ESP_1;
    uint32_t SS_1;
    uint32_t ESP_2;
    uint32_t SS_2;
    uint32_t CR3;
    uint32_t EIP;
    uint32_t EFLAGS;
    uint32_t EAX;
    uint32_t ECX;
    uint32_t EDX;
    uint32_t EBX;
    uint32_t ESP;
    uint32_t EBP;
    uint32_t ESI;
    uint32_t EDI;
    uint32_t ES;
    uint32_t CS;
    uint32_t SS;
    uint32_t DS;
    uint32_t FS;
    uint32_t GS;
    uint32_t LDT_SELECTOR;
    uint32_t MAP_IO;
}TSS_basica_t;

//Vector donde guardo el contexto de las tareas
contexto_tarea_t* contexto_tareas_tabla[5] = {contexto_tarea_0, contexto_tarea_1, contexto_tarea_2, contexto_tarea_3, contexto_tarea_4}; //Si son mas de 5 tareas modificar
TSS_basica_t* TSS_basica_tabla[5] = {NULL, (TSS_basica_t *)__TSS_BASICA_T1_FISICA, (TSS_basica_t *)__TSS_BASICA_T2_FISICA, (TSS_basica_t *)__TSS_BASICA_T3_FISICA, (TSS_basica_t *)__TSS_BASICA_T4_FISICA}; //Si son mas de 5 tareas modificar
contexto_simd_t*  contexto_simd_tabla[5] = {contexto_simd_tarea_0, contexto_simd_tarea_1, contexto_simd_tarea_2, contexto_simd_tarea_3, contexto_simd_tarea_4};//__attribute__ ((aligned(32))) = {0};
directorio_tabla_paginas_t* dtp_tareas_tabla[5] = {DTP_kernel, DTP_tarea1, DTP_tarea2, DTP_tarea3, DTP_tarea4};

__attribute__(( section(".kernel")))
int main(void)
{
    //uint32_t tick;

    //Inicializaciones
    tareas_inicializar();
    teclado_inicializar();
    systick_inicializar();
    habilitar_interrupciones();

    //Loop
    while(1)
    {
        // tick = systick_get_tick();
        
        // if((tick % PERIODO_TAREA_1) == 0)
        //     tarea_1();

        // if((tick % PERIODO_TAREA_2) == 0)
        //     tarea_2();

        // if((tick % PERIODO_TAREA_3) == 0)
        //     tarea_3();

        // tarea_4();
    }
}

__attribute__(( section(".kernel")))
uint8_t get_numero_tarea(contexto_tarea_t contexto)
{
    return (contexto.CR3 - (uint32_t)DTP_kernel) / sizeof(directorio_tabla_paginas_t);
}

__attribute__(( section(".kernel")))
uint8_t get_numero_tarea_actual(void)
{
    return ((uint32_t)get_cr3() - (uint32_t)DTP_kernel) / sizeof(directorio_tabla_paginas_t);
}

__attribute__(( section(".kernel")))
void inicializar_contexto(directorio_tabla_paginas_t *dtp, void *tarea, void *pila_supervisor, void *pila_usuario)
{
    uint8_t n_tarea = ((uint32_t)dtp - (uint32_t)DTP_kernel) / sizeof(directorio_tabla_paginas_t);
    uint32_t *aux;
    contexto_tarea_t *contexto = contexto_tareas_tabla[n_tarea];
    TSS_basica_t *TSS = TSS_basica_tabla[n_tarea];

    aux = (uint32_t*)((uint8_t*)pila_supervisor - 3);
    *(aux--) = DS3_SELECTOR;//SS3
    *(aux--) = (uint32_t)pila_usuario - 3;//ESP3
    *(aux--) = 0x202;//EFLAGS
    *(aux--) = CS3_SELECTOR;//contexto->CS;
    *(aux) = (uint32_t)tarea; //EIP

    contexto->DS = contexto->ES = contexto->FS = contexto->GS = contexto->SS = DS3_SELECTOR-3;
    contexto->EBP = (uint32_t)pila_usuario - 3;
    contexto->ESP = (uint32_t)aux;
    contexto->EIP = (uint32_t)tarea;
    contexto->CR3 = (uint32_t)dtp;

    TSS->CR3 = (uint32_t)dtp;
    TSS->ESP_0 = (uint32_t)aux;
    TSS->SS_0 = DS0_SELECTOR;
}

__attribute__(( section(".kernel")))
void tareas_inicializar(void)
{
    inicializar_contexto(DTP_tarea1, tarea_1, __TAREA_1_PILA_SUPERVISOR_INICIO_LINEAL, __TAREA_1_PILA_USUARIO_INICIO_LINEAL);
    inicializar_contexto(DTP_tarea2, tarea_2, __TAREA_2_PILA_SUPERVISOR_INICIO_LINEAL, __TAREA_2_PILA_USUARIO_INICIO_LINEAL);
    inicializar_contexto(DTP_tarea3, tarea_3, __TAREA_3_PILA_SUPERVISOR_INICIO_LINEAL, __TAREA_3_PILA_USUARIO_INICIO_LINEAL);
    inicializar_contexto(DTP_tarea4, tarea_4, __TAREA_4_PILA_SUPERVISOR_INICIO_LINEAL, __TAREA_4_PILA_USUARIO_INICIO_LINEAL);
//MAGIC_BREAKPOINT
    habilitar_TSS(TSS_SELECTOR);
}

__attribute__(( section(".kernel")))
void guardar_contexto(contexto_tarea_t contexto_tarea)
{
    uint8_t n_tarea;
    
    n_tarea = get_numero_tarea(contexto_tarea);

    __mi_memcpy(&contexto_tarea, contexto_tareas_tabla[n_tarea], sizeof(contexto_tarea));
    guardar_registros_simd(contexto_simd_tabla[n_tarea]);
}

__attribute__(( section(".kernel")))
void scheduler(contexto_tarea_t contexto_tarea_actual)
{
    //MAGIC_BREAKPOINT
    uint8_t aux[11], tarea_actual;
    static uint8_t tarea_siguiente=0;
    static uint32_t cuenta_tarea_1 = PERIODO_TAREA_1,
                    cuenta_tarea_2 = PERIODO_TAREA_2,
                    cuenta_tarea_3 = PERIODO_TAREA_3;
    static uint32_t tick_anterior;
    uint32_t tick_actual = systick_get_tick();

    if(tick_actual - tick_anterior)
    {
        if(cuenta_tarea_1) cuenta_tarea_1--;
        if(cuenta_tarea_2) cuenta_tarea_2--;
        if(cuenta_tarea_3) cuenta_tarea_3--;
    }

    if(cuenta_tarea_1 == 0)
    {
        tarea_siguiente = 1;
        cuenta_tarea_1 = PERIODO_TAREA_1;
    }
    else if(cuenta_tarea_2 == 0)
    {
        tarea_siguiente = 2;
        cuenta_tarea_2 = PERIODO_TAREA_2;
    }
    else if(cuenta_tarea_3 == 0)
    {
        tarea_siguiente = 3;
        cuenta_tarea_3 = PERIODO_TAREA_3;
    }
    else
        tarea_siguiente = 4;

    // tarea_siguiente++;
    // tarea_siguiente %= 5;

    //MAGIC_BREAKPOINT
    tarea_actual = get_numero_tarea(contexto_tarea_actual);
    
    hex32_2_str(tarea_actual, aux);
    my_printf((uint8_t*)"Tarea interrumpida: ", 11, 40);
    my_printf(aux, 11, 40 + 21);  

    //MAGIC_BREAKPOINT
    if(tarea_actual != tarea_siguiente)
    {
        if(tarea_actual != 0)
        {
            guardar_contexto(contexto_tarea_actual);
            prender_cr0_ts();
        }
        //MAGIC_BREAKPOINT
        cambiar_contexto(contexto_tareas_tabla[tarea_siguiente],
                         dtp_tareas_tabla[tarea_siguiente]);
    }
}

__attribute__(( section(".kernel")))
void ir_a_dormir(void)
{
    asm("sti");
    asm("hlt");
}