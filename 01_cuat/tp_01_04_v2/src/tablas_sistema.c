#include "../inc/my_types.h"

#define LIMITE_MAXIMO   0xFFFFFFFF
//extern uint32_t __CS_BASE, __CS_LIMITE, __DS_BASE, __DS_LIMITE, __SS_BASE, __SS_LIMITE;
#define __CS_BASE        0x00000000
#define __CS_LIMITE      0xFFFFFFFF
#define __DS_BASE        __CS_BASE
#define __DS_LIMITE      __CS_LIMITE
#define __SS_BASE        __CS_BASE
#define __SS_LIMITE      __CS_LIMITE

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