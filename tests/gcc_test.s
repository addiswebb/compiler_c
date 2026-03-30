	.file	"test.c"
	.text
	.globl	sum
sum:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	movl	%ecx, 16(%rbp)
	movq	%rdx, 24(%rbp)
	movq	%r8, 32(%rbp)
	movq	%r9, 40(%rbp)
	movl	$0, -4(%rbp)
	leaq	24(%rbp), %rax
	movq	%rax, -16(%rbp)
	movl	$0, -8(%rbp)
	jmp	.L2
.L3:
	movq	-16(%rbp), %rax
	leaq	8(%rax), %rdx
	movq	%rdx, -16(%rbp)
	movl	(%rax), %eax
	addl	%eax, -4(%rbp)
	addl	$1, -8(%rbp)
.L2:
	movl	-8(%rbp), %eax
	cmpl	16(%rbp), %eax
	jl	.L3
	movl	-4(%rbp), %eax
	addq	$16, %rsp
	popq	%rbp
	ret
	.globl	main
main:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$48, %rsp
	call	__main
	movl	$4, 32(%rsp)
	movl	$3, %r9d
	movl	$2, %r8d
	movl	$1, %edx
	movl	$4, %ecx
	call	sum
	addq	$48, %rsp
	popq	%rbp
	ret
