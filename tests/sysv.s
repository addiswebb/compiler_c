	.file	"test.c"
	.text
	.globl	sum_mix
	.type	sum_mix, @function
sum_mix:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$240, %rsp
	movl	%edi, -228(%rbp)
	movq	%rsi, -168(%rbp)
	movq	%rdx, -160(%rbp)
	movq	%rcx, -152(%rbp)
	movq	%r8, -144(%rbp)
	movq	%r9, -136(%rbp)
	test	%al, %al
	je	.L2
	movaps	%xmm0, -128(%rbp)
	movaps	%xmm1, -112(%rbp)
	movaps	%xmm2, -96(%rbp)
	movaps	%xmm3, -80(%rbp)
	movaps	%xmm4, -64(%rbp)
	movaps	%xmm5, -48(%rbp)
	movaps	%xmm6, -32(%rbp)
	movaps	%xmm7, -16(%rbp)
.L2:
	movq	%fs:40, %rax
	movq	%rax, -184(%rbp)
	xorl	%eax, %eax
	movl	$8, -208(%rbp)
	movl	$48, -204(%rbp)
	leaq	16(%rbp), %rax
	movq	%rax, -200(%rbp)
	leaq	-176(%rbp), %rax
	movq	%rax, -192(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -216(%rbp)
	movl	$0, -220(%rbp)
	jmp	.L3
.L10:
	movl	-220(%rbp), %eax
	andl	$1, %eax
	testl	%eax, %eax
	jne	.L4
	movl	-208(%rbp), %eax
	cmpl	$47, %eax
	ja	.L5
	movq	-192(%rbp), %rax
	movl	-208(%rbp), %edx
	movl	%edx, %edx
	addq	%rdx, %rax
	movl	-208(%rbp), %edx
	addl	$8, %edx
	movl	%edx, -208(%rbp)
	jmp	.L6
.L5:
	movq	-200(%rbp), %rax
	leaq	8(%rax), %rdx
	movq	%rdx, -200(%rbp)
.L6:
	movl	(%rax), %eax
	pxor	%xmm0, %xmm0
	cvtsi2sdl	%eax, %xmm0
	movsd	-216(%rbp), %xmm1
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -216(%rbp)
	jmp	.L7
.L4:
	movl	-204(%rbp), %eax
	cmpl	$175, %eax
	ja	.L8
	movq	-192(%rbp), %rax
	movl	-204(%rbp), %edx
	movl	%edx, %edx
	addq	%rdx, %rax
	movl	-204(%rbp), %edx
	addl	$16, %edx
	movl	%edx, -204(%rbp)
	jmp	.L9
.L8:
	movq	-200(%rbp), %rax
	leaq	8(%rax), %rdx
	movq	%rdx, -200(%rbp)
.L9:
	movsd	(%rax), %xmm0
	movsd	-216(%rbp), %xmm1
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -216(%rbp)
.L7:
	addl	$1, -220(%rbp)
.L3:
	movl	-220(%rbp), %eax
	cmpl	-228(%rbp), %eax
	jl	.L10
	movsd	-216(%rbp), %xmm0
	movq	-184(%rbp), %rax
	subq	%fs:40, %rax
	je	.L12
	call	__stack_chk_fail@PLT
.L12:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	sum_mix, .-sum_mix
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movsd	.LC1(%rip), %xmm0
	movq	.LC2(%rip), %rax
	movapd	%xmm0, %xmm1
	movl	$3, %edx
	movq	%rax, %xmm0
	movl	$1, %esi
	movl	$4, %edi
	movl	$2, %eax
	call	sum_mix
	cvttsd2sil	%xmm0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC1:
	.long	0
	.long	1074921472
	.align 8
.LC2:
	.long	0
	.long	1074003968
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
