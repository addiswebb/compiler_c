	.file	"test.c"
	.text
	.globl	cmp_int
cmp_int:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	movq	%rcx, 16(%rbp)
	movq	%rdx, 24(%rbp)
	movq	16(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -4(%rbp)
	movq	24(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -8(%rbp)
	movl	-4(%rbp), %eax
	cmpl	-8(%rbp), %eax
	jge	.L2
	movl	$-1, %eax
	jmp	.L3
.L2:
	movl	-4(%rbp), %eax
	cmpl	-8(%rbp), %eax
	jle	.L4
	movl	$1, %eax
	jmp	.L3
.L4:
	movl	$0, %eax
.L3:
	addq	$16, %rsp
	popq	%rbp
	ret
.LC0:
	.ascii "%d \0"
	.text
	.globl	main
main:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$64, %rsp
	call	__main
	movl	$5, -32(%rbp)
	movl	$1, -28(%rbp)
	movl	$4, -24(%rbp)
	movl	$2, -20(%rbp)
	movl	$3, -16(%rbp)
	movl	$5, -8(%rbp)
	movl	-8(%rbp), %edx
	leaq	-32(%rbp), %rax
	leaq	cmp_int(%rip), %r9
	movl	$4, %r8d
	movq	%rax, %rcx
	call	qsort
	movl	$0, -4(%rbp)
	jmp	.L6
.L7:
	movl	-4(%rbp), %eax
	cltq
	movl	-32(%rbp,%rax,4), %eax
	movl	%eax, %edx
	leaq	.LC0(%rip), %rax
	movq	%rax, %rcx
	call	printf
	addl	$1, -4(%rbp)
.L6:
	movl	-4(%rbp), %eax
	cmpl	-8(%rbp), %eax
	jl	.L7
	movl	$10, %ecx
	call	putchar
	movl	$0, %eax
	addq	$64, %rsp
	popq	%rbp
	ret
