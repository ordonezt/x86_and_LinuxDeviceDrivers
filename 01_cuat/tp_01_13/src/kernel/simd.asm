USE32

section .rutinas

global sumatoria_uint16_saturada, sumatoria_uint64_saturada
global guardar_registros_simd, restaurar_registros_simd
global habilitar_NM

extern borrar_cr0_ts

;uint64_t sumatoria_uint16_saturada(uint64_t datos[], uint32_t cantidad);
sumatoria_uint16_saturada:
;realiza la suma aritmetica saturada en words
;paddusw
    push ecx

    mov eax,[esp + 4 * 2]  ;datos[] 
    mov ecx,[esp + 4 * 3]  ;cantidad

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
    pop ecx
    ret

;uint64_t sumatoria_uint64_saturada(uint64_t datos[], uint32_t cantidad);
sumatoria_uint64_saturada:
;realiza la suma aritmetica saturada en words
;paddq
    push ecx

    mov eax,[esp + 4 * 2]  ;datos[] 
    mov ecx,[esp + 4 * 3]  ;cantidad

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
    movd  edx,mm0

    ;pxor mm0,mm0
    pop ecx
    ret

;void guardar_registros_simd(contexto_simd_t *contexto);
guardar_registros_simd:
    push    edi
    mov     edi, [esp + 4 * 2]
    fxsave  [edi]
    pop     edi
    ret

;void restaurar_registros_simd(contexto_simd_t *contexto);
restaurar_registros_simd:
    push    edi
    mov     edi, [esp + 4 * 2]
    fxrstor [edi]
    pop     edi
    ret

habilitar_NM:
    push eax

    ;CR0.EM = 0
    mov eax, cr0
    and  eax, 0xFFFFFFFB
    mov cr0, eax

    ;CR0.TS = 0
    call borrar_cr0_ts

    ;CR4.OSFXSR = 1
    mov  eax, cr4
    or   eax, 0x200
    mov  cr4, eax

    pop eax
    ret