.section .note.GNU-stack,"",@progbits
.section .rodata
.LC0:
    .byte 0xAA, 0

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
main_0:
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    leaq .LC0(%rip), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rsi
    movq -16(%rbp), %rdi
    mov $2, %rdx
    call memcpy
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq $0, %rax
    imulq $1, %rax
    movq %rax, -24(%rbp)
    movq -16(%rbp), %rax
    addq -24(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movb (%rax), %al
    movb %al, -16(%rbp)
    movsbl -16(%rbp), %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
