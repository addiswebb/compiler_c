.section .note.GNU-stack,"",@progbits
.section .rodata
.align 8
.LC0:
    .quad 0xc020000000000000

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
main_0:
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movsd .LC0(%rip), %xmm0
    movsd %xmm0, -24(%rbp)
    movq -16(%rbp), %rax
    movsd -24(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -16(%rbp)
    cvttsd2si -16(%rbp), %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
