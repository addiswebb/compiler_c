.section .rodata
.LC0:
    .string "Hello %s"
.LC1:
    .string "World"

.text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp
main_0:
    lea .LC0(%rip), %rax
    movq %rax, -8(%rbp)
    lea .LC1(%rip), %rax
    movq %rax, -16(%rbp)
    subq $48, %rsp
    movq -8(%rbp), %rcx
    movq -16(%rbp), %rdx
    call printf
    addq $48, %rsp
    movl %eax, -8(%rbp)
    movl $0, %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
