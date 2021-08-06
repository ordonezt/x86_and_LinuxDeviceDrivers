#ifndef __INTERRUPCIONES_H
#define __INTERRUPCIONES_H

#include "../inc/my_types.h"

typedef struct {
    uint32_t flags;
    uint32_t segmento_codigo;
    uint32_t puntero_instruccion;
}cuadro_interrupcion_t;

typedef struct{
    uint32_t EXT:1 __attribute__((packed));
    uint32_t IDT:1 __attribute__((packed));
    uint32_t TI:1 __attribute__((packed));
    uint32_t selector:13 __attribute__((packed));
}codigo_error_t;

typedef enum{
    SYSCALL_HLT,
    SYSCALL_READ,
    SYSCALL_WRITE
}syscalls_t;

void Divide_Error_Handler(cuadro_interrupcion_t *cuadro);
void Debug_Exception_Handler(cuadro_interrupcion_t *cuadro);
void NMI_Handler(cuadro_interrupcion_t *cuadro);
void Breakpoint_Handler(cuadro_interrupcion_t *cuadro);
void Overflow_Handler(cuadro_interrupcion_t *cuadro);
void BR_Handler(cuadro_interrupcion_t *cuadro);
void Invalid_Opcode_Handler(cuadro_interrupcion_t *cuadro);
void NM_Handler(cuadro_interrupcion_t *cuadro);
void Double_Fault_Handler(cuadro_interrupcion_t *cuadro, uint32_t codigo_error);
void Invalid_TSS_Handler(cuadro_interrupcion_t *cuadro, uint32_t codigo_error);
void NP_Handler(cuadro_interrupcion_t *cuadro, uint32_t codigo_error);
void SS_Handler(cuadro_interrupcion_t *cuadro, uint32_t codigo_error);
void General_Protection_Handler(cuadro_interrupcion_t *cuadro, uint32_t codigo_error);
void Page_Fault_Handler(cuadro_interrupcion_t *cuadro, uint32_t codigo_error);
void MF_Handler(cuadro_interrupcion_t *cuadro);
void Alignment_Check_Handler(cuadro_interrupcion_t *cuadro, uint32_t codigo_error);
void Machine_Check_Handler(cuadro_interrupcion_t *cuadro);
void XM_Handler(cuadro_interrupcion_t *cuadro);
void Virtualization_Exception_Handler(cuadro_interrupcion_t *cuadro);
void Control_Protection_Handler(cuadro_interrupcion_t *cuadro, uint32_t codigo_error);
void PIC0_IRQHandler(cuadro_interrupcion_t *cuadro);
void PIC1_IRQHandler(cuadro_interrupcion_t *cuadro);
void PIC2_IRQHandler(cuadro_interrupcion_t *cuadro);
void PIC3_IRQHandler(cuadro_interrupcion_t *cuadro);
void PIC4_IRQHandler(cuadro_interrupcion_t *cuadro);
void PIC5_IRQHandler(cuadro_interrupcion_t *cuadro);
void PIC6_IRQHandler(cuadro_interrupcion_t *cuadro);
void PIC7_IRQHandler(cuadro_interrupcion_t *cuadro);
void PIC8_IRQHandler(cuadro_interrupcion_t *cuadro);
void PIC9_IRQHandler(cuadro_interrupcion_t *cuadro);
void PIC10_IRQHandler(cuadro_interrupcion_t *cuadro);
void PIC11_IRQHandler(cuadro_interrupcion_t *cuadro);
void PIC12_IRQHandler(cuadro_interrupcion_t *cuadro);
void PIC13_IRQHandler(cuadro_interrupcion_t *cuadro);
void PIC14_IRQHandler(cuadro_interrupcion_t *cuadro);
void PIC15_IRQHandler(cuadro_interrupcion_t *cuadro);
void INT80_IRQHandler(cuadro_interrupcion_t *cuadro);

void habilitar_interrupciones(void);
void deshabilitar_interrupciones(void);
#endif /*__INTERRUPCIONES_H*/