.section .rodata
.align 8
.LC0:
    .quad 0x402d000000000000
.LC1:
    .byte 'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', 0x0A, 0

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp
main_0:
    movsd .LC0(%rip), %xmm0
    movsd %xmm0, -8(%rbp)
    leaq .LC1(%rip), %rax
    movq %rax, -16(%rbp)
    subq $32, %rsp
    movl $12, %ecx
    movsd -8(%rbp), %xmm1
    movq -16(%rbp), %r8
    call foo
    addq $32, %rsp
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
