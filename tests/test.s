.section .rodata
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp
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
    movl -8(%rbp), %eax
    testl %eax, %eax
    jz main_2
    movl $0, %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    cmpl -16(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -8(%rbp)
    movl $0, %eax
    movl %eax, -16(%rbp)
    movl -8(%rbp), %eax
    cmpl -16(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    testl %eax, %eax
    jz main_2
    movl $1, %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    cmpl -16(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    testl %eax, %eax
    jz main_2
    jnz main_1
main_1:
    movl $5, %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp main_2
main_2:
    movl $3, %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
