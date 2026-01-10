.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $0, %rsp
block_0:
    movl $10, -16(%rbp)
    movl -16(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $10, -32(%rbp)
    movl -32(%rbp), %eax
    movl %eax, -24(%rbp)
block_1:
    movl -24(%rbp), %eax
    testl %eax, %eax
    jz block_3
block_2:
    movl $1, -40(%rbp)
    movl -8(%rbp), %eax
    subl -40(%rbp), %eax
    movl %eax, -48(%rbp)
    movl -48(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $1, -56(%rbp)
    movl -24(%rbp), %eax
    subl -56(%rbp), %eax
    movl %eax, -64(%rbp)
    movl -64(%rbp), %eax
    movl %eax, -24(%rbp)
    jmp block_1
block_3:
    movl -8(%rbp), %eax
    jmp return
return:
    mov %rbp, %rsp
    pop %rbp
    ret
