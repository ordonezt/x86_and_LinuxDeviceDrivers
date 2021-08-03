#include "../../inc/interrupciones.h"
#include "../../inc/pic.h"
#include "../../inc/teclado.h"
#include "../../inc/ring_buffer.h"
#include "../../inc/systick.h"
#include "../../inc/pantalla.h"
#include "../../inc/tablas_sistema.h"
#include "../../inc/main.h"

extern ring_buffer_t ring_buffer;
extern contexto_simd_t  contexto_simd_tabla[5];

/* Lo ideal seria hacer handlers de assembler que llamen handlers de c, por ejemplo

Divide_Error_Handler_asm:
    
    push    parametros
    call    Divide_Error_Handler_c
    add     esp, tamaño de parametros

    iret

Para lograr que el handler en C haga directamente el iret y acomode la pila hay que
ponerle el atributo interrupt, con los parametros correctos de funcion y compilar con -mgeneral-regs-only
*/

//https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html#Extended-Asm
__attribute__(( section(".interrupciones"), interrupt))
void Divide_Error_Handler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x0, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void Debug_Exception_Handler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x1, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void NMI_Handler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x2, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void Breakpoint_Handler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x3, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void Overflow_Handler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x4, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void BR_Handler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x5, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void Invalid_Opcode_Handler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x6, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void NM_Handler(cuadro_interrupcion_t *cuadro)
{
    /**
     * Se invoca al querer modificar un registro simd luego de un cambio de contexto.
     * Tengo que restaurar el contexto simd
     * */
    // asm("xchg %bx, %bx");
    // asm("mov $0x7, %dl");
    // asm("hlt");
    //MAGIC_BREAKPOINT
    uint8_t n_tarea = get_numero_tarea_actual();
    borrar_cr0_ts();
    restaurar_registros_simd(&contexto_simd_tabla[n_tarea]);   
}

__attribute__(( section(".interrupciones"), interrupt))
void Double_Fault_Handler(cuadro_interrupcion_t *cuadro, uint32_t codigo_error)
{
    asm("xchg %bx, %bx");
    asm("mov $0x8, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void Invalid_TSS_Handler(cuadro_interrupcion_t *cuadro, uint32_t codigo_error)
{
    asm("xchg %bx, %bx");
    asm("mov $0xA, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void NP_Handler(cuadro_interrupcion_t *cuadro, uint32_t codigo_error)
{
    asm("xchg %bx, %bx");
    asm("mov $0xB, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void SS_Handler(cuadro_interrupcion_t *cuadro, uint32_t codigo_error)
{
    asm("xchg %bx, %bx");
    asm("mov $0xC, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void General_Protection_Handler(cuadro_interrupcion_t *cuadro, uint32_t codigo_error)
{
    asm("xchg %bx, %bx");
    asm("mov $0xD, %dl");
    asm("hlt");
}

uint8_t* get_cr2(void);

__attribute__(( section(".interrupciones"), interrupt))
void Page_Fault_Handler(cuadro_interrupcion_t *cuadro, uint32_t codigo_error)
{
    //uint8_t *direccion = get_cr2();
    
    // if((codigo_error & 0b001) == 0)
    //     agregar_pagina_dinamicamente(direccion);


    // asm("xchg %bx, %bx");
    // asm("mov $0xE, %dl");
    // asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void MF_Handler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x10, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void Alignment_Check_Handler(cuadro_interrupcion_t *cuadro, uint32_t codigo_error)
{
    asm("xchg %bx, %bx");
    asm("mov $0x11, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void Machine_Check_Handler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x12, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void XM_Handler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x13, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void Virtualization_Exception_Handler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x14, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void Control_Protection_Handler(cuadro_interrupcion_t *cuadro, uint32_t codigo_error)
{
    asm("xchg %bx, %bx");
    asm("mov $0x15, %dl");
    asm("hlt");
}

//Este handler es invocado por un wrapper de asm, porque el bendito gcc me pone codigo en el handler de C sin que yo pueda meter mano
//Y como tengo que guardar el contexto me hace pelota todo
__attribute__(( section(".interrupciones")))
void PIC0_IRQHandler_c(contexto_tarea_t contexto_tarea_anterior)
{
    //MAGIC_BREAKPOINT
    //asm("xchg %bx, %bx");
    //     uint8_t aux[9];

    //TODO Limpiar y pushear
    
    // hex32_2_str(contexto_tarea_anterior.EFLAGS, aux);
    // my_printf("EFL: ", 11, 0);
    // my_printf(aux, 11, 5);
    // hex32_2_str(contexto_tarea_anterior.CS, aux);
    // my_printf("CS : ", 12, 0);
    // my_printf(aux, 12, 5);
    // hex32_2_str(contexto_tarea_anterior.EIP, aux);
    // my_printf("EIP: ", 13, 0);
    // my_printf(aux, 13, 5);
    // hex32_2_str(contexto_tarea_anterior.EAX, aux);
    // my_printf("EAX: ", 14, 0);
    // my_printf(aux, 14, 5);
    // hex32_2_str(contexto_tarea_anterior.ECX, aux);
    // my_printf("ECX: ", 15, 0);
    // my_printf(aux, 15, 5);
    // hex32_2_str(contexto_tarea_anterior.EDX, aux);
    // my_printf("EDX: ", 16, 0);
    // my_printf(aux, 16, 5);
    // hex32_2_str(contexto_tarea_anterior.EBX, aux);
    // my_printf("EBX: ", 17, 0);
    // my_printf(aux, 17, 5);
    // hex32_2_str(contexto_tarea_anterior.ESP, aux);
    // my_printf("ESP: ", 18, 0);
    // my_printf(aux, 18, 5);
    // hex32_2_str(contexto_tarea_anterior.EBP, aux);
    // my_printf("EBP: ", 19, 0);
    // my_printf(aux, 19, 5);
    // hex32_2_str(contexto_tarea_anterior.ESI, aux);
    // my_printf("ESI: ", 20, 0);
    // my_printf(aux, 20, 5);
    // hex32_2_str(contexto_tarea_anterior.EDI, aux);
    // my_printf("EDI: ", 21, 0);
    // my_printf(aux, 21, 5);
    // hex32_2_str(contexto_tarea_anterior.CR3, aux);
    // my_printf("CR3: ", 22, 0);
    // my_printf(aux, 22, 5);
    // hex32_2_str(contexto_tarea_anterior.SS, aux);
    // my_printf("SS : ", 23, 0);
    // my_printf(aux, 23, 5);
    // hex32_2_str(contexto_tarea_anterior.ES, aux);
    // my_printf("ES : ", 24, 0);
    // my_printf(aux, 24, 5);

    systick_incrementar_tick();
    pic_limpiar_interrupcion(INTERRUPCION_SYSTICK);
    // MAGIC_BREAKPOINT
    scheduler(contexto_tarea_anterior);
    // asm("mov $0x20, %dl");
    // asm("hlt");
}

//Teclado
__attribute__(( section(".interrupciones"), interrupt))
void PIC1_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    uint8_t tecla = teclado_get_tecla();

    if(tecla_es_make_code(tecla))
    {
        tecla = tecla2caracter(tecla);

        if(tecla == '\n' || ring_buffer_esta_lleno(&ring_buffer))
        {
            insertar_en_tabla_digitos(ring_buffer.buffer, ring_buffer_get_ocupado(&ring_buffer));
            ring_buffer_vaciar(&ring_buffer);
        }
        else if(caracter_es_numero(tecla))
        {
            //asm("xchg %bx, %bx");
            ring_buffer_insertar(&ring_buffer, tecla);
        }
    }
 
    //asm("mov $0x21, %dl");
    //asm("hlt");
    pic_limpiar_interrupcion(INTERRUPCION_TECLADO);
}

__attribute__(( section(".interrupciones"), interrupt))
void PIC2_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x22, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void PIC3_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x23, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void PIC4_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x24, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void PIC5_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x25, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void PIC6_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x26, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void PIC7_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x27, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void PIC8_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x28, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void PIC9_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x29, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void PIC10_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x2A, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void PIC11_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x2B, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void PIC12_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x2C, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void PIC13_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x2D, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void PIC14_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x2E, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt))
void PIC15_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x2F, %dl");
    asm("hlt");
}