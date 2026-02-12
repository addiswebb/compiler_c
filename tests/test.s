.section .rodata

.text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
main_0:
    movl $1, %eax
    movl %eax, -8(%rbp)
    movl $2, %eax
    movl %eax, -16(%rbp)
    movl $3, %eax
    movl %eax, -24(%rbp)
    movl $4, %eax
    movl %eax, -32(%rbp)
    movl $5, %eax
    movl %eax, -40(%rbp)
    subq $48, %rsp
    movl -8(%rbp), %ecx
    movl -16(%rbp), %edx
    movl -24(%rbp), %r8d
    movl -32(%rbp), %r9d
    movl -40(%rbp), %eax
    movl %eax, 32(%rsp)
    call test
    addq $48, %rsp
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
