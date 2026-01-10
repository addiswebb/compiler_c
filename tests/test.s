.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $112, %rsp
block_0:
    movl $12, -16(%rbp)
    movl -16(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $0, -32(%rbp)
    movl -32(%rbp), %eax
    movl %eax, -24(%rbp)
    movl $1, -48(%rbp)
    movl -48(%rbp), %eax
    movl %eax, -40(%rbp)
    movl $2, -64(%rbp)
    movl -64(%rbp), %eax
    movl %eax, -56(%rbp)
block_1:
    movl -8(%rbp), %eax
    subl -56(%rbp), %eax
    movl %eax, -72(%rbp)
    movl $1, -80(%rbp)
    movl -72(%rbp), %eax
    addl -80(%rbp), %eax
    movl %eax, -88(%rbp)
    movl -88(%rbp), %eax
    testl %eax, %eax
    jz block_3
block_2:
    movl -24(%rbp), %eax
    addl -40(%rbp), %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    movl %eax, -96(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -24(%rbp)
    movl -96(%rbp), %eax
    movl %eax, -40(%rbp)
    movl $1, -96(%rbp)
    movl -56(%rbp), %eax
    addl -96(%rbp), %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    movl %eax, -56(%rbp)
    jmp block_1
block_3:
    movl -40(%rbp), %eax
    jmp return
return:
    mov %rbp, %rsp
    pop %rbp
    ret
