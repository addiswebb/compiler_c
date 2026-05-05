.section .note.GNU-stack,"",@progbits
.section .rodata
.LC0:
    .byte 'H', 'e', 'l', 'o', 'W', 'o', 'r', 'l', 'd', 0x0A, 'W', 'O', 'W', 'O', ' ', 0x0A, ' ', 'W', 'o', 0x0A, 0

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp
main_0:
    leaq .LC0(%rip), %rax
    movq %rax, -8(%rbp)
    subq $176, %rsp
    movq -8(%rbp), %rdi
    xor %eax, %eax
    call printf
    addq $176, %rsp
    movl %eax, -8(%rbp)
    movl $0, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
