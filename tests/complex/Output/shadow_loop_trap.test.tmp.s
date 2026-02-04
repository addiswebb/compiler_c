.section .rodata
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $64, %rsp
main_0:
    movl $3, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $0, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -24(%rbp)
main_1:
    movl $2, %eax
    movl %eax, -56(%rbp)
    movl -24(%rbp), %eax
    cmpl -56(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    testl %eax, %eax
    jz main_6
main_2:
    movl $1, %eax
    movl %eax, -56(%rbp)
    movl -24(%rbp), %eax
    addl -56(%rbp), %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -40(%rbp)
    movl $1, %eax
    movl %eax, -56(%rbp)
    movl -40(%rbp), %eax
    cmpl -56(%rbp), %eax
    setg %al
    movzbl %al, %eax
    movl %eax, -56(%rbp)
    movl $0, %eax
    movl %eax, -64(%rbp)
    movl -56(%rbp), %eax
    cmpl -64(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    testl %eax, %eax
    jz main_4
    movl $1, %eax
    movl %eax, -56(%rbp)
    movl -24(%rbp), %eax
    cmpl -56(%rbp), %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    cmpl -64(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -56(%rbp)
    movl $0, %eax
    movl %eax, -64(%rbp)
    movl -56(%rbp), %eax
    cmpl -64(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    testl %eax, %eax
    movl $1, %eax
    movl %eax, -56(%rbp)
    movl -40(%rbp), %eax
    cmpl -56(%rbp), %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    cmpl -64(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    testl %eax, %eax
    jz main_4
main_3:
    movl $5, %eax
    movl %eax, -56(%rbp)
    movl -40(%rbp), %eax
    addl -56(%rbp), %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -40(%rbp)
    jmp main_4
main_4:
    jmp main_5
main_5:
    movl $1, %eax
    movl %eax, -56(%rbp)
    movl -24(%rbp), %eax
    movl %eax, -64(%rbp)
    movl -24(%rbp), %eax
    addl -56(%rbp), %eax
    movl %eax, -24(%rbp)
    jmp main_1
main_6:
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
