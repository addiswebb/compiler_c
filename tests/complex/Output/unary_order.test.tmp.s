.section .rodata
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $64, %rsp
main_0:
    movl $1, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $1, %eax
    movl %eax, -40(%rbp)
    movl -8(%rbp), %eax
    movl %eax, -48(%rbp)
    movl -8(%rbp), %eax
    addl -40(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $1, %eax
    movl %eax, -40(%rbp)
    movl -8(%rbp), %eax
    movl %eax, -56(%rbp)
    movl -8(%rbp), %eax
    addl -40(%rbp), %eax
    movl %eax, -8(%rbp)
    movl -48(%rbp), %eax
    addl -8(%rbp), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    addl -8(%rbp), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -24(%rbp)
    movl $10, %eax
    movl %eax, -40(%rbp)
    movl -24(%rbp), %eax
    imull -40(%rbp)
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    addl -8(%rbp), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
