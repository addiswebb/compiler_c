.global main
    jmp main
main:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
main_0:
    movl $1, %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $1, %eax
    movl %eax, -24(%rbp)
    movl -8(%rbp), %eax
    movl %eax, -32(%rbp)
    movl -8(%rbp), %eax
    subl -24(%rbp), %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    testl %eax, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
