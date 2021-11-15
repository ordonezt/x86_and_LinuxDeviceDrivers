	.arch armv7-a
	.eabi_attribute 28, 1
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 6
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"dsp.c"
	.text
	.global	__aeabi_idiv
	.align	1
	.global	media_q15
	.arch armv7-a
	.syntax unified
	.thumb
	.thumb_func
	.fpu vfpv3-d16
	.type	media_q15, %function
media_q15:
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	;En r0 esta el puntero al resultado
	;En r1 esta el puntero a los datos a promediar
	;En r2 esta la longitud del vector
	push	{r7, lr}				;Pushea el registro r7 y el lr (link register con la direccion de retorno) a la pila
	sub	sp, sp, #24					;Reserva 24 bytes en el stack
	add	r7, sp, #0					;Pone el resultado sp + 0 en r7 (pone el stack en r7)
	str	r0, [r7, #12]				;Guarda r0 (puntero a resultado) en el stack (r7) + 12 bytes
	str	r1, [r7, #8]				;Guarda r1 (puntero a datos) en el stack (r7) + 8 bytes
	str	r2, [r7, #4]				;Guarda r2 (longitud del vector) en el stack (r7) + 4 bytes
	;Aca inicializa la variable acum en 0
	movs	r3, #0					;Pone r3 en 0
	str	r3, [r7, #20]				;Guarda r3 (0) en el stack (r7) + 20 bytes
	;Aca empieza a comparar los punteros con NULL y la longitud con 0. Si da error va a retornar -1
	ldr	r3, [r7, #12]				;Guarda en r3 el contenido del stack + 12 bytes (puntero a resultado)
	cmp	r3, #0						;Compara r3 con 0
	beq	.L2							;Si r3 es 0 salta a .L2
	ldr	r3, [r7, #8]				;Si r3 no era 0 guarda en r3 el contenido del stack + 8 bytes (puntero a datos)
	cmp	r3, #0						;Compara r3 con 0
	beq	.L2							;Si r3 es 0 salta a .L2
	ldr	r3, [r7, #4]				;Si r3 no era 0 guarda en r3 el contenido del stack + 4 bytes (longitud del vector)
	cmp	r3, #0						;Compara r3 con 0
	bne	.L3							;Si no es igual a 0 salta a .L3, si es 0 entra en .L2
.L2:			
	mov	r3, #-1						;Pone -1 en r3
	b	.L4							;Salta a .L4
	;En .L3 esta la inicializacion del for, inicializa "i" en 0
.L3:
	movs	r3, #0					;Pone 0 en r3
	str	r3, [r7, #16]				;Guarda el 0 en el stack + 16 bytes (i = 0)
	b	.L5							;Salta a .L5
.L6:
	ldr	r3, [r7, #16]				;Levanta el valor del contador "i" en r3 (la primera vez vale 0)
	lsls	r3, r3, #1				;Shiftea hacia la izquierda 1 bit (multiplica por 2 el contador)
	ldr	r2, [r7, #8]				;Levanta en r2 el puntero a los datos
	add	r3, r3, r2					;Pone en r3 el proximo dato int16_t del vector
	ldrsh	r3, [r3]				;Guarda en r3 la "halfword" (int16_t) apuntada por r3 (r3 = datos[i])
	mov	r2, r3						;Copia en r2 el resultado anterior (dato 16 bits)
	ldr	r3, [r7, #20]				;Levanta en r3 el valor del acumulador (la primera vez vale 0)
	add	r3, r3, r2					;Suma r3 (acumulador) + r2 (valor extraido del vector en la linea 61) (r3 = acum + datos[i])
	str	r3, [r7, #20]				;Guarda en el acumulador esta suma (acum = r3)
	;Aca incrementa el contador (i++)
	ldr	r3, [r7, #16]				;Levanta el valor del contador "i" en r3
	adds	r3, r3, #1				;Incrementa en 1 el contador
	str	r3, [r7, #16]				;Guarda el valor del contador incrementado
.L5:
	ldr	r2, [r7, #16]				;Levanta en r2 el valor del contador (r2 = i)
	ldr	r3, [r7, #4]				;Levanta en r3 el parametro "longitud" (r3 = longitud)
	cmp	r2, r3						;Compara el contador con la longitud del vector (i == longitud)
	blt	.L6							;No se como salta a .L6, pero tiene que ser si i < longitud
	;Si llegue aca es por que i = longitud
	;Significa que el for termino y es tiempo de dividir el acumulador
	ldr	r1, [r7, #4]				;Guarda en r1 la longitud del vector (r1 = longitud)
	ldr	r0, [r7, #20]				;Guarda en r0 el valor del acumulador (r0 = acum)
	bl	__aeabi_idiv(PLT)			;Llama a una subrutina para dividir wtf ¿?
	mov	r3, r0						;Guarda el resultado de la division (retornado en r0) en r3 (r3 = division(acumulador, longitud))
	sxth	r2, r3					;Extiende el signo de 16 a 32 bits y copia el resultado en r2
	ldr	r3, [r7, #12]				;Levanta el puntero al resultado (parametro de entrada) en r3
	strh	r2, [r3]	@ movhi		;Guarda el resultado en la direccion apuntada por el puntero "resultado"
	;Codigo de retorno por exito
	movs	r3, #0					;return 0	
;En .L4 esta el codigo de retorno, es el fin de la funcion
.L4:
	mov	r0, r3						;Mueve r3 a r0 (Valor de retorno)
	adds	r7, r7, #24				;Vuelve el stack a su valor original
	mov	sp, r7						;Recupera el stack almacenado en r7
	@ sp needed						;???
	pop	{r7, pc}					;Pushea recuperando el registro r7 y pone el lr en el pc
	.size	media_q15, .-media_q15
	.align	1
	.global	bloque_ganancia_polarizacion_float
	.syntax unified
	.thumb
	.thumb_func
	.fpu vfpv3-d16
	.type	bloque_ganancia_polarizacion_float, %function
bloque_ganancia_polarizacion_float:
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7}
	sub	sp, sp, #36
	add	r7, sp, #0
	str	r0, [r7, #20]
	str	r1, [r7, #16]
	vstr.32	s0, [r7, #12]
	vstr.32	s1, [r7, #8]
	str	r2, [r7, #4]
	ldr	r3, [r7, #20]
	cmp	r3, #0
	beq	.L8
	ldr	r3, [r7, #16]
	cmp	r3, #0
	bne	.L9
.L8:
	mov	r3, #-1
	b	.L10
.L9:
	movs	r3, #0
	str	r3, [r7, #28]
	b	.L11
.L12:
	ldr	r3, [r7, #28]
	lsls	r3, r3, #2
	ldr	r2, [r7, #20]
	add	r3, r3, r2
	vldr.32	s14, [r3]
	vldr.32	s15, [r7, #12]
	vmul.f32	s14, s14, s15
	ldr	r3, [r7, #28]
	lsls	r3, r3, #2
	ldr	r2, [r7, #16]
	add	r3, r3, r2
	vldr.32	s15, [r7, #8]
	vadd.f32	s15, s14, s15
	vstr.32	s15, [r3]
	ldr	r3, [r7, #28]
	adds	r3, r3, #1
	str	r3, [r7, #28]
.L11:
	ldr	r2, [r7, #28]
	ldr	r3, [r7, #4]
	cmp	r2, r3
	blt	.L12
	movs	r3, #0
.L10:
	mov	r0, r3
	adds	r7, r7, #36
	mov	sp, r7
	@ sp needed
	ldr	r7, [sp], #4
	bx	lr
	.size	bloque_ganancia_polarizacion_float, .-bloque_ganancia_polarizacion_float
	.align	1
	.global	q15_to_float
	.syntax unified
	.thumb
	.thumb_func
	.fpu vfpv3-d16
	.type	q15_to_float, %function
q15_to_float:
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7}
	sub	sp, sp, #28
	add	r7, sp, #0
	str	r0, [r7, #12]
	str	r1, [r7, #8]
	str	r2, [r7, #4]
	ldr	r3, [r7, #12]
	cmp	r3, #0
	beq	.L14
	ldr	r3, [r7, #8]
	cmp	r3, #0
	bne	.L15
.L14:
	mov	r3, #-1
	b	.L16
.L15:
	movs	r3, #0
	str	r3, [r7, #20]
	b	.L17
.L18:
	ldr	r3, [r7, #20]
	lsls	r3, r3, #1
	ldr	r2, [r7, #12]
	add	r3, r3, r2
	ldrsh	r1, [r3]
	ldr	r3, [r7, #20]
	lsls	r3, r3, #2
	ldr	r2, [r7, #8]
	add	r3, r3, r2
	vmov	s15, r1	@ int
	vcvt.f32.s32	s15, s15
	vstr.32	s15, [r3]
	ldr	r3, [r7, #20]
	adds	r3, r3, #1
	str	r3, [r7, #20]
.L17:
	ldr	r2, [r7, #20]
	ldr	r3, [r7, #4]
	cmp	r2, r3
	blt	.L18
	movs	r3, #0
.L16:
	mov	r0, r3
	adds	r7, r7, #28
	mov	sp, r7
	@ sp needed
	ldr	r7, [sp], #4
	bx	lr
	.size	q15_to_float, .-q15_to_float
	.ident	"GCC: (Debian 8.3.0-6) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
