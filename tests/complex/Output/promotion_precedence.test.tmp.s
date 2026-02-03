.section .rodata
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $80, %rsp
main_0:
    movl $120, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movb %al, -56(%rbp)
    movb -56(%rbp), %al
    movb %al, -8(%rbp)
    movl $10, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movb %al, -56(%rbp)
    movb -56(%rbp), %al
    movb %al, -24(%rbp)
    movsbl -8(%rbp), %eax
    movl %eax, -56(%rbp)
    movsbl -24(%rbp), %eax
    movl %eax, -64(%rbp)
    movl -56(%rbp), %eax
    addl -64(%rbp), %eax
    movl %eax, -56(%rbp)
    movl $1, %eax
    movl %eax, -64(%rbp)
    movl -56(%rbp), %eax
    movl -64(%rbp), %ecx
    shll %cl, %eax
    movl %eax, -56(%rbp)
    movl $250, %eax
    movl %eax, -64(%rbp)
    movl -56(%rbp), %eax
    cmpl -64(%rbp), %eax
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
    movsbl -8(%rbp), %eax
    movl %eax, -72(%rbp)
    movl $1, %eax
    movl %eax, -80(%rbp)
    movl -72(%rbp), %eax
    andl -80(%rbp), %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    cmpl -64(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -64(%rbp)
    movl -56(%rbp), %eax
    orl -64(%rbp), %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
