.section .rodata
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $64, %rsp
main_0:
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movq %rax, -8(%rbp)
    movl $0, %eax
    movl %eax, -24(%rbp)
    movl $1, %eax
    movl %eax, -32(%rbp)
    movq -8(%rbp), %rax
    addq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movb (%rax), %al
    movb %al, -48(%rbp)
    movzbl -48(%rbp), %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
