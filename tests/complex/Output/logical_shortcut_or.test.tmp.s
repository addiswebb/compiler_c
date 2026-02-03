.section .rodata
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $64, %rsp
main_0:
    movl $0, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $1, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -24(%rbp)
    movl $0, %eax
    movl %eax, -40(%rbp)
    movl -8(%rbp), %eax
    cmpl -40(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -48(%rbp)
    movl -48(%rbp), %eax
    testl %eax, %eax
    movl $1, %eax
    movl %eax, -56(%rbp)
    movl -24(%rbp), %eax
    movl %eax, -64(%rbp)
    movl -24(%rbp), %eax
    addl -56(%rbp), %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    cmpl -40(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -48(%rbp), %eax
    orl -40(%rbp), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz main_2
main_1:
    movl -24(%rbp), %eax
    movl %eax, -8(%rbp)
    jmp main_2
main_2:
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
