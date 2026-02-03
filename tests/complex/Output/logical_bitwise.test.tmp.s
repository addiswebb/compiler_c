.section .rodata
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $96, %rsp
main_0:
    movl $5, %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $3, %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    movl %eax, -24(%rbp)
    movl -8(%rbp), %eax
    andl -24(%rbp), %eax
    movl %eax, -72(%rbp)
    movl -8(%rbp), %eax
    xorl -24(%rbp), %eax
    movl %eax, -80(%rbp)
    movl -72(%rbp), %eax
    orl -80(%rbp), %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    movl %eax, -40(%rbp)
    movl -8(%rbp), %eax
    cmpl -24(%rbp), %eax
    setg %al
    movzbl %al, %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    testl %eax, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -72(%rbp)
    movl $0, %eax
    movl %eax, -80(%rbp)
    movl -72(%rbp), %eax
    cmpl -80(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -72(%rbp)
    movl $1, %eax
    movl %eax, -88(%rbp)
    movl -24(%rbp), %eax
    andl -88(%rbp), %eax
    movl %eax, -88(%rbp)
    movl -88(%rbp), %eax
    cmpl -80(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -80(%rbp)
    movl -72(%rbp), %eax
    orl -80(%rbp), %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    movl %eax, -56(%rbp)
    movl -40(%rbp), %eax
    addl -56(%rbp), %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
