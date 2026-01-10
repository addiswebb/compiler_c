.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $0, %rsp
block_0:
    movl $10, -16(%rbp)
    movl -16(%rbp), %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    jmp return
return:
    mov %rbp, %rsp
    pop %rbp
    ret
