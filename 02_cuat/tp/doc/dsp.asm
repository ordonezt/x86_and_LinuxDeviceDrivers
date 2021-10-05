	.file	"dsp.c"
	.text
	.globl	media_q15
	.type	media_q15, @function
media_q15:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movl	%edx, -36(%rbp)
	movl	$0, -8(%rbp)
	cmpq	$0, -24(%rbp)
	je	.L2
	cmpq	$0, -32(%rbp)
	je	.L2
	cmpl	$0, -36(%rbp)
	jne	.L3
.L2:
	movl	$-1, %eax
	jmp	.L4
.L3:
	movl	$0, -4(%rbp)
	jmp	.L5
.L6:
	movl	-4(%rbp), %eax
	cltq
	leaq	(%rax,%rax), %rdx
	movq	-32(%rbp), %rax
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	cwtl
	addl	%eax, -8(%rbp)
	addl	$1, -4(%rbp)
.L5:
	movl	-4(%rbp), %eax
	cmpl	-36(%rbp), %eax
	jl	.L6
	movl	-8(%rbp), %eax
	cltd
	idivl	-36(%rbp)
	movl	%eax, %edx
	movq	-24(%rbp), %rax
	movw	%dx, (%rax)
	movl	$0, %eax
.L4:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	media_q15, .-media_q15
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
