#include "../inc/my_types.h"

#define LIMITE_MAXIMO 0xFFFFFFFF

typedef struct{
    uint16_t limite_0;
    uint32_t base_0  :24;
    uint32_t A       :1;
    union{
        uint32_t W   :1;
        uint32_t R   :1;
    };
    union{
        uint32_t ED  :1;
        uint32_t C   :1;
    };
    uint32_t bit_11  :1;
    uint32_t S       :1;
    uint32_t DPL     :2;
    uint32_t P       :1;
    uint16_t limite_1:4;
    uint16_t AVL:1;
    uint16_t L:1;
    uint16_t D_B:1;
    uint16_t G:1;
    uint16_t base_1:8;
}descriptor_segmento_t;

__attribute__(( section(".rutinas"))) void cargar_descriptor_segmento(   descriptor_segmento_t *descriptor,
                                                                            uint32_t base, uint32_t limite, uint8_t granularidad,
                                                                            uint8_t default_big, uint8_t L, uint8_t AVL,
                                                                            uint8_t presente, uint8_t DPL, uint8_t sistema,
                                                                            uint8_t b11, uint8_t ED_C, uint8_t W_R,
                                                                            uint8_t accedido)
{
    descriptor->base_0 = (base >> 0 )    & 0x00FFFFFF;
    descriptor->base_1 = (base >> 24)    & 0xFF;

    descriptor->limite_0 = (limite >> 0 )    & 0x0000FFFF;
    descriptor->limite_0 = (limite >> 16)    & 0x0000000F;
    
    descriptor->A = accedido;
    
    descriptor->bit_11 = b11;
    if(b11 == 0)
    {
        descriptor->W   = W_R;
        descriptor->ED  = ED_C;
    }
    else
    {
        descriptor->R = W_R;
        descriptor->C = ED_C;
    }
    
    descriptor->S   = sistema;
    descriptor->DPL = DPL;
    descriptor->P   = presente;
    descriptor->AVL = AVL;
    descriptor->L   = L;
    descriptor->D_B = default_big;
    descriptor->G   = granularidad;
}