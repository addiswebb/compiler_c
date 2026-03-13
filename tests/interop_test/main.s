.section .rodata

.text
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
    movl $97, %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    movb %al, -24(%rbp)
    movb -24(%rbp), %al
    movb %al, -7(%rbp)
    movl $2, %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    movb %al, -24(%rbp)
    movb -24(%rbp), %al
    movb %al, -6(%rbp)
    subq $48, %rsp
    movq -8(%rbp), %rcx
    call foo
    addq $48, %rsp
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
