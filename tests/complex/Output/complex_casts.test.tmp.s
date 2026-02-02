.section .rodata
.align 8
.LC0:
    .quad 0x4016cccccccccccd
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $64, %rsp
main_0:
    movsd .LC0(%rip), %xmm0
    movsd %xmm0, -56(%rbp)
    movsd -56(%rbp), %xmm0
    movsd %xmm0, -8(%rbp)
    cvttsd2si -8(%rbp), %eax
    movl %eax, -56(%rbp)
    movl $2, %eax
    movl %eax, -64(%rbp)
    movl -56(%rbp), %eax
    imull -64(%rbp)
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -24(%rbp)
    movslq -24(%rbp), %rax
    cvtsi2ss %rax, %xmm0
    movss %xmm0, -56(%rbp)
    cvttss2si -56(%rbp), %eax
    movl %eax, -56(%rbp)
    movl $4, %eax
    movl %eax, -64(%rbp)
    movl -56(%rbp), %eax
    cltd
    idivl -64(%rbp)
    movl %eax, -56(%rbp)
    movslq -56(%rbp), %rax
    cvtsi2ss %rax, %xmm0
    movss %xmm0, -56(%rbp)
    movss -56(%rbp), %xmm0
    movss %xmm0, -40(%rbp)
    cvttss2si -40(%rbp), %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    addl -24(%rbp), %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
