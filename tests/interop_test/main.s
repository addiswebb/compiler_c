.section .rodata
.LC0:
    .string "hello"

.text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
main_0:
    lea .LC0(%rip), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rdx
    lea -8(%rbp), %rcx
    mov $6, %r8d
    call memcpy
    lea -8(%rbp), %rax
    movq %rax, -24(%rbp)
    subq $48, %rsp
    movq -24(%rbp), %rcx
    call puts
    addq $48, %rsp
    movl %eax, -24(%rbp)
    movl $99, %eax
    movb %al, -24(%rbp)
    movsbl -24(%rbp), %eax
    movl %eax, -24(%rbp)
    subq $48, %rsp
    movl -24(%rbp), %ecx
    call putchar
    addq $48, %rsp
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
