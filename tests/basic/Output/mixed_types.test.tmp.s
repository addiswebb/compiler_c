.section .rodata
.align 8
.LC1:
    .quad 0x4004000000000000
.align 8
.LC2:
    .quad 0x4010000000000000
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $64, %rsp
main_0:
    movl $10, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -8(%rbp)
    movsd .LC1(%rip), %xmm0
    movsd %xmm0, -56(%rbp)
    movsd -56(%rbp), %xmm0
    cvtsd2ss %xmm0, %xmm1
    movss %xmm1, -56(%rbp)
    movss -56(%rbp), %xmm0
    movss %xmm0, -24(%rbp)
    movsd .LC2(%rip), %xmm0
    movsd %xmm0, -56(%rbp)
    movsd -56(%rbp), %xmm0
    movsd %xmm0, -40(%rbp)
    cvttss2si -24(%rbp), %eax
    movl %eax, -56(%rbp)
    movl -8(%rbp), %eax
    addl -56(%rbp), %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -8(%rbp)
    movss -24(%rbp), %xmm0
    cvtss2sd %xmm0, %xmm1
    movsd %xmm1, -56(%rbp)
    movsd -40(%rbp), %xmm0
    divsd -56(%rbp), %xmm0
    movsd %xmm0, -56(%rbp)
    movsd -56(%rbp), %xmm0
    movsd %xmm0, -40(%rbp)
    cvttsd2si -40(%rbp), %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    addl -8(%rbp), %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
