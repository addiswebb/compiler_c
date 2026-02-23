.section .rodata

.text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp
main_0:
    movl $97, %eax
    movb %al, -8(%rbp)
    movsbl -8(%rbp), %eax
    movl %eax, -8(%rbp)
    subq $48, %rsp
    movl -8(%rbp), %ecx
    call putchar
    addq $48, %rsp
    movl %eax, -8(%rbp)
    movl $0, %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
