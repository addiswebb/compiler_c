	.file	"test.c"
	.text
	.def	__main;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
	.align 8
.LC0:
	.ascii "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d d %d %d %d %d %d %d %d %d %d %d %d %d %d\12\0"
	.text
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$240, %rsp
	.seh_stackalloc	240
	.seh_endprologue
	call	__main
	movl	$28, 224(%rsp)
	movl	$27, 216(%rsp)
	movl	$26, 208(%rsp)
	movl	$25, 200(%rsp)
	movl	$24, 192(%rsp)
	movl	$23, 184(%rsp)
	movl	$22, 176(%rsp)
	movl	$21, 168(%rsp)
	movl	$20, 160(%rsp)
	movl	$19, 152(%rsp)
	movl	$18, 144(%rsp)
	movl	$17, 136(%rsp)
	movl	$16, 128(%rsp)
	movl	$15, 120(%rsp)
	movl	$14, 112(%rsp)
	movl	$13, 104(%rsp)
	movl	$12, 96(%rsp)
	movl	$11, 88(%rsp)
	movl	$10, 80(%rsp)
	movl	$9, 72(%rsp)
	movl	$8, 64(%rsp)
	movl	$7, 56(%rsp)
	movl	$6, 48(%rsp)
	movl	$5, 40(%rsp)
	movl	$4, 32(%rsp)
	movl	$3, %r9d
	movl	$2, %r8d
	movl	$1, %edx
	leaq	.LC0(%rip), %rax
	movq	%rax, %rcx
	call	printf
	movl	$0, %eax
	addq	$240, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.ident	"GCC: (x86_64-posix-seh-rev0, Built by MinGW-Builds project) 13.2.0"
	.def	printf;	.scl	2;	.type	32;	.endef
