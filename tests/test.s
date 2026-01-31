.section .rodata
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
main_0:
    movl $10, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -8(%rbp)
    lea -8(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq %rax, -24(%rbp)
    movl $5, %eax
    movl %eax, -40(%rbp)
    movq -24(%rbp), %rax
    movq -40(%rbp), %rbx
    movq %rbx, (%rax)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
