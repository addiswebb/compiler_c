.section .note.GNU-stack,"",@progbits
.section .rodata
.LC0:
    .byte '%', 's', 0x0A, 0
.global str
.data
str:
    .byte 'h', 'e', 'l', 'l', 'o', 0
    .byte 'w', 'o', 'r', 'l', 'd', 0

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
main_0:
    leaq .LC0(%rip), %rax
    movq %rax, -8(%rbp)
    leaq str(%rip), %rax
    movq %rax, -16(%rbp)
    movq $1, %rax
    imulq $6, %rax
    movq %rax, -24(%rbp)
    movq -16(%rbp), %rax
    addq -24(%rbp), %rax
    movq %rax, -16(%rbp)
    subq $176, %rsp
    movq -8(%rbp), %rdi
    movq -16(%rbp), %rsi
    xor %eax, %eax
    call printf
    addq $176, %rsp
    movl %eax, -8(%rbp)
    movl $0, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
