#include "../../inc/interrupciones.h"
#include "../../inc/pic.h"
#include "../../inc/teclado.h"
#include "../../inc/ring_buffer.h"
#include "../../inc/systick.h"
#include "../../inc/pantalla.h"
#include "../../inc/tablas_sistema.h"

extern ring_buffer_t ring_buffer;

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
__attribute__(( section(".interrupciones"), interrupt)) void Divide_Error_Handler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x0, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void Debug_Exception_Handler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x1, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void NMI_Handler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x2, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void Breakpoint_Handler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x3, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void Overflow_Handler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x4, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void BR_Handler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x5, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void Invalid_Opcode_Handler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x6, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void NM_Handler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x7, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void Double_Fault_Handler(cuadro_interrupcion_t *cuadro, uint32_t codigo_error)
{
    asm("xchg %bx, %bx");
    asm("mov $0x8, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void Invalid_TSS_Handler(cuadro_interrupcion_t *cuadro, uint32_t codigo_error)
{
    asm("xchg %bx, %bx");
    asm("mov $0xA, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void NP_Handler(cuadro_interrupcion_t *cuadro, uint32_t codigo_error)
{
    asm("xchg %bx, %bx");
    asm("mov $0xB, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void SS_Handler(cuadro_interrupcion_t *cuadro, uint32_t codigo_error)
{
    asm("xchg %bx, %bx");
    asm("mov $0xC, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void General_Protection_Handler(cuadro_interrupcion_t *cuadro, uint32_t codigo_error)
{
    asm("xchg %bx, %bx");
    asm("mov $0xD, %dl");
    asm("hlt");
}

uint8_t* get_cr2(void);

__attribute__(( section(".interrupciones"), interrupt)) void Page_Fault_Handler(cuadro_interrupcion_t *cuadro, uint32_t codigo_error)
{
    uint8_t *direccion = get_cr2();
    
    MAGIC_BREAKPOINT
    //Ingresar direcciones tipo:
    //0x54000
    //0x56000
    if((codigo_error & 0b001) == 0)
        agregar_pagina_dinamicamente(direccion);


    // asm("xchg %bx, %bx");
    // asm("mov $0xE, %dl");
    // asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void MF_Handler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x10, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void Alignment_Check_Handler(cuadro_interrupcion_t *cuadro, uint32_t codigo_error)
{
    asm("xchg %bx, %bx");
    asm("mov $0x11, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void Machine_Check_Handler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x12, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void XM_Handler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x13, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void Virtualization_Exception_Handler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x14, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void Control_Protection_Handler(cuadro_interrupcion_t *cuadro, uint32_t codigo_error)
{
    asm("xchg %bx, %bx");
    asm("mov $0x15, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void PIC0_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    //asm("xchg %bx, %bx");
    systick_incrementar_tick();
    // asm("mov $0x20, %dl");
    // asm("hlt");
    pic_limpiar_interrupcion(INTERRUPCION_SYSTICK);
}

//Teclado
__attribute__(( section(".interrupciones"), interrupt)) void PIC1_IRQHandler(cuadro_interrupcion_t *cuadro)
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

__attribute__(( section(".interrupciones"), interrupt)) void PIC2_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x22, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void PIC3_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x23, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void PIC4_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x24, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void PIC5_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x25, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void PIC6_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x26, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void PIC7_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x27, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void PIC8_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x28, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void PIC9_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x29, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void PIC10_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x2A, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void PIC11_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x2B, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void PIC12_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x2C, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void PIC13_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x2D, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void PIC14_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x2E, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"), interrupt)) void PIC15_IRQHandler(cuadro_interrupcion_t *cuadro)
{
    asm("xchg %bx, %bx");
    asm("mov $0x2F, %dl");
    asm("hlt");
}