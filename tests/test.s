.section .rodata
.LC0:
    .string "hello"
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
main_0:
    lea .LC0(%rip), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rdx
    lea -8(%rbp), %rcx
    mov $6, %r8d
    call memcpy
    movl $1, %eax
    movl %eax, -16(%rbp)
    movslq -16(%rbp), %rax
    movq %rax, -24(%rbp)
    lea -8(%rbp), %rax
    addq -24(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movb (%rax), %al
    movb %al, -24(%rbp)
    movsbl -24(%rbp), %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
