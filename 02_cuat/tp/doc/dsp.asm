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
	push	{r7, lr}
	sub	sp, sp, #24
	add	r7, sp, #0
	str	r0, [r7, #12]
	str	r1, [r7, #8]
	str	r2, [r7, #4]
	movs	r3, #0
	str	r3, [r7, #20]
	ldr	r3, [r7, #12]
	cmp	r3, #0
	beq	.L2
	ldr	r3, [r7, #8]
	cmp	r3, #0
	beq	.L2
	ldr	r3, [r7, #4]
	cmp	r3, #0
	bne	.L3
.L2:
	mov	r3, #-1
	b	.L4
.L3:
	movs	r3, #0
	str	r3, [r7, #16]
	b	.L5
.L6:
	ldr	r3, [r7, #16]
	lsls	r3, r3, #1
	ldr	r2, [r7, #8]
	add	r3, r3, r2
	ldrsh	r3, [r3]
	mov	r2, r3
	ldr	r3, [r7, #20]
	add	r3, r3, r2
	str	r3, [r7, #20]
	ldr	r3, [r7, #16]
	adds	r3, r3, #1
	str	r3, [r7, #16]
.L5:
	ldr	r2, [r7, #16]
	ldr	r3, [r7, #4]
	cmp	r2, r3
	blt	.L6
	ldr	r1, [r7, #4]
	ldr	r0, [r7, #20]
	bl	__aeabi_idiv(PLT)
	mov	r3, r0
	sxth	r2, r3
	ldr	r3, [r7, #12]
	strh	r2, [r3]	@ movhi
	movs	r3, #0
.L4:
	mov	r0, r3
	adds	r7, r7, #24
	mov	sp, r7
	@ sp needed
	pop	{r7, pc}
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
