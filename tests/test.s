.section .rodata
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $208, %rsp
main_0:
    movl $10, %eax
    movl %eax, -200(%rbp)
    movl $10, %eax
    movl %eax, -208(%rbp)
    movl -200(%rbp), %eax
    cmpl -208(%rbp), %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -200(%rbp)
    movl -200(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $10, %eax
    movl %eax, -200(%rbp)
    movl $5, %eax
    movl %eax, -208(%rbp)
    movl -200(%rbp), %eax
    cmpl -208(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -200(%rbp)
    movl -200(%rbp), %eax
    movl %eax, -24(%rbp)
    movl $3, %eax
    movl %eax, -200(%rbp)
    movl $5, %eax
    movl %eax, -208(%rbp)
    movl -200(%rbp), %eax
    cmpl -208(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -200(%rbp)
    movl -200(%rbp), %eax
    movl %eax, -40(%rbp)
    movl $5, %eax
    movl %eax, -200(%rbp)
    movl $5, %eax
    movl %eax, -208(%rbp)
    movl -200(%rbp), %eax
    cmpl -208(%rbp), %eax
    setle %al
    movzbl %al, %eax
    movl %eax, -200(%rbp)
    movl -200(%rbp), %eax
    movl %eax, -56(%rbp)
    movl $7, %eax
    movl %eax, -200(%rbp)
    movl $3, %eax
    movl %eax, -208(%rbp)
    movl -200(%rbp), %eax
    cmpl -208(%rbp), %eax
    setg %al
    movzbl %al, %eax
    movl %eax, -200(%rbp)
    movl -200(%rbp), %eax
    movl %eax, -72(%rbp)
    movl $5, %eax
    movl %eax, -200(%rbp)
    movl $5, %eax
    movl %eax, -208(%rbp)
    movl -200(%rbp), %eax
    cmpl -208(%rbp), %eax
    setge %al
    movzbl %al, %eax
    movl %eax, -200(%rbp)
    movl -200(%rbp), %eax
    movl %eax, -88(%rbp)
    movl $2, %eax
    movl %eax, -200(%rbp)
    movl -200(%rbp), %eax
    negl %eax
    movl %eax, -200(%rbp)
    movl $0, %eax
    movl %eax, -208(%rbp)
    movl -200(%rbp), %eax
    cmpl -208(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -200(%rbp)
    movl -200(%rbp), %eax
    movl %eax, -104(%rbp)
    movl $1, %eax
    movl %eax, -200(%rbp)
    movl -200(%rbp), %eax
    negl %eax
    movl %eax, -200(%rbp)
    movl $5, %eax
    movl %eax, -208(%rbp)
    movl -208(%rbp), %eax
    negl %eax
    movl %eax, -208(%rbp)
    movl -200(%rbp), %eax
    cmpl -208(%rbp), %eax
    setg %al
    movzbl %al, %eax
    movl %eax, -200(%rbp)
    movl -200(%rbp), %eax
    movl %eax, -120(%rbp)
    movl $3, %eax
    movl %eax, -200(%rbp)
    movl $5, %eax
    movl %eax, -208(%rbp)
    movl -200(%rbp), %eax
    cmpl -208(%rbp), %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -200(%rbp)
    movl -200(%rbp), %eax
    movl %eax, -136(%rbp)
    movl $4, %eax
    movl %eax, -200(%rbp)
    movl $4, %eax
    movl %eax, -208(%rbp)
    movl -200(%rbp), %eax
    cmpl -208(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -200(%rbp)
    movl -200(%rbp), %eax
    movl %eax, -152(%rbp)
    movl $5, %eax
    movl %eax, -200(%rbp)
    movl $3, %eax
    movl %eax, -208(%rbp)
    movl -200(%rbp), %eax
    cmpl -208(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -200(%rbp)
    movl -200(%rbp), %eax
    movl %eax, -168(%rbp)
    movl $2, %eax
    movl %eax, -200(%rbp)
    movl $8, %eax
    movl %eax, -208(%rbp)
    movl -200(%rbp), %eax
    cmpl -208(%rbp), %eax
    setg %al
    movzbl %al, %eax
    movl %eax, -200(%rbp)
    movl -200(%rbp), %eax
    movl %eax, -184(%rbp)
    movl -8(%rbp), %eax
    addl -24(%rbp), %eax
    movl %eax, -200(%rbp)
    movl -200(%rbp), %eax
    addl -40(%rbp), %eax
    movl %eax, -200(%rbp)
    movl -200(%rbp), %eax
    addl -56(%rbp), %eax
    movl %eax, -200(%rbp)
    movl -200(%rbp), %eax
    addl -72(%rbp), %eax
    movl %eax, -200(%rbp)
    movl -200(%rbp), %eax
    addl -88(%rbp), %eax
    movl %eax, -200(%rbp)
    movl -200(%rbp), %eax
    addl -104(%rbp), %eax
    movl %eax, -200(%rbp)
    movl -200(%rbp), %eax
    addl -120(%rbp), %eax
    movl %eax, -200(%rbp)
    movl -200(%rbp), %eax
    addl -136(%rbp), %eax
    movl %eax, -200(%rbp)
    movl -200(%rbp), %eax
    addl -152(%rbp), %eax
    movl %eax, -200(%rbp)
    movl -200(%rbp), %eax
    addl -168(%rbp), %eax
    movl %eax, -200(%rbp)
    movl -200(%rbp), %eax
    addl -184(%rbp), %eax
    movl %eax, -200(%rbp)
    movl $4, %eax
    movl %eax, -208(%rbp)
    movl -200(%rbp), %eax
    addl -208(%rbp), %eax
    movl %eax, -200(%rbp)
    movl -200(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
