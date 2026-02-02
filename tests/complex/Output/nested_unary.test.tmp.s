.section .rodata
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
main_0:
    movl $5, %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    negl %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    negl %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    notl %eax
    movl %eax, -24(%rbp)
    movl $1, %eax
    movl %eax, -32(%rbp)
    movl -24(%rbp), %eax
    addl -32(%rbp), %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $1, %eax
    movl %eax, -24(%rbp)
    movl -8(%rbp), %eax
    movl %eax, -32(%rbp)
    movl -8(%rbp), %eax
    addl -24(%rbp), %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
