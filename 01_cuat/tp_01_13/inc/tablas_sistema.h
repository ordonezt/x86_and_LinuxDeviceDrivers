#ifndef __TABLAS_SISTEMAS_H
#define __TABLAS_SISTEMAS_H

#include "../inc/my_types.h"

#define LIMITE_MAXIMO   0xFFFFFFFF
//extern uint32_t __CS_BASE, __CS_LIMITE, __DS_BASE, __DS_LIMITE, __SS_BASE, __SS_LIMITE;
#define __CS_BASE        0x00000000
#define __CS_LIMITE      0xFFFFFFFF
#define __DS_BASE        __CS_BASE
#define __DS_LIMITE      __CS_LIMITE
#define __SS_BASE        __CS_BASE
#define __SS_LIMITE      __CS_LIMITE

#define CS0_SELECTOR 0b0000000000001000
#define DS0_SELECTOR 0b0000000000010000

#define CS3_SELECTOR 0b0000000000011011
#define DS3_SELECTOR 0b0000000000100011

#define TSS_SELECTOR 0b0000000000101000

// #define CS3_SELECTOR 0b0000000000011000
// #define DS3_SELECTOR 0b0000000000100000

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
extern uint8_t __TAREA_1_PILA_USUARIO_FINAL_LINEAL[];
extern uint8_t __TAREA_2_PILA_USUARIO_FINAL_LINEAL[];
extern uint8_t __TAREA_3_PILA_USUARIO_FINAL_LINEAL[];
extern uint8_t __TAREA_4_PILA_USUARIO_FINAL_LINEAL[];

extern uint8_t __TAREA_1_PILA_SUPERVISOR_FINAL_LINEAL[];
extern uint8_t __TAREA_2_PILA_SUPERVISOR_FINAL_LINEAL[];
extern uint8_t __TAREA_3_PILA_SUPERVISOR_FINAL_LINEAL[];
extern uint8_t __TAREA_4_PILA_SUPERVISOR_FINAL_LINEAL[];

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

extern uint8_t __TAREA_1_PILA_USUARIO_FINAL_FISICA[];
extern uint8_t __TAREA_2_PILA_USUARIO_FINAL_FISICA[];
extern uint8_t __TAREA_3_PILA_USUARIO_FINAL_FISICA[];
extern uint8_t __TAREA_4_PILA_USUARIO_FINAL_FISICA[];

extern uint8_t __TAREA_1_PILA_SUPERVISOR_FINAL_FISICA[];
extern uint8_t __TAREA_2_PILA_SUPERVISOR_FINAL_FISICA[];
extern uint8_t __TAREA_3_PILA_SUPERVISOR_FINAL_FISICA[];
extern uint8_t __TAREA_4_PILA_SUPERVISOR_FINAL_FISICA[];

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

extern uint8_t __TAREA_1_PILA_USUARIO_INICIO_LINEAL[];
extern uint8_t __TAREA_2_PILA_USUARIO_INICIO_LINEAL[];
extern uint8_t __TAREA_3_PILA_USUARIO_INICIO_LINEAL[];
extern uint8_t __TAREA_4_PILA_USUARIO_INICIO_LINEAL[];

extern uint8_t __TAREA_1_PILA_USUARIO_INICIO_FISICA[];
extern uint8_t __TAREA_2_PILA_USUARIO_INICIO_FISICA[];
extern uint8_t __TAREA_3_PILA_USUARIO_INICIO_FISICA[];
extern uint8_t __TAREA_4_PILA_USUARIO_INICIO_FISICA[];

extern uint8_t __TAREA_1_PILA_SUPERVISOR_INICIO_LINEAL[];
extern uint8_t __TAREA_2_PILA_SUPERVISOR_INICIO_LINEAL[];
extern uint8_t __TAREA_3_PILA_SUPERVISOR_INICIO_LINEAL[];
extern uint8_t __TAREA_4_PILA_SUPERVISOR_INICIO_LINEAL[];

extern uint8_t __TAREA_1_PILA_SUPERVISOR_INICIO_FISICA[];
extern uint8_t __TAREA_2_PILA_SUPERVISOR_INICIO_FISICA[];
extern uint8_t __TAREA_3_PILA_SUPERVISOR_INICIO_FISICA[];
extern uint8_t __TAREA_4_PILA_SUPERVISOR_INICIO_FISICA[];

extern uint8_t __TAREA_0_CONTEXTO_GENERAL_VMA_FISICA[];
extern uint8_t __TAREA_0_CONTEXTO_SIMD_VMA_FISICA[];
extern uint8_t __TAREA_0_CONTEXTO_GENERAL_VMA_LINEAL[];
extern uint8_t __TAREA_0_CONTEXTO_SIMD_VMA_LINEAL[];

extern uint8_t __TAREA_1_CONTEXTO_GENERAL_VMA_FISICA[];
extern uint8_t __TAREA_1_CONTEXTO_SIMD_VMA_FISICA[];
extern uint8_t __TAREA_1_CONTEXTO_GENERAL_VMA_LINEAL[];
extern uint8_t __TAREA_1_CONTEXTO_SIMD_VMA_LINEAL[];

extern uint8_t __TAREA_2_CONTEXTO_GENERAL_VMA_FISICA[];
extern uint8_t __TAREA_2_CONTEXTO_SIMD_VMA_FISICA[];
extern uint8_t __TAREA_2_CONTEXTO_GENERAL_VMA_LINEAL[];
extern uint8_t __TAREA_2_CONTEXTO_SIMD_VMA_LINEAL[];

extern uint8_t __TAREA_3_CONTEXTO_GENERAL_VMA_FISICA[];
extern uint8_t __TAREA_3_CONTEXTO_SIMD_VMA_FISICA[];
extern uint8_t __TAREA_3_CONTEXTO_GENERAL_VMA_LINEAL[];
extern uint8_t __TAREA_3_CONTEXTO_SIMD_VMA_LINEAL[];

extern uint8_t __TAREA_4_CONTEXTO_GENERAL_VMA_FISICA[];
extern uint8_t __TAREA_4_CONTEXTO_SIMD_VMA_FISICA[];
extern uint8_t __TAREA_4_CONTEXTO_GENERAL_VMA_LINEAL[];
extern uint8_t __TAREA_4_CONTEXTO_SIMD_VMA_LINEAL[];

extern uint8_t __TSS_BASICA_T1_FISICA[];
extern uint8_t __TSS_BASICA_T2_FISICA[];
extern uint8_t __TSS_BASICA_T3_FISICA[];
extern uint8_t __TSS_BASICA_T4_FISICA[];
extern uint8_t __TSS_BASICA_LINEAL[];

#define DTP     ((directorio_tabla_paginas_t *) __TABLAS_PAGINACION_VMA_LINEAL)
#define PT      ((tabla_paginas_t *) &DTP[1])

extern registro_descriptor_segmento_t* _gdtr;

extern descriptor_segmento_t *gdt_nulo, *cs_gdt, *ds_gdt;//, *ss_gdt;
extern int *ss_gdt;
//#define ss_gdt  ((uint32_t *)0xFFFFF500)

#define DTP_kernel &DTP[0]
#define DTP_tarea1 &DTP[1]
#define DTP_tarea2 &DTP[2]
#define DTP_tarea3 &DTP[3]
#define DTP_tarea4 &DTP[4]

// directorio_tabla_paginas_t  DTP_kernel __attribute__ ((section (".tablas_paginacion"))), 
//                             DTP_tarea1 __attribute__ ((section (".tablas_paginacion"))), 
//                             DTP_tarea2 __attribute__ ((section (".tablas_paginacion"))), 
//                             DTP_tarea3 __attribute__ ((section (".tablas_paginacion"))), 
//                             DTP_tarea4 __attribute__ ((section (".tablas_paginacion")));

#define TP_kernel_base     (tabla_paginas_t*)&DTP[5]
#define TP0_kernel         (TP_kernel_base + 0)
#define TP1_kernel         (TP_kernel_base + 1)//(tabla_paginas_t*)&DTP[6]
#define TP2_kernel         (TP_kernel_base + 2)//(tabla_paginas_t*)&DTP[7]
#define TP3_kernel         (TP_kernel_base + 3)//(tabla_paginas_t*)&DTP[8]
#define TP4_kernel         (TP_kernel_base + 4)//(tabla_paginas_t*)&DTP[9]
#define TP5_kernel         (TP_kernel_base + 5)//(tabla_paginas_t*)&DTP[10]
//#define TP6_kernel         (TP_kernel_base + 6)
#define TP_kernel_final    TP5_kernel

#define TP_tarea1_base     (TP_kernel_final + 1)
#define TP0_tarea1         (TP_tarea1_base + 0)//(tabla_paginas_t*)&DTP[11]
#define TP1_tarea1         (TP_tarea1_base + 1)//(tabla_paginas_t*)&DTP[12]
#define TP2_tarea1         (TP_tarea1_base + 2)//(tabla_paginas_t*)&DTP[13]
#define TP3_tarea1         (TP_tarea1_base + 3)//(tabla_paginas_t*)&DTP[14]
#define TP4_tarea1         (TP_tarea1_base + 4)//(tabla_paginas_t*)&DTP[15]
#define TP5_tarea1         (TP_tarea1_base + 5)//(tabla_paginas_t*)&DTP[16]
#define TP6_tarea1         (TP_tarea1_base + 6)
#define TP_tarea1_final    TP6_tarea1

#define TP_tarea2_base     (TP_tarea1_final + 1)
#define TP0_tarea2         (TP_tarea2_base + 0)//(tabla_paginas_t*)&DTP[17]
#define TP1_tarea2         (TP_tarea2_base + 1)//(tabla_paginas_t*)&DTP[18]
#define TP2_tarea2         (TP_tarea2_base + 2)//(tabla_paginas_t*)&DTP[19]
#define TP3_tarea2         (TP_tarea2_base + 3)//(tabla_paginas_t*)&DTP[20]
#define TP4_tarea2         (TP_tarea2_base + 4)//(tabla_paginas_t*)&DTP[21]
#define TP5_tarea2         (TP_tarea2_base + 5)//(tabla_paginas_t*)&DTP[22]
#define TP6_tarea2         (TP_tarea2_base + 6)//(tabla_paginas_t*)&DTP[23]
#define TP7_tarea2         (TP_tarea2_base + 7)
#define TP_tarea2_final    TP7_tarea2

#define TP_tarea3_base     (TP_tarea2_final + 1)
#define TP0_tarea3         (TP_tarea3_base + 0)//(tabla_paginas_t*)&DTP[24]
#define TP1_tarea3         (TP_tarea3_base + 1)//(tabla_paginas_t*)&DTP[25]
#define TP2_tarea3         (TP_tarea3_base + 2)//(tabla_paginas_t*)&DTP[26]
#define TP3_tarea3         (TP_tarea3_base + 3)//(tabla_paginas_t*)&DTP[27]
#define TP4_tarea3         (TP_tarea3_base + 4)//(tabla_paginas_t*)&DTP[28]
#define TP5_tarea3         (TP_tarea3_base + 5)//(tabla_paginas_t*)&DTP[29]
#define TP6_tarea3         (TP_tarea3_base + 6)//(tabla_paginas_t*)&DTP[30]
#define TP7_tarea3         (TP_tarea3_base + 7)
#define TP_tarea3_final    TP7_tarea3

#define TP_tarea4_base     (TP_tarea3_final + 1)
#define TP0_tarea4         (TP_tarea4_base + 0)//(tabla_paginas_t*)&DTP[31]
#define TP1_tarea4         (TP_tarea4_base + 1)//(tabla_paginas_t*)&DTP[32]
#define TP2_tarea4         (TP_tarea4_base + 2)//(tabla_paginas_t*)&DTP[33]
#define TP3_tarea4         (TP_tarea4_base + 3)//(tabla_paginas_t*)&DTP[34]
#define TP4_tarea4         (TP_tarea4_base + 4)//(tabla_paginas_t*)&DTP[35]
#define TP5_tarea4         (TP_tarea4_base + 5)//(tabla_paginas_t*)&DTP[36]
#define TP6_tarea4         (TP_tarea4_base + 6)//(tabla_paginas_t*)&DTP[37]
#define TP7_tarea4         (TP_tarea4_base + 7)
#define TP_tarea4_final    TP7_tarea4

#define contexto_tarea_0        (contexto_tarea_t*)__TAREA_0_CONTEXTO_GENERAL_VMA_LINEAL
#define contexto_simd_tarea_0   (contexto_simd_t*)__TAREA_0_CONTEXTO_SIMD_VMA_LINEAL

#define contexto_tarea_1        (contexto_tarea_t*)__TAREA_1_CONTEXTO_GENERAL_VMA_LINEAL
#define contexto_simd_tarea_1   (contexto_simd_t*)__TAREA_1_CONTEXTO_SIMD_VMA_LINEAL

#define contexto_tarea_2        (contexto_tarea_t*)__TAREA_2_CONTEXTO_GENERAL_VMA_LINEAL
#define contexto_simd_tarea_2   (contexto_simd_t*)__TAREA_2_CONTEXTO_SIMD_VMA_LINEAL

#define contexto_tarea_3        (contexto_tarea_t*)__TAREA_3_CONTEXTO_GENERAL_VMA_LINEAL
#define contexto_simd_tarea_3   (contexto_simd_t*)__TAREA_3_CONTEXTO_SIMD_VMA_LINEAL

#define contexto_tarea_4        (contexto_tarea_t*)__TAREA_4_CONTEXTO_GENERAL_VMA_LINEAL
#define contexto_simd_tarea_4   (contexto_simd_t*)__TAREA_4_CONTEXTO_SIMD_VMA_LINEAL

// tabla_paginas_t TP0 __attribute__ ((section (".tablas_paginacion"))), 
//                 TP1 __attribute__ ((section (".tablas_paginacion"))),
//                 TP2 __attribute__ ((section (".tablas_paginacion"))), 
//                 TP3 __attribute__ ((section (".tablas_paginacion"))),
//                 TP4 __attribute__ ((section (".tablas_paginacion"))),
//                 TP5 __attribute__ ((section (".tablas_paginacion"))),
//                 TP_tarea_2 __attribute__ ((section (".tablas_paginacion"))),
//                 TP_tarea_3 __attribute__ ((section (".tablas_paginacion"))),
//                 TP_tarea_4 __attribute__ ((section (".tablas_paginacion")));

void agregar_pagina_dinamicamente(uint8_t *direccion_lineal);

#endif  /*__TABLAS_SISTEMAS_H*/