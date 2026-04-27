.section .note.GNU-stack,"",@progbits
.section .rodata
.LC0:
    .byte '%', 's', 0x0A, 0

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
main_0:
    movl %edi, -8(%rbp)
    movq %rsi, -16(%rbp)
    leaq .LC0(%rip), %rax
    movq %rax, -24(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -32(%rbp)
    movq $0, %rax
    imulq $8, %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    addq -40(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -32(%rbp)
    subq $176, %rsp
    movq -24(%rbp), %rdi
    movq -32(%rbp), %rsi
    xor %eax, %eax
    call printf
    addq $176, %rsp
    movl %eax, -24(%rbp)
    movl $0, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
