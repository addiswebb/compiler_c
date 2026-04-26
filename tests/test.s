.section .note.GNU-stack,"",@progbits

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp
main_0:
    movl $5, %eax
    addl $0, %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    addl $1, %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
