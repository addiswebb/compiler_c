.section .rodata

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
main_0:
    movl $2, %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $1, %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    movl %eax, -4(%rbp)
