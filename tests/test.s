.section .rodata
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
main_0:
    movl $1, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movb %al, -40(%rbp)
    movb -40(%rbp), %al
    movb %al, -8(%rbp)
    movl $0, %eax
    movb %al, -40(%rbp)
    movb -40(%rbp), %al
    movb %al, -7(%rbp)
    movl $0, %eax
    movb %al, -40(%rbp)
    movb -40(%rbp), %al
    movb %al, -6(%rbp)
    movl $0, %eax
    movb %al, -40(%rbp)
    movb -40(%rbp), %al
    movb %al, -5(%rbp)
    movl $0, %eax
    movb %al, -40(%rbp)
    movb -40(%rbp), %al
    movb %al, -4(%rbp)
    movl $0, %eax
    movb %al, -40(%rbp)
    movb -40(%rbp), %al
    movb %al, -3(%rbp)
    movl $5, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -24(%rbp)
    movl $0, %eax
    movl %eax, -40(%rbp)
    movslq -40(%rbp), %rax
    movq %rax, -40(%rbp)
    lea -8(%rbp), %rax
    addq -40(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movb (%rax), %al
    movb %al, -40(%rbp)
    movsbl -40(%rbp), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    addl -24(%rbp), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
