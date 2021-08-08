;------------------------------------------------------------------------------------------------------------
;		init_pic
;
;	Funcion: 	Inicializa el pic en cascada y le asigna el rango de tipo de interrupcion de 0x20 a 0x27 
;				y de 0x28 a 0x2F respectivamente.
;				
;------------------------------------------------------------------------------------------------------------
global init_pic, pic_deshabilitar_todo, pic_limpiar_interrupcion, pic_limpiar_interrupciones

FIN_INTERRUPCION_NO_ESPECIFICO  EQU   0x20
DIRECCION_OCW2                  EQU   0x20
PIC_PUERTO_MASCARAS             EQU   0x21


section .rutinas

;Inicializa el pic
init_pic:
  ;// Inicializaci�n PIC N�1
    ;//ICW1:
    mov     al,0x11;//Establece IRQs activas x flanco, Modo cascada, e ICW4
    out     0x20,al
    ;//ICW2:
    mov     al,0x20   ;//Establece para el PIC#1 el valor base del Tipo de INT que recibi� en el registro BH = 0x20
    out     0x21,al
    ;//ICW3:
    mov     al,0x04;//Establece PIC#1 como Master, e indica que un PIC Slave cuya Interrupci�n ingresa por IRQ2
    out     0x21,al
    ;//ICW4
    mov     al,0x01;// Establece al PIC en Modo 8086
    out     0x21,al
  ;//Antes de inicializar el PIC N�2, deshabilitamos las Interrupciones del PIC1
    mov     al,0xFD   ;0x11111101
    out     0x21,al
  ;//Ahora inicializamos el PIC N�2
    ;//ICW1
    mov     al,0x11;//Establece IRQs activas x flanco, Modo cascada, e ICW4
    out     0xA0,al
    ;//ICW2
    mov     al,0x28    ;//Establece para el PIC#2 el valor base del Tipo de INT que recibi� en el registro BL = 0x28
    out     0xA1,al
    ;//ICW3
    mov     al,0x02;//Establece al PIC#2 como Slave, y le indca que ingresa su Interrupci�n al Master por IRQ2
    out     0xA1,al
    ;//ICW4
    mov     al,0x01;// Establece al PIC en Modo 8086
    out     0xA1,al
  ;Enmascaramos el resto de las Interrupciones (las del PIC#2)
    mov     al,0xFF
    out     0xA1,al
  ; Habilitamos la interrupcion del timer.
    mov     al,0xFC   ;0x11111100
    out     0x21,al
    ret

;Deshabilita todas las interrupciones del pic
pic_deshabilitar_todo:

  pushad

  ;Enmascaramos todas las Interrupciones (las del PIC#1)
  mov     al,0xFF   ;0x11111111
  out     0x21,al

  ;Enmascaramos el resto de las Interrupciones (las del PIC#2), aunque este enmascarado todo el el PIC#1
  out     0xA1,al

  popad
  ret

;Limpia la interrupcion del pic de forma no especifica
pic_limpiar_interrupciones:

  pushad
  
  xor eax, eax

  mov al             , FIN_INTERRUPCION_NO_ESPECIFICO
  out DIRECCION_OCW2 , al

  popad
  ret

;Realiza el fin de interrupcion especifico
pic_limpiar_interrupcion:
  push ebp
  mov ebp, esp

  pushad
  xor eax, eax
  mov eax, [ebp + 4*2]
  and eax, 0x07
  or eax, 0x60
  out DIRECCION_OCW2, al

  popad
  pop ebp
  ret

;Lee el estado de las mascaras del pic
pic_leer_mascaras:
  push edx
  xor eax, eax
  mov edx, PIC_PUERTO_MASCARAS
  in  al, dx
  pop edx
  ret
