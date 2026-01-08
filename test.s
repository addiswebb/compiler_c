.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp

    movl $5, -16(%rbp)
    movl $5, -24(%rbp)
    movl -16(%rbp), %eax
    imull -24(%rbp)
    movl %eax, -32(%rbp)
    movl $5, -40(%rbp)
    movl -32(%rbp), %eax
    idivl -40(%rbp)
    movl %eax, -48(%rbp)
    movl -48(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $10, -64(%rbp)
    movl -8(%rbp), %eax
    imull -64(%rbp)
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    movl %eax, -56(%rbp)
    movl $5, -88(%rbp)
    movl -56(%rbp), %eax
    idivl -88(%rbp)
    movl %eax, -96(%rbp)
    movl -96(%rbp), %eax
    movl %eax, -80(%rbp)
    movl $0, -104(%rbp)
    movl -80(%rbp), %eax
    subl -104(%rbp), %eax
    movl %eax, -112(%rbp)
    movl -112(%rbp), %eax

    mov %rbp, %rsp
    pop %rbp
    ret
