.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp

    movl $1, -16(%rbp)
    movl -16(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $5, -24(%rbp)
    movl -8(%rbp), %eax
    addl -24(%rbp), %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $2, -40(%rbp)
    movl -8(%rbp), %eax
    imull -40(%rbp)
    movl %eax, -48(%rbp)
    movl -48(%rbp), %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax

    mov %rbp, %rsp
    pop %rbp
    ret
