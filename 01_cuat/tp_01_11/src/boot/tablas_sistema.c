#include "../../inc/interrupciones.h"

#define LIMITE_MAXIMO   0xFFFFFFFF
//extern uint32_t __CS_BASE, __CS_LIMITE, __DS_BASE, __DS_LIMITE, __SS_BASE, __SS_LIMITE;
#define __CS_BASE        0x00000000
#define __CS_LIMITE      0xFFFFFFFF
#define __DS_BASE        __CS_BASE
#define __DS_LIMITE      __CS_LIMITE
#define __SS_BASE        __CS_BASE
#define __SS_LIMITE      __CS_LIMITE


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


typedef uint32_t descriptor_pagina;
typedef uint32_t descriptor_tabla_pagina;

typedef struct{
    descriptor_pagina descriptores_paginas[1024];
}tabla_paginas_t;

typedef struct{
    descriptor_tabla_pagina descriptores_tablas_pagina[1024];
}directorio_tabla_paginas_t;

//Valores que vienen del linkerscript
extern uint8_t __PAGINACION_DINAMICA_BASE_FISICA[];

extern uint8_t __TABLAS_SISTEMA_VMA_LINEAL[];
extern uint8_t __tablas_sistema_size[];
extern uint8_t __TABLAS_PAGINACION_VMA_LINEAL[];
extern uint8_t __tablas_paginacion_size[];
extern uint8_t __RUTINAS_VMA_LINEAL[];
extern uint8_t __rutinas_size[];
extern uint8_t __VIDEO_VMA_LINEAL[];
extern uint8_t __video_size[];
extern uint8_t __INTERRUPCIONES_VMA_LINEAL[];
extern uint8_t __interrupciones_size[];
extern uint8_t __TABLA_DIGITOS_VMA_LINEAL[];
extern uint8_t __tabla_digitos_size[];
extern uint8_t __DATOS_VMA_LINEAL[];
extern uint8_t __datos_size[];
extern uint8_t __KERNEL_VMA_LINEAL[];
extern uint8_t __kernel_size[];
extern uint8_t __TAREA_1_VMA_LINEAL[];
extern uint8_t __TAREA_1_TEXT_VMA_LINEAL[];
extern uint8_t __tarea_1_text_size[];
extern uint8_t __TAREA_1_BSS_VMA_LINEAL[];
extern uint8_t __tarea_1_bss_size[];
extern uint8_t __TAREA_1_DATA_VMA_LINEAL[];
extern uint8_t __tarea_1_data_size[];
extern uint8_t __TAREA_1_RODATA_VMA_LINEAL[];
extern uint8_t __tarea_1_rodata_size[];
extern uint8_t __KERNEL_PILA_FINAL_LINEAL[];
extern uint8_t __TAREA_1_PILA_FINAL_LINEAL[];
extern uint8_t __ROM_VMA_LINEAL[];
extern uint8_t __RUTINAS_ROM_VMA_LINEAL[];
extern uint8_t __TABLAS_SISTEMA_16_VMA_LINEAL[];
extern uint8_t __RESET_VMA_LINEAL[];

extern uint8_t __TABLAS_SISTEMA_VMA_FISICA[];
extern uint8_t __TABLAS_PAGINACION_VMA_FISICA[];
extern uint8_t __RUTINAS_VMA_FISICA[];
extern uint8_t __VIDEO_VMA_FISICA[];
extern uint8_t __INTERRUPCIONES_VMA_FISICA[];
extern uint8_t __TABLA_DIGITOS_VMA_FISICA[];
extern uint8_t __DATOS_VMA_FISICA[];
extern uint8_t __KERNEL_VMA_FISICA[];
extern uint8_t __TAREA_1_VMA_FISICA[];
extern uint8_t __TAREA_1_TEXT_VMA_FISICA[];
extern uint8_t __TAREA_1_BSS_VMA_FISICA[];
extern uint8_t __TAREA_1_DATA_VMA_FISICA[];
extern uint8_t __TAREA_1_RODATA_VMA_FISICA[];
extern uint8_t __KERNEL_PILA_FINAL_FISICA[];
extern uint8_t __TAREA_1_PILA_FINAL_FISICA[];
extern uint8_t __ROM_VMA_FISICA[];
extern uint8_t __RUTINAS_ROM_VMA_FISICA[];
extern uint8_t __TABLAS_SISTEMA_16_VMA_FISICA[];
extern uint8_t __RESET_VMA_FISICA[];

extern uint8_t __TAREA_2_VMA_LINEAL[];
extern uint8_t __TAREA_2_TEXT_VMA_LINEAL[];
extern uint8_t __tarea_2_text_size[];
extern uint8_t __TAREA_2_BSS_VMA_LINEAL[];
extern uint8_t __tarea_2_bss_size[];
extern uint8_t __TAREA_2_DATA_VMA_LINEAL[];
extern uint8_t __tarea_2_data_size[];
extern uint8_t __TAREA_2_RODATA_VMA_LINEAL[];
extern uint8_t __tarea_2_rodata_size[];

extern uint8_t __TAREA_2_VMA_FISICA[];
extern uint8_t __TAREA_2_TEXT_VMA_FISICA[];
extern uint8_t __TAREA_2_BSS_VMA_FISICA[];
extern uint8_t __TAREA_2_DATA_VMA_FISICA[];
extern uint8_t __TAREA_2_RODATA_VMA_FISICA[];


extern uint8_t __TAREA_3_VMA_LINEAL[];
extern uint8_t __TAREA_3_TEXT_VMA_LINEAL[];
extern uint8_t __tarea_3_text_size[];
extern uint8_t __TAREA_3_BSS_VMA_LINEAL[];
extern uint8_t __tarea_3_bss_size[];
extern uint8_t __TAREA_3_DATA_VMA_LINEAL[];
extern uint8_t __tarea_3_data_size[];
extern uint8_t __TAREA_3_RODATA_VMA_LINEAL[];
extern uint8_t __tarea_3_rodata_size[];

extern uint8_t __TAREA_3_VMA_FISICA[];
extern uint8_t __TAREA_3_TEXT_VMA_FISICA[];
extern uint8_t __TAREA_3_BSS_VMA_FISICA[];
extern uint8_t __TAREA_3_DATA_VMA_FISICA[];
extern uint8_t __TAREA_3_RODATA_VMA_FISICA[];


extern uint8_t __TAREA_4_VMA_LINEAL[];
extern uint8_t __TAREA_4_TEXT_VMA_LINEAL[];
extern uint8_t __tarea_4_text_size[];
extern uint8_t __TAREA_4_BSS_VMA_LINEAL[];
extern uint8_t __tarea_4_bss_size[];
extern uint8_t __TAREA_4_DATA_VMA_LINEAL[];
extern uint8_t __tarea_4_data_size[];
extern uint8_t __TAREA_4_RODATA_VMA_LINEAL[];
extern uint8_t __tarea_4_rodata_size[];

extern uint8_t __TAREA_4_VMA_FISICA[];
extern uint8_t __TAREA_4_TEXT_VMA_FISICA[];
extern uint8_t __TAREA_4_BSS_VMA_FISICA[];
extern uint8_t __TAREA_4_DATA_VMA_FISICA[];
extern uint8_t __TAREA_4_RODATA_VMA_FISICA[];

#define DTP     ((directorio_tabla_paginas_t *) __TABLAS_PAGINACION_VMA_LINEAL)
#define PT      ((tabla_paginas_t *) &DTP[1])

extern registro_descriptor_segmento_t* _gdtr;

extern descriptor_segmento_t *gdt_nulo, *cs_gdt, *ds_gdt;//, *ss_gdt;
extern int *ss_gdt;
//#define ss_gdt  ((uint32_t *)0xFFFFF500)

#define DTP_kernel DTP[0]
#define DTP_tarea1 DTP[1]
#define DTP_tarea2 DTP[2]
#define DTP_tarea3 DTP[3]
#define DTP_tarea4 DTP[4]

// directorio_tabla_paginas_t  DTP_kernel __attribute__ ((section (".tablas_paginacion"))), 
//                             DTP_tarea1 __attribute__ ((section (".tablas_paginacion"))), 
//                             DTP_tarea2 __attribute__ ((section (".tablas_paginacion"))), 
//                             DTP_tarea3 __attribute__ ((section (".tablas_paginacion"))), 
//                             DTP_tarea4 __attribute__ ((section (".tablas_paginacion")));

#define TP0 DTP[5]
#define TP1 DTP[6]
#define TP2 DTP[7]
#define TP3 DTP[8]
#define TP4 DTP[9]
#define TP5 DTP[10]
#define TP_tarea_2 DTP[11]
#define TP_tarea_3 DTP[12]
#define TP_tarea_4 DTP[13]

// tabla_paginas_t TP0 __attribute__ ((section (".tablas_paginacion"))), 
//                 TP1 __attribute__ ((section (".tablas_paginacion"))),
//                 TP2 __attribute__ ((section (".tablas_paginacion"))), 
//                 TP3 __attribute__ ((section (".tablas_paginacion"))),
//                 TP4 __attribute__ ((section (".tablas_paginacion"))),
//                 TP5 __attribute__ ((section (".tablas_paginacion"))),
//                 TP_tarea_2 __attribute__ ((section (".tablas_paginacion"))),
//                 TP_tarea_3 __attribute__ ((section (".tablas_paginacion"))),
//                 TP_tarea_4 __attribute__ ((section (".tablas_paginacion")));

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
    // //Probando doble falta, comentar cuando quieras que ande bien
    // cargar_descriptor_segmento_int( &IDT[0], 
    //                                 0,
    //                                 (uint32_t)Divide_Error_Handler,
    //                                 TIPO_EXCEPCION, //Tipo
    //                                 1, //D
    //                                 1, //P
    //                                 0 //DPL
    //                                 );

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
                                    (uint32_t)Invalid_Opcode_Handler,
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

__attribute__(( section(".rutinas")))
void cr3_set(directorio_tabla_paginas_t *direccion_DTP, uint8_t PCD, uint8_t PWT)
{
    uint32_t cr3 = 0;

    cr3 |= ((uint32_t)direccion_DTP & 0xFFFFF000);
    cr3 |= (PCD << 4);
    cr3 |= (PWT << 3);

    asm("mov %0, %%cr3"
        : 
        : "r" (cr3));
}

__attribute__(( section(".rutinas")))
void DTP_agregar_tabla( directorio_tabla_paginas_t *direccion_DTP, uint16_t indice, tabla_paginas_t *direccion_tabla,
                        uint8_t tamanio, uint8_t accedida, uint8_t PCD, uint8_t PWT, uint8_t US_SUP, uint8_t R_RW, uint8_t presente)
{
    uint32_t dpte = 0;
    
    dpte |= ((uint32_t)direccion_tabla & 0xFFFFF000);
    dpte |= ( tamanio       << 7);
    dpte |= ( accedida      << 5);
    dpte |= ( PCD           << 4);
    dpte |= ( PWT           << 3);
    dpte |= ( US_SUP        << 2);
    dpte |= ( R_RW          << 1);
    dpte |= ( presente      << 0);

    direccion_DTP->descriptores_tablas_pagina[indice] = dpte;
}

__attribute__(( section(".rutinas")))
void DTP_inicializar(directorio_tabla_paginas_t *direccion_DTP)
{
    uint32_t auxiliar;
    
    //Borrar
    direccion_DTP = &DTP_kernel;

    //La primer tabla de paginas cubre desde 0x0000_0000 hasta 0x003F_FFFF
    auxiliar = (uint32_t)__TABLAS_SISTEMA_VMA_LINEAL >> (10+12);
    DTP_agregar_tabla(  direccion_DTP, 
                        auxiliar,  //Indice
                        &TP0,//&PT[auxiliar], //Direccion de la tabla
                        0,  //Tamaño de 4K
                        0,  //Accedida
                        0,  //PCD NO
                        0,  //PWT NO
                        0,  //Supervisor
                        1,  //RW
                        1   //Presente
                        );

    //La siguiente tabla de paginas cubre desde 0x0040_0000 hasta 0x007F_FFFF
    auxiliar = (uint32_t)__TAREA_1_PILA_FINAL_LINEAL >> (10+12);
    DTP_agregar_tabla(  direccion_DTP, 
                        auxiliar,  //Indice
                        &TP1,//&PT[auxiliar], //Direccion de la tabla
                        0,  //Tamaño de 4K
                        0,  //Accedida
                        0,  //PCD NO
                        0,  //PWT NO
                        0,  //Supervisor
                        1,  //RW
                        1   //Presente
                        );

    //La siguiente tabla de paginas cubre desde 0x00C0_0000 hasta 0x00FF_FFFF
    auxiliar = (uint32_t)__VIDEO_VMA_LINEAL >> (10+12);
    DTP_agregar_tabla(  direccion_DTP, 
                        auxiliar,  //Indice
                        &TP2,//&PT[auxiliar], //Direccion de la tabla
                        0,  //Tamaño de 4K
                        0,  //Accedida
                        0,  //PCD NO
                        0,  //PWT NO
                        0,  //Supervisor
                        1,  //RW
                        1   //Presente
                        );

    //La siguiente tabla de paginas cubre desde 0x0100_0000 hasta 0x013F_FFFF
    auxiliar = (uint32_t)__DATOS_VMA_LINEAL >> (10+12);
    DTP_agregar_tabla(  direccion_DTP, 
                        auxiliar,  //Indice
                        &TP3,//&PT[auxiliar], //Direccion de la tabla
                        0,  //Tamaño de 4K
                        0,  //Accedida
                        0,  //PCD NO
                        0,  //PWT NO
                        0,  //Supervisor
                        1,  //RW
                        1   //Presente
                        );

    //La siguiente tabla de paginas cubre desde 0x1FC0_0000 hasta 0x1FFF_FFFF
    auxiliar = (uint32_t)__KERNEL_PILA_FINAL_LINEAL >> (10+12);
    DTP_agregar_tabla(  direccion_DTP, 
                        auxiliar,  //Indice
                        &TP4,//&PT[auxiliar], //Direccion de la tabla
                        0,  //Tamaño de 4K
                        0,  //Accedida
                        0,  //PCD NO
                        0,  //PWT NO
                        0,  //Supervisor
                        1,  //RW
                        1   //Presente
                        );


    //La siguiente tabla de paginas cubre desde 0xFFC0_0000 hasta 0xFFFF_FFFF
    auxiliar = (uint32_t)__ROM_VMA_LINEAL >> (10+12);
    DTP_agregar_tabla(  direccion_DTP, 
                        auxiliar,  //Indice
                        &TP5,//&PT[auxiliar], //Direccion de la tabla
                        0,  //Tamaño de 4K
                        0,  //Accedida
                        0,  //PCD NO
                        0,  //PWT NO
                        0,  //Supervisor
                        1,  //RW
                        1   //Presente
                        );

    //La siguiente tabla de paginas cubre desde 0xFFC0_0000 hasta 0xFFFF_FFFF
    auxiliar = (uint32_t)__TAREA_2_VMA_LINEAL >> (10+12);
    DTP_agregar_tabla(  direccion_DTP, 
                        auxiliar,  //Indice
                        &TP_tarea_2,//&PT[auxiliar], //Direccion de la tabla
                        0,  //Tamaño de 4K
                        0,  //Accedida
                        0,  //PCD NO
                        0,  //PWT NO
                        0,  //Supervisor
                        1,  //RW
                        1   //Presente
                        );
    
    //     //La siguiente tabla de paginas cubre desde 0xFFC0_0000 hasta 0xFFFF_FFFF
    // auxiliar = (uint32_t)__TAREA_3_VMA_LINEAL >> (10+12);
    // DTP_agregar_tabla(  direccion_DTP, 
    //                     auxiliar,  //Indice
    //                     &TP3,//&PT[auxiliar], //Direccion de la tabla
    //                     0,  //Tamaño de 4K
    //                     0,  //Accedida
    //                     0,  //PCD NO
    //                     0,  //PWT NO
    //                     0,  //Supervisor
    //                     1,  //RW
    //                     1   //Presente
    //                     );

    // //La siguiente tabla de paginas cubre desde 0xFFC0_0000 hasta 0xFFFF_FFFF
    // auxiliar = (uint32_t)__TAREA_4_VMA_LINEAL >> (10+12);
    // DTP_agregar_tabla(  direccion_DTP, 
    //                     auxiliar,  //Indice
    //                     &TP3,//&PT[auxiliar], //Direccion de la tabla
    //                     0,  //Tamaño de 4K
    //                     0,  //Accedida
    //                     0,  //PCD NO
    //                     0,  //PWT NO
    //                     0,  //Supervisor
    //                     1,  //RW
    //                     1   //Presente
    //                     );
}

__attribute__(( section(".rutinas")))
void PT_agregar_pagina( tabla_paginas_t *direccion_PT, uint16_t indice, uint8_t *puntero_pagina, uint8_t G, 
                        uint8_t PAT, uint8_t D, uint8_t A, uint8_t PCD, uint8_t PWT, uint8_t U_S, uint8_t R_W, uint8_t P)
{
    uint32_t pte = 0;

    pte = (uint32_t)puntero_pagina & 0xFFFFF000;

    pte |= G    << 8;
    pte |= PAT  << 7;
    pte |= D    << 6;
    pte |= A    << 5;
    pte |= PCD  << 4;
    pte |= PWT  << 3;
    pte |= U_S  << 2;
    pte |= R_W  << 1;
    pte |= P    << 0;

    direccion_PT->descriptores_paginas[indice] = pte;
}

__attribute__(( section(".rutinas")))
void PTs_inicializar(tabla_paginas_t *direccion_PT)
{
    uint16_t aux;
    uint32_t aux2;
    uint8_t i;
    uint32_t direccion_lineal, direccion_fisica, cant_pag;

    //Tablas de sistema
    cant_pag = (uint32_t)__tablas_sistema_size / 0x1000 + (((uint32_t)__tablas_sistema_size % 0x1000) != 0);
    if(cant_pag == 0)
        cant_pag = 1;
    for(i = 0; i < cant_pag; i++)
    {
        direccion_lineal = (uint32_t)__TABLAS_SISTEMA_VMA_LINEAL + i * 0x1000;
        direccion_fisica = (uint32_t)__TABLAS_SISTEMA_VMA_FISICA + i * 0x1000;
        aux = (direccion_lineal >> 12) & 0x3FF;
        aux2 = (direccion_lineal >> 22) & 0x3FF;
        PT_agregar_pagina(  &TP0,    //Direccion de las tablas
                            aux,                    //Indice
                            (uint8_t*)direccion_fisica,    //Puntero a pagina
                            1,                      //Global
                            0,                      //PAT no
                            0,                      //D no
                            0,                      //A si
                            0,                      //PCD no
                            0,                      //PWT no
                            0,                      //Supervisor
                            0,                      //Lectura
                            1);                     //Presente
    }
    
    //Tablas de paginacion
    cant_pag = (uint32_t)__tablas_paginacion_size / 0x1000 + (((uint32_t)__tablas_paginacion_size % 0x1000) != 0);
    if(cant_pag == 0)
        cant_pag = 1;
    for(i = 0; i < cant_pag; i++)
    {
        direccion_lineal = (uint32_t)__TABLAS_PAGINACION_VMA_LINEAL + i * 0x1000;
        direccion_fisica = (uint32_t)__TABLAS_PAGINACION_VMA_FISICA + i * 0x1000;
        aux = (direccion_lineal >> 12) & 0x3FF;
        aux2 = (direccion_lineal >> 22) & 0x3FF;
        PT_agregar_pagina(  &TP0,    //Direccion de las tablas
                            aux,                    //Indice
                            (uint8_t*)direccion_fisica,//Puntero a pagina
                            1,                      //Global
                            0,                      //PAT no
                            0,                      //D no
                            0,                      //A si
                            0,                      //PCD no
                            0,                      //PWT no
                            0,                      //Supervisor
                            1,                      //Escritura
                            1);                     //Presente
    }


    //Rutinas
    cant_pag = (uint32_t)__rutinas_size / 0x1000 + (((uint32_t)__rutinas_size % 0x1000) != 0);
    if(cant_pag == 0)
        cant_pag = 1;
    for(i = 0; i < cant_pag; i++)
    {
        direccion_lineal = (uint32_t)__RUTINAS_VMA_LINEAL + i * 0x1000;
        direccion_fisica = (uint32_t)__RUTINAS_VMA_FISICA + i * 0x1000;
        aux = (direccion_lineal >> 12) & 0x3FF;
        aux2 = (direccion_lineal >> 22) & 0x3FF;
        PT_agregar_pagina(  &TP0,    //Direccion de las tablas
                            aux,                    //Indice
                            (uint8_t*)direccion_fisica,    //Puntero a pagina
                            1,                      //Global
                            0,                      //PAT no
                            0,                      //D no
                            0,                      //A si
                            0,                      //PCD no
                            0,                      //PWT no
                            0,                      //Supervisor
                            0,                      //Lectura
                            1);                     //Presente
    }

    //Sección de RAM de video
    cant_pag = (uint32_t)__video_size / 0x1000 + (((uint32_t)__video_size % 0x1000) != 0);
    if(cant_pag == 0)
        cant_pag = 1;
    for(i = 0; i < cant_pag; i++)
    {
        direccion_lineal = (uint32_t)__VIDEO_VMA_LINEAL + i * 0x1000;
        direccion_fisica = (uint32_t)__VIDEO_VMA_FISICA + i * 0x1000;
        aux = (direccion_lineal >> 12) & 0x3FF;
        aux2 = (direccion_lineal >> 22) & 0x3FF;
        PT_agregar_pagina(  &TP2,    //Direccion de las tablas
                            aux,                    //Indice
                            (uint8_t*)direccion_fisica,    //Puntero a pagina
                            1,                      //Global
                            0,                      //PAT no
                            0,                      //D no
                            0,                      //A si
                            0,                      //PCD no
                            0,                      //PWT no
                            0,                      //Supervisor
                            1,                      //Escritura
                            1);                     //Presente
    }
    
    //Interrupciones
    cant_pag = (uint32_t)__interrupciones_size / 0x1000 + (((uint32_t)__interrupciones_size % 0x1000) != 0);
    if(cant_pag == 0)
        cant_pag = 1;
    for(i = 0; i < cant_pag; i++)
    {
        direccion_lineal = (uint32_t)__INTERRUPCIONES_VMA_LINEAL + i * 0x1000;
        direccion_fisica = (uint32_t)__INTERRUPCIONES_VMA_FISICA + i * 0x1000;
        aux = (direccion_lineal >> 12) & 0x3FF;
        aux2 = (direccion_lineal >> 22) & 0x3FF;
        PT_agregar_pagina(  &TP0,    //Direccion de las tablas
                            aux,                    //Indice
                            (uint8_t*)direccion_fisica,    //Puntero a pagina
                            1,                      //Global
                            0,                      //PAT no
                            0,                      //D no
                            0,                      //A si
                            0,                      //PCD no
                            0,                      //PWT no
                            0,                      //Supervisor
                            0,                      //Lectura
                            1);                     //Presente
    }

    //Tabla de digitos
    cant_pag = (uint32_t)__tabla_digitos_size / 0x1000 + (((uint32_t)__tabla_digitos_size % 0x1000) != 0);
    if(cant_pag == 0)
        cant_pag = 1;
    for(i = 0; i < cant_pag; i++)
    {
        direccion_lineal = (uint32_t)__TABLA_DIGITOS_VMA_LINEAL + i * 0x1000;
        direccion_fisica = (uint32_t)__TABLA_DIGITOS_VMA_FISICA + i * 0x1000;
        aux = (direccion_lineal >> 12) & 0x3FF;
        aux2 = (direccion_lineal >> 22) & 0x3FF;
        PT_agregar_pagina(  &TP3,    //Direccion de las tablas
                            aux,                    //Indice
                            (uint8_t*)direccion_fisica,    //Puntero a pagina
                            1,                      //Global
                            0,                      //PAT no
                            0,                      //D no
                            0,                      //A si
                            0,                      //PCD no
                            0,                      //PWT no
                            0,                      //Supervisor
                            1,                      //Escritura
                            1);                     //Presente
    }

    //Datos
    cant_pag = (uint32_t)__datos_size / 0x1000 + (((uint32_t)__datos_size % 0x1000) != 0);
    if(cant_pag == 0)
        cant_pag = 1;
    for(i = 0; i < cant_pag; i++)
    {
        direccion_lineal = (uint32_t)__DATOS_VMA_LINEAL + i * 0x1000;
        direccion_fisica = (uint32_t)__DATOS_VMA_FISICA + i * 0x1000;
        aux = (direccion_lineal >> 12) & 0x3FF;
        aux2 = (direccion_lineal >> 22) & 0x3FF;
        PT_agregar_pagina(  &TP3,    //Direccion de las tablas
                            aux,                    //Indice
                            (uint8_t*)direccion_fisica,    //Puntero a pagina
                            1,                      //Global
                            0,                      //PAT no
                            0,                      //D no
                            0,                      //A si
                            0,                      //PCD no
                            0,                      //PWT no
                            0,                      //Supervisor
                            1,                      //Escritura
                            1);                     //Presente
    }
    
    //Kernel
    cant_pag = (uint32_t)__kernel_size / 0x1000 + (((uint32_t)__kernel_size % 0x1000) != 0);
    if(cant_pag == 0)
        cant_pag = 1;
    for(i = 0; i < cant_pag; i++)
    {
        direccion_lineal = (uint32_t)__KERNEL_VMA_LINEAL + i * 0x1000;
        direccion_fisica = (uint32_t)__KERNEL_VMA_FISICA + i * 0x1000;
        aux = (direccion_lineal >> 12) & 0x3FF;
        aux2 = (direccion_lineal >> 22) & 0x3FF;
        PT_agregar_pagina(  &TP3,    //Direccion de las tablas
                            aux,                    //Indice
                            (uint8_t*)direccion_fisica,    //Puntero a pagina
                            1,                      //Global
                            0,                      //PAT no
                            0,                      //D no
                            0,                      //A si
                            0,                      //PCD no
                            0,                      //PWT no
                            0,                      //Supervisor
                            0,                      //Lectura
                            1);                     //Presente
    }
    
    //Tarea 1 Codigo
    cant_pag = (uint32_t)__tarea_1_text_size / 0x1000 + (((uint32_t)__tarea_1_text_size % 0x1000) != 0);
    if(cant_pag == 0)
        cant_pag = 1;
    for(i = 0; i < cant_pag; i++)
    {
        direccion_lineal = (uint32_t)__TAREA_1_TEXT_VMA_LINEAL + i * 0x1000;
        direccion_fisica = (uint32_t)__TAREA_1_TEXT_VMA_FISICA + i * 0x1000;
        aux = (direccion_lineal >> 12) & 0x3FF;
        aux2 = (direccion_lineal >> 22) & 0x3FF;
        PT_agregar_pagina(  &TP3,    //Direccion de las tablas
                            aux,                    //Indice
                            (uint8_t*)direccion_fisica,    //Puntero a pagina
                            1,                      //Global
                            0,                      //PAT no
                            0,                      //D no
                            0,                      //A si
                            0,                      //PCD no
                            0,                      //PWT no
                            0,                      //Supervisor
                            0,                      //Lectura
                            1);                     //Presente
    }
    
    //Tarea 1 datos constantes
    cant_pag = (uint32_t)__tarea_1_bss_size / 0x1000 + (((uint32_t)__tarea_1_bss_size % 0x1000) != 0);
    if(cant_pag == 0)
        cant_pag = 1;
    for(i = 0; i < cant_pag; i++)
    {
        direccion_lineal = (uint32_t)__TAREA_1_BSS_VMA_LINEAL + i * 0x1000;
        direccion_fisica = (uint32_t)__TAREA_1_BSS_VMA_FISICA + i * 0x1000;
        aux = (direccion_lineal >> 12) & 0x3FF;
        aux2 = (direccion_lineal >> 22) & 0x3FF;
        PT_agregar_pagina(  &TP3,    //Direccion de las tablas
                            aux,                    //Indice
                            (uint8_t*)direccion_fisica,    //Puntero a pagina
                            1,                      //Global
                            0,                      //PAT no
                            0,                      //D no
                            0,                      //A si
                            0,                      //PCD no
                            0,                      //PWT no
                            0,                      //Supervisor
                            1,                      //Escritura
                            1);                     //Presente
    }
    
    //Tarea 1 datos
    cant_pag = (uint32_t)__tarea_1_data_size / 0x1000 + (((uint32_t)__tarea_1_data_size % 0x1000) != 0);
    if(cant_pag == 0)
        cant_pag = 1;
    for(i = 0; i < cant_pag; i++)
    {
        direccion_lineal = (uint32_t)__TAREA_1_DATA_VMA_LINEAL + i * 0x1000;
        direccion_fisica = (uint32_t)__TAREA_1_DATA_VMA_FISICA + i * 0x1000;
        aux = (direccion_lineal >> 12) & 0x3FF;
        aux2 = (direccion_lineal >> 22) & 0x3FF;
        PT_agregar_pagina(  &TP3,    //Direccion de las tablas
                            aux,                    //Indice
                            (uint8_t*)direccion_fisica,    //Puntero a pagina
                            1,                      //Global
                            0,                      //PAT no
                            0,                      //D no
                            0,                      //A si
                            0,                      //PCD no
                            0,                      //PWT no
                            0,                      //Supervisor
                            1,                      //Escritura
                            1);                     //Presente
    }

    //Tarea 1 rodata
    cant_pag = (uint32_t)__tarea_1_rodata_size / 0x1000 + (((uint32_t)__tarea_1_rodata_size % 0x1000) != 0);
    if(cant_pag == 0)
        cant_pag = 1;
    for(i = 0; i < cant_pag; i++)
    {
        direccion_lineal = (uint32_t)__TAREA_1_RODATA_VMA_LINEAL + i * 0x1000;
        direccion_fisica = (uint32_t)__TAREA_1_RODATA_VMA_FISICA + i * 0x1000;
        aux = (direccion_lineal >> 12) & 0x3FF;
        aux2 = (direccion_lineal >> 22) & 0x3FF;
        PT_agregar_pagina(  &TP3,    //Direccion de las tablas
                            aux,                    //Indice
                            (uint8_t*)direccion_fisica,    //Puntero a pagina
                            1,                      //Global
                            0,                      //PAT no
                            0,                      //D no
                            0,                      //A si
                            0,                      //PCD no
                            0,                      //PWT no
                            0,                      //Supervisor
                            0,                      //Lectura
                            1);                     //Presente
    }

    //PROVISORIO 
    //Tarea 2 Codigo
    cant_pag = (uint32_t)__tarea_2_text_size / 0x1000 + (((uint32_t)__tarea_2_text_size % 0x1000) != 0);
    if(cant_pag == 0)
        cant_pag = 1;
    for(i = 0; i < cant_pag; i++)
    {
        direccion_lineal = (uint32_t)__TAREA_2_TEXT_VMA_LINEAL + i * 0x1000;
        direccion_fisica = (uint32_t)__TAREA_2_TEXT_VMA_FISICA + i * 0x1000;
        aux = (direccion_lineal >> 12) & 0x3FF;
        aux2 = (direccion_lineal >> 22) & 0x3FF;
        PT_agregar_pagina(  &TP_tarea_2,    //Direccion de las tablas
                            aux,                    //Indice
                            (uint8_t*)direccion_fisica,    //Puntero a pagina
                            1,                      //Global
                            0,                      //PAT no
                            0,                      //D no
                            0,                      //A si
                            0,                      //PCD no
                            0,                      //PWT no
                            0,                      //Supervisor
                            0,                      //Lectura
                            1);                     //Presente
    }
    
    //Tarea 2 datos constantes
    cant_pag = (uint32_t)__tarea_2_bss_size / 0x1000 + (((uint32_t)__tarea_2_bss_size % 0x1000) != 0);
    if(cant_pag == 0)
        cant_pag = 1;
    for(i = 0; i < cant_pag; i++)
    {
        direccion_lineal = (uint32_t)__TAREA_2_BSS_VMA_LINEAL + i * 0x1000;
        direccion_fisica = (uint32_t)__TAREA_2_BSS_VMA_FISICA + i * 0x1000;
        aux = (direccion_lineal >> 12) & 0x3FF;
        aux2 = (direccion_lineal >> 22) & 0x3FF;
        PT_agregar_pagina(  &TP_tarea_2,    //Direccion de las tablas
                            aux,                    //Indice
                            (uint8_t*)direccion_fisica,    //Puntero a pagina
                            1,                      //Global
                            0,                      //PAT no
                            0,                      //D no
                            0,                      //A si
                            0,                      //PCD no
                            0,                      //PWT no
                            0,                      //Supervisor
                            1,                      //Escritura
                            1);                     //Presente
    }
    
    //Tarea 2 datos
    cant_pag = (uint32_t)__tarea_2_data_size / 0x1000 + (((uint32_t)__tarea_2_data_size % 0x1000) != 0);
    if(cant_pag == 0)
        cant_pag = 1;
    for(i = 0; i < cant_pag; i++)
    {
        direccion_lineal = (uint32_t)__TAREA_2_DATA_VMA_LINEAL + i * 0x1000;
        direccion_fisica = (uint32_t)__TAREA_2_DATA_VMA_FISICA + i * 0x1000;
        aux = (direccion_lineal >> 12) & 0x3FF;
        aux2 = (direccion_lineal >> 22) & 0x3FF;
        PT_agregar_pagina(  &TP_tarea_2,    //Direccion de las tablas
                            aux,                    //Indice
                            (uint8_t*)direccion_fisica,    //Puntero a pagina
                            1,                      //Global
                            0,                      //PAT no
                            0,                      //D no
                            0,                      //A si
                            0,                      //PCD no
                            0,                      //PWT no
                            0,                      //Supervisor
                            1,                      //Escritura
                            1);                     //Presente
    }

    //Tarea 2 rodata
    cant_pag = (uint32_t)__tarea_2_rodata_size / 0x1000 + (((uint32_t)__tarea_2_rodata_size % 0x1000) != 0);
    if(cant_pag == 0)
        cant_pag = 1;
    for(i = 0; i < cant_pag; i++)
    {
        direccion_lineal = (uint32_t)__TAREA_2_RODATA_VMA_LINEAL + i * 0x1000;
        direccion_fisica = (uint32_t)__TAREA_2_RODATA_VMA_FISICA + i * 0x1000;
        aux = (direccion_lineal >> 12) & 0x3FF;
        aux2 = (direccion_lineal >> 22) & 0x3FF;
        PT_agregar_pagina(  &TP_tarea_2,    //Direccion de las tablas
                            aux,                    //Indice
                            (uint8_t*)direccion_fisica,    //Puntero a pagina
                            1,                      //Global
                            0,                      //PAT no
                            0,                      //D no
                            0,                      //A si
                            0,                      //PCD no
                            0,                      //PWT no
                            0,                      //Supervisor
                            0,                      //Lectura
                            1);                     //Presente
    }


    //Tarea 3 Codigo
    cant_pag = (uint32_t)__tarea_3_text_size / 0x1000 + (((uint32_t)__tarea_3_text_size % 0x1000) != 0);
    if(cant_pag == 0)
        cant_pag = 1;
    for(i = 0; i < cant_pag; i++)
    {
        direccion_lineal = (uint32_t)__TAREA_3_TEXT_VMA_LINEAL + i * 0x1000;
        direccion_fisica = (uint32_t)__TAREA_3_TEXT_VMA_FISICA + i * 0x1000;
        aux = (direccion_lineal >> 12) & 0x3FF;
        aux2 = (direccion_lineal >> 22) & 0x3FF;
        PT_agregar_pagina(  &TP_tarea_2,    //Direccion de las tablas
                            aux,                    //Indice
                            (uint8_t*)direccion_fisica,    //Puntero a pagina
                            1,                      //Global
                            0,                      //PAT no
                            0,                      //D no
                            0,                      //A si
                            0,                      //PCD no
                            0,                      //PWT no
                            0,                      //Supervisor
                            0,                      //Lectura
                            1);                     //Presente
    }
    
    //Tarea 3 datos constantes
    cant_pag = (uint32_t)__tarea_3_bss_size / 0x1000 + (((uint32_t)__tarea_3_bss_size % 0x1000) != 0);
    if(cant_pag == 0)
        cant_pag = 1;
    for(i = 0; i < cant_pag; i++)
    {
        direccion_lineal = (uint32_t)__TAREA_3_BSS_VMA_LINEAL + i * 0x1000;
        direccion_fisica = (uint32_t)__TAREA_3_BSS_VMA_FISICA + i * 0x1000;
        aux = (direccion_lineal >> 12) & 0x3FF;
        aux2 = (direccion_lineal >> 22) & 0x3FF;
        PT_agregar_pagina(  &TP_tarea_2,    //Direccion de las tablas
                            aux,                    //Indice
                            (uint8_t*)direccion_fisica,    //Puntero a pagina
                            1,                      //Global
                            0,                      //PAT no
                            0,                      //D no
                            0,                      //A si
                            0,                      //PCD no
                            0,                      //PWT no
                            0,                      //Supervisor
                            1,                      //Escritura
                            1);                     //Presente
    }
    
    //Tarea 3 datos
    cant_pag = (uint32_t)__tarea_3_data_size / 0x1000 + (((uint32_t)__tarea_3_data_size % 0x1000) != 0);
    if(cant_pag == 0)
        cant_pag = 1;
    for(i = 0; i < cant_pag; i++)
    {
        direccion_lineal = (uint32_t)__TAREA_3_DATA_VMA_LINEAL + i * 0x1000;
        direccion_fisica = (uint32_t)__TAREA_3_DATA_VMA_FISICA + i * 0x1000;
        aux = (direccion_lineal >> 12) & 0x3FF;
        aux2 = (direccion_lineal >> 22) & 0x3FF;
        PT_agregar_pagina(  &TP_tarea_2,    //Direccion de las tablas
                            aux,                    //Indice
                            (uint8_t*)direccion_fisica,    //Puntero a pagina
                            1,                      //Global
                            0,                      //PAT no
                            0,                      //D no
                            0,                      //A si
                            0,                      //PCD no
                            0,                      //PWT no
                            0,                      //Supervisor
                            1,                      //Escritura
                            1);                     //Presente
    }

    //Tarea 3 rodata
    cant_pag = (uint32_t)__tarea_3_rodata_size / 0x1000 + (((uint32_t)__tarea_3_rodata_size % 0x1000) != 0);
    if(cant_pag == 0)
        cant_pag = 1;
    for(i = 0; i < cant_pag; i++)
    {
        direccion_lineal = (uint32_t)__TAREA_3_RODATA_VMA_LINEAL + i * 0x1000;
        direccion_fisica = (uint32_t)__TAREA_3_RODATA_VMA_FISICA + i * 0x1000;
        aux = (direccion_lineal >> 12) & 0x3FF;
        aux2 = (direccion_lineal >> 22) & 0x3FF;
        PT_agregar_pagina(  &TP_tarea_2,    //Direccion de las tablas
                            aux,                    //Indice
                            (uint8_t*)direccion_fisica,    //Puntero a pagina
                            1,                      //Global
                            0,                      //PAT no
                            0,                      //D no
                            0,                      //A si
                            0,                      //PCD no
                            0,                      //PWT no
                            0,                      //Supervisor
                            0,                      //Lectura
                            1);                     //Presente
    }

    //Tarea 4 Codigo
    cant_pag = (uint32_t)__tarea_4_text_size / 0x1000 + (((uint32_t)__tarea_4_text_size % 0x1000) != 0);
    if(cant_pag == 0)
        cant_pag = 1;
    for(i = 0; i < cant_pag; i++)
    {
        direccion_lineal = (uint32_t)__TAREA_4_TEXT_VMA_LINEAL + i * 0x1000;
        direccion_fisica = (uint32_t)__TAREA_4_TEXT_VMA_FISICA + i * 0x1000;
        aux = (direccion_lineal >> 12) & 0x3FF;
        aux2 = (direccion_lineal >> 22) & 0x3FF;
        PT_agregar_pagina(  &TP_tarea_2,    //Direccion de las tablas
                            aux,                    //Indice
                            (uint8_t*)direccion_fisica,    //Puntero a pagina
                            1,                      //Global
                            0,                      //PAT no
                            0,                      //D no
                            0,                      //A si
                            0,                      //PCD no
                            0,                      //PWT no
                            0,                      //Supervisor
                            0,                      //Lectura
                            1);                     //Presente
    }
    
    //Tarea 4 datos constantes
    cant_pag = (uint32_t)__tarea_4_bss_size / 0x1000 + (((uint32_t)__tarea_4_bss_size % 0x1000) != 0);
    if(cant_pag == 0)
        cant_pag = 1;
    for(i = 0; i < cant_pag; i++)
    {
        direccion_lineal = (uint32_t)__TAREA_4_BSS_VMA_LINEAL + i * 0x1000;
        direccion_fisica = (uint32_t)__TAREA_4_BSS_VMA_FISICA + i * 0x1000;
        aux = (direccion_lineal >> 12) & 0x3FF;
        aux2 = (direccion_lineal >> 22) & 0x3FF;
        PT_agregar_pagina(  &TP_tarea_2,    //Direccion de las tablas
                            aux,                    //Indice
                            (uint8_t*)direccion_fisica,    //Puntero a pagina
                            1,                      //Global
                            0,                      //PAT no
                            0,                      //D no
                            0,                      //A si
                            0,                      //PCD no
                            0,                      //PWT no
                            0,                      //Supervisor
                            1,                      //Escritura
                            1);                     //Presente
    }
    
    //Tarea 4 datos
    cant_pag = (uint32_t)__tarea_4_data_size / 0x1000 + (((uint32_t)__tarea_4_data_size % 0x1000) != 0);
    if(cant_pag == 0)
        cant_pag = 1;
    for(i = 0; i < cant_pag; i++)
    {
        direccion_lineal = (uint32_t)__TAREA_4_DATA_VMA_LINEAL + i * 0x1000;
        direccion_fisica = (uint32_t)__TAREA_4_DATA_VMA_FISICA + i * 0x1000;
        aux = (direccion_lineal >> 12) & 0x3FF;
        aux2 = (direccion_lineal >> 22) & 0x3FF;
        PT_agregar_pagina(  &TP_tarea_2,    //Direccion de las tablas
                            aux,                    //Indice
                            (uint8_t*)direccion_fisica,    //Puntero a pagina
                            1,                      //Global
                            0,                      //PAT no
                            0,                      //D no
                            0,                      //A si
                            0,                      //PCD no
                            0,                      //PWT no
                            0,                      //Supervisor
                            1,                      //Escritura
                            1);                     //Presente
    }

    //Tarea 4 rodata
    cant_pag = (uint32_t)__tarea_4_rodata_size / 0x1000 + (((uint32_t)__tarea_4_rodata_size % 0x1000) != 0);
    if(cant_pag == 0)
        cant_pag = 1;
    for(i = 0; i < cant_pag; i++)
    {
        direccion_lineal = (uint32_t)__TAREA_4_RODATA_VMA_LINEAL + i * 0x1000;
        direccion_fisica = (uint32_t)__TAREA_4_RODATA_VMA_FISICA + i * 0x1000;
        aux = (direccion_lineal >> 12) & 0x3FF;
        aux2 = (direccion_lineal >> 22) & 0x3FF;
        PT_agregar_pagina(  &TP_tarea_2,    //Direccion de las tablas
                            aux,                    //Indice
                            (uint8_t*)direccion_fisica,    //Puntero a pagina
                            1,                      //Global
                            0,                      //PAT no
                            0,                      //D no
                            0,                      //A si
                            0,                      //PCD no
                            0,                      //PWT no
                            0,                      //Supervisor
                            0,                      //Lectura
                            1);                     //Presente
    }
    //PROVISORIO FIN
    
    //Pila del kernel
    aux = ((uint32_t)__KERNEL_PILA_FINAL_LINEAL >> 12) & 0x3FF;
    aux2 = ((uint32_t)__KERNEL_PILA_FINAL_LINEAL >> 22) & 0x3FF;
    PT_agregar_pagina(  &TP4,    //Direccion de las tablas
                        aux,                    //Indice
                        __KERNEL_PILA_FINAL_FISICA,   //Puntero a pagina
                        1,                      //Global
                        0,                      //PAT no
                        0,                      //D no
                        0,                      //A si
                        0,                      //PCD no
                        0,                      //PWT no
                        0,                      //Supervisor
                        1,                      //Escritura
                        1);                     //Presente
    
    //Pila de tarea 1
    aux = ((uint32_t)__TAREA_1_PILA_FINAL_LINEAL >> 12) & 0x3FF;
    aux2 = ((uint32_t)__TAREA_1_PILA_FINAL_LINEAL >> 22) & 0x3FF;
    PT_agregar_pagina(  &TP1,    //Direccion de las tablas
                        aux,                    //Indice
                        __TAREA_1_PILA_FINAL_FISICA,   //Puntero a pagina
                        1,                      //Global
                        0,                      //PAT no
                        0,                      //D no
                        0,                      //A si
                        0,                      //PCD no
                        0,                      //PWT no
                        0,                      //Supervisor
                        1,                      //Escritura
                        1);                     //Presente
    
    //Paginacion de la ROM
    for(i=0; i<16; i++)
    {
        direccion_lineal = (uint32_t)__ROM_VMA_LINEAL + i * 0x1000;
        direccion_fisica = (uint32_t)__ROM_VMA_FISICA + i * 0x1000;
        aux = ((uint32_t)direccion_lineal >> 12) & 0x3FF;
        aux2 = ((uint32_t)direccion_lineal >> 22) & 0x3FF;
        PT_agregar_pagina(  &TP5,    //Direccion de las tablas
                            aux,                    //Indice
                            (uint8_t*)direccion_fisica,    //Puntero a pagina
                            1,                      //Global
                            0,                      //PAT no
                            0,                      //D no
                            0,                      //A si
                            0,                      //PCD no
                            0,                      //PWT no
                            0,                      //Supervisor
                            0,                      //Lectura
                            1);                     //Presente
    }
}

__attribute__(( section(".rutinas")))
void paginacion_inicializar(void)
{
    MAGIC_BREAKPOINT
    //Inicializo CR3
    cr3_set(&DTP_kernel, 1, 1);

    //Inicializar tablas DTP y PT
    //Inicializo la DTP
    DTP_inicializar(&DTP_kernel);

    //Inicializo las PTs
    PTs_inicializar(PT);

    //Activo la paginacion
    asm("mov %cr0, %eax");
    asm("or  $0x80000000, %eax");
    asm("mov %eax, %cr0");
    
    MAGIC_BREAKPOINT
}

uint32_t numero_pagina_dinamica __attribute__ ((section (".datos")));

__attribute__(( section(".rutinas")))
void agregar_pagina_dinamicamente(uint8_t *direccion_lineal)
{
    uint32_t aux1, aux2, direccion_fisica;

    direccion_fisica = (uint32_t)__PAGINACION_DINAMICA_BASE_FISICA + numero_pagina_dinamica * 0x1000;
    numero_pagina_dinamica++;

    //Agrego la tabla de paginas
    aux1 = (uint32_t)direccion_lineal >> (10+12);
    
    DTP_agregar_tabla(  DTP, 
                        aux1,  //Indice
                        &PT[aux1], //Direccion de la tabla
                        0,  //Tamaño de 4K
                        0,  //Accedida
                        0,  //PCD NO
                        0,  //PWT NO
                        0,  //Supervisor
                        1,  //RW
                        1   //Presente
                        );

    
    //Agrego la pagina
    aux2 = ((uint32_t)direccion_lineal >> 12) & 0x3FF;
    PT_agregar_pagina(  &PT[aux1],              //Direccion de las tablas
                        aux2,                   //Indice
                        (uint8_t*)direccion_fisica,       //Puntero a pagina
                        1,                      //Global
                        0,                      //PAT no
                        0,                      //D no
                        0,                      //A si
                        0,                      //PCD no
                        0,                      //PWT no
                        0,                      //Supervisor
                        1,                      //Escritura
                        1);                     //Presente
}