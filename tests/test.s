.section .rodata
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
main_0:
    movl $1, %eax
    movl %eax, -8(%rbp)
    movl $0, %eax
    movl %eax, -16(%rbp)
    movl -8(%rbp), %eax
    cmpl -16(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -8(%rbp)
    movl $0, %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    cmpl -16(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -16(%rbp)
    movl -8(%rbp), %eax
    orl -16(%rbp), %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
