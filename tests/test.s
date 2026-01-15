.section .rodata
.LC0:
    .align 4
    .long 0x42e107d5
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
main_0:
    movss .LC0(%rip), %xmm0
    movss %xmm0, -16(%rbp)
    movss -16(%rbp), %xmm0
    movss %xmm0, -8(%rbp)
    movss -8(%rbp), %xmm0
    cvttss2si %xmm0, %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
