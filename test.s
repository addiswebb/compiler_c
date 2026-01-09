.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $0, %rsp

block_0:
    movl $0, -16(%rbp)
    movl -16(%rbp), %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    testl %eax, %eax
    jz block_2
block_1:
    movl $5, -24(%rbp)
    movl -24(%rbp), %eax
    jmp return
    jmp block_3
block_2:
    movl $10, -32(%rbp)
    movl -32(%rbp), %eax
    jmp return
    jmp block_3
block_3:
    movl -8(%rbp), %eax
    jmp return

return:
    mov %rbp, %rsp
    pop %rbp
    ret
.section .note.GNU-stack,"",@progbits
