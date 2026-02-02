.section .rodata
.align 8
.LC1:
    .quad 0x400c000000000000
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $64, %rsp
main_0:
    movl $2, %eax
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
    cvttss2si -24(%rbp), %eax
    movl %eax, -56(%rbp)
    movl $1, %eax
    movl %eax, -64(%rbp)
    movl -56(%rbp), %eax
    addl -64(%rbp), %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -40(%rbp)
    movl $2, %eax
    movl %eax, -56(%rbp)
    movl -40(%rbp), %eax
    imull -56(%rbp)
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -40(%rbp)
    movl $1, %eax
    movl %eax, -56(%rbp)
    movl -8(%rbp), %eax
    addl -56(%rbp), %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
