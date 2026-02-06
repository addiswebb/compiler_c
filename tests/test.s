.section .rodata
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
main_0:
    movl $1, %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    movb %al, -24(%rbp)
    movb -24(%rbp), %al
    movb %al, -8(%rbp)
    movl $5, %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    movl %eax, -4(%rbp)
    lea -4(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
