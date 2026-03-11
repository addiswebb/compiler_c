.section .rodata
.LC0:
    .byte '%', 'l', 'f', ' ', '%', 'd', ' ', '%', 'l', 'f', 0x0A, 0
.align 8
.LC1:
    .quad 0x3ff0000000000000
.align 8
.LC3:
    .quad 0x4008000000000000

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
main_0:
    lea .LC0(%rip), %rax
    movq %rax, -8(%rbp)
    movsd .LC1(%rip), %xmm0
    movsd %xmm0, -16(%rbp)
    movl $2, %eax
    movl %eax, -24(%rbp)
    movsd .LC3(%rip), %xmm0
    movsd %xmm0, -32(%rbp)
    subq $48, %rsp
    movq -8(%rbp), %rcx
    movq -16(%rbp), %rdx
    movsd -16(%rbp), %xmm0
    movl -24(%rbp), %r8d
    movq -32(%rbp), %r9
    movsd -32(%rbp), %xmm1
    call printf
    addq $48, %rsp
    movl %eax, -8(%rbp)
    movl $0, %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
