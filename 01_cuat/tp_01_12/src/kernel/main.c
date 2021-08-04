#include "../../inc/teclado.h"
#include "../../inc/systick.h"
#include "../../inc/main.h"
#include "../../inc/rutinas.h"
#include "../../inc/tarea_1.h"
#include "../../inc/tarea_2.h"
#include "../../inc/tarea_3.h"
#include "../../inc/tarea_4.h"
#include "../../inc/pantalla.h"
#include "../../inc/tablas_sistema.h"
#include "../../inc/interrupciones.h"

//Vector donde guardo el contexto de las tareas
contexto_tarea_t contexto_tareas_tabla[5] = {0}; //Si son mas de 5 tareas modificar
contexto_simd_t  contexto_simd_tabla[5] = {0};//__attribute__ ((aligned(32))) = {0};
directorio_tabla_paginas_t* dtp_tareas_tabla[5] = {DTP_kernel, DTP_tarea1, DTP_tarea2, DTP_tarea3, DTP_tarea4};

__attribute__(( section(".kernel")))
int main(void)
{
    uint32_t tick;

    //Inicializaciones
    tareas_inicializar();
    teclado_inicializar();
    systick_inicializar();
    habilitar_interrupciones();

    //Loop
    while(1)
    {
        tick = systick_get_tick();
        
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
void inicializar_contexto(directorio_tabla_paginas_t *dtp, void *tarea, void *pila)
{
    uint8_t n_tarea = ((uint32_t)dtp - (uint32_t)DTP_kernel) / sizeof(directorio_tabla_paginas_t);
    contexto_tarea_t *contexto = &contexto_tareas_tabla[n_tarea];
    
    contexto->CS = CS_SELECTOR;
    contexto->DS = contexto->ES = contexto->FS = contexto->GS = contexto->SS = DS_SELECTOR;
    contexto->EBP = (uint32_t)pila;
    contexto->ESP = contexto->EBP - (3 + 4 * 2);
    contexto->EIP = (uint32_t)tarea;
    contexto->CR3 = (uint32_t)dtp;
    contexto->EFLAGS = 2;

    //TODO Inicializar el stack aca (a lo mejor apagar paginacion)
    // *(((uint32_t*)pila_fisica) + 0) = contexto->EFLAGS;
    // *(((uint32_t*)pila_fisica) - 1) = contexto->CS;
    // *(((uint32_t*)pila_fisica) - 2) = contexto->EIP;
}

__attribute__(( section(".kernel")))
void tareas_inicializar(void)
{
    inicializar_contexto(DTP_tarea1, tarea_1, __TAREA_1_PILA_INICIO_LINEAL);
    inicializar_contexto(DTP_tarea2, tarea_2, __TAREA_2_PILA_INICIO_LINEAL);
    inicializar_contexto(DTP_tarea3, tarea_3, __TAREA_3_PILA_INICIO_LINEAL);
    inicializar_contexto(DTP_tarea4, tarea_4, __TAREA_4_PILA_INICIO_LINEAL);
}

__attribute__(( section(".kernel")))
void guardar_contexto(contexto_tarea_t contexto_tarea)
{
    uint8_t aux[11], n_tarea;
    
    n_tarea = get_numero_tarea(contexto_tarea);

    __mi_memcpy(&contexto_tarea, &contexto_tareas_tabla[n_tarea], sizeof(contexto_tarea));
    guardar_registros_simd(&contexto_simd_tabla[n_tarea]);
}

__attribute__(( section(".kernel")))
void scheduler(contexto_tarea_t contexto_tarea_actual)
{
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
    

    // //TODO Limpiar y pushear
    // hex32_2_str(contexto_tareas_tabla[tarea_anterior].EFLAGS, aux);
    // my_printf((uint8_t*)"EFL: ", 11, 0);
    // my_printf(aux, 11, 5);
    // hex32_2_str(contexto_tareas_tabla[tarea_anterior].CS, aux);
    // my_printf((uint8_t*)"CS : ", 12, 0);
    // my_printf(aux, 12, 5);
    // hex32_2_str(contexto_tareas_tabla[tarea_anterior].EIP, aux);
    // my_printf((uint8_t*)"EIP: ", 13, 0);
    // my_printf(aux, 13, 5);
    // hex32_2_str(contexto_tareas_tabla[tarea_anterior].EAX, aux);
    // my_printf((uint8_t*)"EAX: ", 14, 0);
    // my_printf(aux, 14, 5);
    // hex32_2_str(contexto_tareas_tabla[tarea_anterior].ECX, aux);
    // my_printf((uint8_t*)"ECX: ", 15, 0);
    // my_printf(aux, 15, 5);
    // hex32_2_str(contexto_tareas_tabla[tarea_anterior].EDX, aux);
    // my_printf((uint8_t*)"EDX: ", 16, 0);
    // my_printf(aux, 16, 5);
    // hex32_2_str(contexto_tareas_tabla[tarea_anterior].EBX, aux);
    // my_printf((uint8_t*)"EBX: ", 17, 0);
    // my_printf(aux, 17, 5);
    // hex32_2_str(contexto_tareas_tabla[tarea_anterior].ESP, aux);
    // my_printf((uint8_t*)"ESP: ", 18, 0);
    // my_printf(aux, 18, 5);
    // hex32_2_str(contexto_tareas_tabla[tarea_anterior].EBP, aux);
    // my_printf((uint8_t*)"EBP: ", 19, 0);
    // my_printf(aux, 19, 5);
    // hex32_2_str(contexto_tareas_tabla[tarea_anterior].ESI, aux);
    // my_printf((uint8_t*)"ESI: ", 20, 0);
    // my_printf(aux, 20, 5);
    // hex32_2_str(contexto_tareas_tabla[tarea_anterior].EDI, aux);
    // my_printf((uint8_t*)"EDI: ", 21, 0);
    // my_printf(aux, 21, 5);
    // hex32_2_str(contexto_tareas_tabla[tarea_anterior].CR3, aux);
    // my_printf((uint8_t*)"CR3: ", 22, 0);
    // my_printf(aux, 22, 5);
    // hex32_2_str(contexto_tareas_tabla[tarea_anterior].SS, aux);
    // my_printf((uint8_t*)"SS : ", 23, 0);
    // my_printf(aux, 23, 5);
    // hex32_2_str(contexto_tareas_tabla[tarea_anterior].ES, aux);
    // my_printf((uint8_t*)"ES : ", 24, 0);
    // my_printf(aux, 24, 5);    

    //MAGIC_BREAKPOINT
    if(tarea_actual != tarea_siguiente)
    {
        guardar_contexto(contexto_tarea_actual);
        prender_cr0_ts();
        cambiar_contexto(&contexto_tareas_tabla[tarea_siguiente]);
    }
}

__attribute__(( section(".kernel")))
void ir_a_dormir(void)
{//MAGIC_BREAKPOINT
    asm("hlt");
}