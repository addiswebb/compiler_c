.section .rodata
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
main_0:
    movb $97, %al
    movb %al, -16(%rbp)
    movb -16(%rbp), %al
    movb %al, -8(%rbp)
    movzbl -8(%rbp), %eax
    movl %eax, -24(%rbp)
    movl $3, %eax
    movl %eax, -32(%rbp)
    movl -24(%rbp), %eax
    addl -32(%rbp), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
