USE32

section .rutinas

global sumatoria_uint16_saturada, sumatoria_uint64_saturada

;uint64_t sumatoria_uint16_saturada(uint64_t datos[], uint32_t cantidad);
sumatoria_uint16_saturada:
;realiza la suma aritmetica saturada en words
;paddusw
    mov eax,[esp + 4 * 1]  ;datos[] 
    mov ecx,[esp + 4 * 2]  ;cantidad

    pxor mm0,mm0
ciclo_16:
    paddusw mm0, qword[eax]
    add eax,8
    dec ecx
    jnz ciclo_16

    ;Si termine de sumar retorno el valor
    ;copio en edx_eax el contenido del registro mm0
    movd  eax,mm0
    psrlq mm0,32
    movd  edx,mm0

    ;pxor mm0,mm0
    ret

;uint64_t sumatoria_uint64_saturada(uint64_t datos[], uint32_t cantidad);
sumatoria_uint64_saturada:
;realiza la suma aritmetica saturada en words
;paddq
    mov eax,[esp + 4 * 1]  ;datos[] 
    mov ecx,[esp + 4 * 2]  ;cantidad

    pxor mm0,mm0
ciclo_64:
    paddq mm0, qword[eax]
    add eax,8
    dec ecx
    jnz ciclo_64

    ;Si termine de sumar retorno el valor
    ;copio en edx_eax el contenido del registro mm0
    movd  eax,mm0
    psrlq mm0,32
    movd  ebx,mm0

    ;pxor mm0,mm0
    ret