.section .rodata
.LC0:
    .byte '%', 'd', ' ', '%', 'l', 'f', ' ', '%', 's', 0x0A, 0

.text
.global foo
foo:
    push %rbp
    mov %rsp, %rbp
    subq $64, %rsp
foo_0:
    movl %ecx, -8(%rbp)
    movsd %xmm1, -16(%rbp)
    movq %r8, -24(%rbp)
    leaq .LC0(%rip), %rax
    movq %rax, -32(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -48(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -56(%rbp)
    subq $32, %rsp
    movq -32(%rbp), %rcx
    movl -40(%rbp), %edx
    movq -48(%rbp), %r8
    movsd -48(%rbp), %xmm2
    movq -56(%rbp), %r9
    call printf
    addq $32, %rsp
    movl %eax, -32(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -32(%rbp)
    cvtsi2sd -32(%rbp), %xmm0
    movsd %xmm0, -32(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -40(%rbp)
    movsd -32(%rbp), %xmm0
    addsd -40(%rbp), %xmm0
    movsd %xmm0, -32(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -40(%rbp)
    movq $0, %rax
    imulq $1, %rax
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    addq -48(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movb (%rax), %al
    movb %al, -40(%rbp)
    movsbl -40(%rbp), %eax
    movl %eax, -40(%rbp)
    cvtsi2sd -40(%rbp), %xmm0
    movsd %xmm0, -40(%rbp)
    movsd -32(%rbp), %xmm0
    addsd -40(%rbp), %xmm0
    movsd %xmm0, -32(%rbp)
    cvttsd2si -32(%rbp), %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
