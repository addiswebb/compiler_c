.section .rodata

.text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp
main_0:
    movl $5, %eax
    movl %eax, -8(%rbp)
    movl $6, %eax
    movl %eax, -16(%rbp)
    movl -8(%rbp), %eax
    addl -16(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $7, %eax
    movl %eax, -16(%rbp)
    movl -8(%rbp), %eax
    addl -16(%rbp), %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
