#include "../inc/my_types.h"
#include "../inc/interrupciones.h"

#define LIMITE_MAXIMO   0xFFFFFFFF
//extern uint32_t __CS_BASE, __CS_LIMITE, __DS_BASE, __DS_LIMITE, __SS_BASE, __SS_LIMITE;
#define __CS_BASE        0x00000000
#define __CS_LIMITE      0xFFFFFFFF
#define __DS_BASE        __CS_BASE
#define __DS_LIMITE      __CS_LIMITE
#define __SS_BASE        __CS_BASE
#define __SS_LIMITE      __CS_LIMITE

#define DS_SEL_32   0b10000

typedef enum{
    TIPO_TAREA          = 0b101,
    TIPO_INTERRUPCION   = 0b110,
    TIPO_EXCEPCION      = 0b111
}tipo_excepcion_t;

// typedef struct{
//     uint16_t limite_0;
//     uint32_t base_0  :24;
//     uint32_t A       :1;
//     union{
//         uint32_t W   :1;
//         uint32_t R   :1;
//     };
//     union{
//         uint32_t ED  :1;
//         uint32_t C   :1;
//     };
//     uint32_t bit_11  :1;
//     uint32_t S       :1;
//     uint32_t DPL     :2;
//     uint32_t P       :1;
//     uint8_t limite_1:4;
//     uint8_t AVL:1;
//     uint8_t L:1;
//     uint8_t D_B:1;
//     uint8_t G:1;
//     uint8_t base_1;
// }descriptor_segmento_t;

typedef struct{
    uint16_t    limite_0;
    uint16_t    base_0;
    uint8_t     base_1;
    uint8_t     A       :1;
    uint8_t     W_R     :1;
    uint8_t     ED_C    :1;
    uint8_t     bit_11  :1;
    uint8_t     S       :1;
    uint8_t     DPL     :2;
    uint8_t     P       :1;
    uint8_t     limite_1:4;
    uint8_t     AVL     :1;
    uint8_t     L       :1;
    uint8_t     D_B     :1;
    uint8_t     G       :1;
    uint8_t     base_2;
}descriptor_segmento_t;

typedef struct{
    uint16_t    offset_0;
    uint16_t    sel_segmento;
    uint8_t     ceros;
    uint8_t     tipo    :3;
    uint8_t     D      :1;
    uint8_t     S       :1;
    uint8_t     DPL     :2;
    uint8_t     P       :1;
    uint16_t    offset_1;
}descriptor_segmento_int_t;

typedef struct{
    uint32_t campo_0;
    uint32_t campo_1;
}registro_descriptor_segmento_t;

extern registro_descriptor_segmento_t* _gdtr;

extern descriptor_segmento_t *gdt_nulo, *cs_gdt, *ds_gdt;//, *ss_gdt;
extern int *ss_gdt;
//#define ss_gdt  ((uint32_t *)0xFFFFF500)

__attribute__(( section(".rutinas"))) void cargar_descriptor_segmento(   descriptor_segmento_t *descriptor,
                                                                            uint32_t base, uint32_t limite, uint8_t granularidad,
                                                                            uint8_t default_big, uint8_t L, uint8_t AVL,
                                                                            uint8_t presente, uint8_t DPL, uint8_t sistema,
                                                                            uint8_t b11, uint8_t ED_C, uint8_t W_R,
                                                                            uint8_t accedido)
{
    descriptor->base_0 = (base >> 0 )    & 0x0000FFFF;
    descriptor->base_1 = (base >> 16)    & 0xFF;
    descriptor->base_2 = (base >> 24)    & 0xFF;

    descriptor->limite_0 = (limite >> 0 )    & 0x0000FFFF;
    descriptor->limite_1 = (limite >> 16)    & 0x0000000F;
    
    descriptor->A = accedido;
    
    descriptor->bit_11 = b11;
    descriptor->W_R   = W_R;
    descriptor->ED_C  = ED_C;
    
    descriptor->S   = sistema;
    descriptor->DPL = DPL;
    descriptor->P   = presente;
    descriptor->AVL = AVL;
    descriptor->L   = L;
    descriptor->D_B = default_big;
    descriptor->G   = granularidad;
}

__attribute__(( section(".rutinas"))) void cargar_gdt(descriptor_segmento_t GDT[], registro_descriptor_segmento_t *gdtr)
{
    uint16_t limite;

    //Descriptor nulo
    cargar_descriptor_segmento(&GDT[0], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    //cargar_descriptor_segmento(&GDT[0], 0xFFFFFFFF, 0xFFFFFFFF, 0x0, 0x0, 0x0, 0x0, 0x0, 0, 0, 0, 0, 0, 0);

    //Descriptor de codigo
    cargar_descriptor_segmento(&GDT[1], __CS_BASE, __CS_LIMITE, 1, 1, 0, 0, 1, 00, 1, 1, 0, 0, 1);

    //Descriptor de datos
    cargar_descriptor_segmento(&GDT[2], __DS_BASE, __DS_LIMITE,
                                                                1, //G
                                                                1, //D/B
                                                                0, //L
                                                                0, //AVL
                                                                1, //P
                                                                00, //DPL
                                                                1, //S
                                                                0, //B11
                                                                0, //ED/C
                                                                1, //W/R
                                                                1);//A

    //Descriptor de pila
    cargar_descriptor_segmento(&GDT[3], __SS_BASE, __SS_LIMITE, 1, 1, 0, 0, 1, 00, 1, 0, 0, 1, 1);

    limite =        sizeof(GDT[0])  + 
                    sizeof(GDT[1])  + 
                    sizeof(GDT[2])  + 
                    sizeof(GDT[3]) 
                    - 1;
    
    gdtr->campo_0 = limite;
    gdtr->campo_0 |= ((uint32_t)GDT & 0xFFFF) << 16;
    gdtr->campo_1 = ((uint32_t)GDT & 0xFFFF0000) >> 16;
    // gdtr->base = (uint32_t)GDT;
    // gdtr->base = 0x12345678;
    // gdtr->base =    ((((uint32_t)GDT >> 0)          & 0xFF) << (32-8))  | 
    //                 ((((uint32_t)GDT >> (32-24))    & 0xFF) << (32-16)) | 
    //                 ((((uint32_t)GDT >> (32-16))    & 0xFF) << (32-24)) | 
    //                  (((uint32_t)GDT >> (32-8))     & 0xFF);

    //asm("lgdt   %[_gdtr]"::[_gdtr] "m" (_gdtr));
}

__attribute__(( section(".rutinas"))) void forzar_SS(descriptor_segmento_t GDT[])
{
    //Descriptor de datos
    cargar_descriptor_segmento(&GDT[2], __DS_BASE, __DS_LIMITE,
                                                                1, //G
                                                                1, //D/B
                                                                0, //L
                                                                0, //AVL
                                                                0, //No presente
                                                                00, //DPL
                                                                1, //S
                                                                0, //B11
                                                                0, //ED/C
                                                                1, //W/R
                                                                1);//A

    asm("mov $0b10000, %eax"); //DS_SEL_32
    asm("mov %eax, %ss");
}

__attribute__(( section(".rutinas"))) void cargar_descriptor_segmento_int(  descriptor_segmento_int_t *descriptor,
                                                                            uint16_t selector, uint32_t offset, tipo_excepcion_t tipo,
                                                                            uint8_t D, uint8_t presente, uint8_t DPL)
{
    descriptor->offset_0        = (offset >> 0 ) & 0x0000FFFF;
    descriptor->offset_1        = (offset >> 16) & 0xFFFF;
    descriptor->sel_segmento    = selector;
    descriptor->ceros           = 0;
    descriptor->tipo            = tipo;
    descriptor->D               = D;
    descriptor->S               = 0;
    descriptor->DPL             = DPL;
    descriptor->P               = presente;
}
__attribute__(( section(".rutinas"))) void cargar_idt(descriptor_segmento_int_t IDT[], registro_descriptor_segmento_t *idtr, uint16_t selector)
{
    uint16_t limite;

    cargar_descriptor_segmento_int( &IDT[0], 
                                    selector,
                                    (uint32_t)Divide_Error_Handler,
                                    TIPO_EXCEPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );
    //Probando doble falta, comentar cuando quieras que ande bien
    cargar_descriptor_segmento_int( &IDT[0], 
                                    0,
                                    (uint32_t)Divide_Error_Handler,
                                    TIPO_EXCEPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    cargar_descriptor_segmento_int( &IDT[1], 
                                    selector,
                                    (uint32_t)Debug_Exception_Handler,
                                    TIPO_EXCEPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );
    
    cargar_descriptor_segmento_int( &IDT[2], 
                                    selector,
                                    (uint32_t)NMI_Handler,
                                    TIPO_INTERRUPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    cargar_descriptor_segmento_int( &IDT[3], 
                                    selector,
                                    (uint32_t)Breakpoint_Handler,
                                    TIPO_EXCEPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    cargar_descriptor_segmento_int( &IDT[4], 
                                    selector,
                                    (uint32_t)Overflow_Handler,
                                    TIPO_EXCEPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    cargar_descriptor_segmento_int( &IDT[5], 
                                    selector,
                                    (uint32_t)BR_Handler,
                                    TIPO_EXCEPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );
    
    cargar_descriptor_segmento_int( &IDT[6], 
                                    selector,
                                    (uint32_t)Invalis_Opcode_Handler,
                                    TIPO_EXCEPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    cargar_descriptor_segmento_int( &IDT[7], 
                                    selector,
                                    (uint32_t)NM_Handler,
                                    TIPO_EXCEPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    cargar_descriptor_segmento_int( &IDT[8], 
                                    selector,
                                    (uint32_t)Double_Fault_Handler,
                                    TIPO_EXCEPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );


    //Coprocesor Segment Overrun (Reservada)
    
    
    cargar_descriptor_segmento_int( &IDT[10], 
                                    selector,
                                    (uint32_t)Invalid_TSS_Handler,
                                    TIPO_EXCEPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    cargar_descriptor_segmento_int( &IDT[11], 
                                    selector,
                                    (uint32_t)NP_Handler,
                                    TIPO_EXCEPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    cargar_descriptor_segmento_int( &IDT[12], 
                                    selector,
                                    (uint32_t)SS_Handler,
                                    TIPO_EXCEPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    cargar_descriptor_segmento_int( &IDT[13], 
                                    selector,
                                    (uint32_t)General_Protection_Handler,
                                    TIPO_EXCEPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );
    
    cargar_descriptor_segmento_int( &IDT[14], 
                                    selector,
                                    (uint32_t)Page_Fault_Handler,
                                    TIPO_EXCEPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    
    //Reservada
    

    cargar_descriptor_segmento_int( &IDT[16], 
                                    selector,
                                    (uint32_t)MF_Handler,
                                    TIPO_EXCEPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    cargar_descriptor_segmento_int( &IDT[17], 
                                    selector,
                                    (uint32_t)Alignment_Check_Handler,
                                    TIPO_EXCEPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );
    
    cargar_descriptor_segmento_int( &IDT[18], 
                                    selector,
                                    (uint32_t)Machine_Check_Handler,
                                    TIPO_EXCEPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    cargar_descriptor_segmento_int( &IDT[19], 
                                    selector,
                                    (uint32_t)XM_Handler,
                                    TIPO_EXCEPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    cargar_descriptor_segmento_int( &IDT[20], 
                                    selector,
                                    (uint32_t)Virtualization_Exception_Handler,
                                    TIPO_EXCEPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    cargar_descriptor_segmento_int( &IDT[21], 
                                    selector,
                                    (uint32_t)Control_Protection_Handler,
                                    TIPO_EXCEPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );
    

    /* Espacio reservado: Inidice 22 a 31 */
    
    
    cargar_descriptor_segmento_int( &IDT[32], 
                                    selector,
                                    (uint32_t)PIC0_IRQHandler,
                                    TIPO_INTERRUPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    cargar_descriptor_segmento_int( &IDT[33], 
                                    selector,
                                    (uint32_t)PIC1_IRQHandler,
                                    TIPO_INTERRUPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    cargar_descriptor_segmento_int( &IDT[34], 
                                    selector,
                                    (uint32_t)PIC2_IRQHandler,
                                    TIPO_INTERRUPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    cargar_descriptor_segmento_int( &IDT[35], 
                                    selector,
                                    (uint32_t)PIC3_IRQHandler,
                                    TIPO_INTERRUPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    cargar_descriptor_segmento_int( &IDT[36], 
                                    selector,
                                    (uint32_t)PIC4_IRQHandler,
                                    TIPO_INTERRUPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    cargar_descriptor_segmento_int( &IDT[37], 
                                    selector,
                                    (uint32_t)PIC5_IRQHandler,
                                    TIPO_INTERRUPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    cargar_descriptor_segmento_int( &IDT[38], 
                                    selector,
                                    (uint32_t)PIC6_IRQHandler,
                                    TIPO_INTERRUPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    cargar_descriptor_segmento_int( &IDT[39], 
                                    selector,
                                    (uint32_t)PIC7_IRQHandler,
                                    TIPO_INTERRUPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    cargar_descriptor_segmento_int( &IDT[40], 
                                    selector,
                                    (uint32_t)PIC8_IRQHandler,
                                    TIPO_INTERRUPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    cargar_descriptor_segmento_int( &IDT[41], 
                                    selector,
                                    (uint32_t)PIC9_IRQHandler,
                                    TIPO_INTERRUPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    cargar_descriptor_segmento_int( &IDT[42], 
                                    selector,
                                    (uint32_t)PIC10_IRQHandler,
                                    TIPO_INTERRUPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    cargar_descriptor_segmento_int( &IDT[43], 
                                    selector,
                                    (uint32_t)PIC11_IRQHandler,
                                    TIPO_INTERRUPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    cargar_descriptor_segmento_int( &IDT[44], 
                                    selector,
                                    (uint32_t)PIC12_IRQHandler,
                                    TIPO_INTERRUPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    cargar_descriptor_segmento_int( &IDT[45], 
                                    selector,
                                    (uint32_t)PIC13_IRQHandler,
                                    TIPO_INTERRUPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    cargar_descriptor_segmento_int( &IDT[46], 
                                    selector,
                                    (uint32_t)PIC14_IRQHandler,
                                    TIPO_INTERRUPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    cargar_descriptor_segmento_int( &IDT[47], 
                                    selector,
                                    (uint32_t)PIC15_IRQHandler,
                                    TIPO_INTERRUPCION, //Tipo
                                    1, //D
                                    1, //P
                                    0 //DPL
                                    );

    limite = sizeof(IDT[0]) * (47 + 1) - 1;
    
    idtr->campo_0 = limite;
    idtr->campo_0 |= ((uint32_t)IDT & 0xFFFF) << 16;
    idtr->campo_1 = ((uint32_t)IDT & 0xFFFF0000) >> 16;
}