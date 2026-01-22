.section .rodata
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $128, %rsp
main_0:
    movl $10, %eax
    movl %eax, -104(%rbp)
    movl $10, %eax
    movl %eax, -112(%rbp)
    movl -104(%rbp), %eax
    cmpl -112(%rbp), %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $10, %eax
    movl %eax, -104(%rbp)
    movl $5, %eax
    movl %eax, -112(%rbp)
    movl -104(%rbp), %eax
    cmpl -112(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    movl %eax, -16(%rbp)
    movl $3, %eax
    movl %eax, -104(%rbp)
    movl $5, %eax
    movl %eax, -112(%rbp)
    movl -104(%rbp), %eax
    cmpl -112(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    movl %eax, -24(%rbp)
    movl $5, %eax
    movl %eax, -104(%rbp)
    movl $5, %eax
    movl %eax, -112(%rbp)
    movl -104(%rbp), %eax
    cmpl -112(%rbp), %eax
    setle %al
    movzbl %al, %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    movl %eax, -32(%rbp)
    movl $7, %eax
    movl %eax, -104(%rbp)
    movl $3, %eax
    movl %eax, -112(%rbp)
    movl -104(%rbp), %eax
    cmpl -112(%rbp), %eax
    setg %al
    movzbl %al, %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    movl %eax, -40(%rbp)
    movl $5, %eax
    movl %eax, -104(%rbp)
    movl $5, %eax
    movl %eax, -112(%rbp)
    movl -104(%rbp), %eax
    cmpl -112(%rbp), %eax
    setge %al
    movzbl %al, %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    movl %eax, -48(%rbp)
    movl $0, %eax
    movl %eax, -104(%rbp)
    movl $2, %eax
    movl %eax, -112(%rbp)
    movl -104(%rbp), %eax
    subl -112(%rbp), %eax
    movl %eax, -104(%rbp)
    movl $0, %eax
    movl %eax, -112(%rbp)
    movl -104(%rbp), %eax
    cmpl -112(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    movl %eax, -56(%rbp)
    movl $0, %eax
    movl %eax, -104(%rbp)
    movl $1, %eax
    movl %eax, -112(%rbp)
    movl -104(%rbp), %eax
    subl -112(%rbp), %eax
    movl %eax, -104(%rbp)
    movl $0, %eax
    movl %eax, -112(%rbp)
    movl $5, %eax
    movl %eax, -120(%rbp)
    movl -112(%rbp), %eax
    subl -120(%rbp), %eax
    movl %eax, -112(%rbp)
    movl -104(%rbp), %eax
    cmpl -112(%rbp), %eax
    setg %al
    movzbl %al, %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    movl %eax, -64(%rbp)
    movl $3, %eax
    movl %eax, -104(%rbp)
    movl $5, %eax
    movl %eax, -112(%rbp)
    movl -104(%rbp), %eax
    cmpl -112(%rbp), %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    movl %eax, -72(%rbp)
    movl $4, %eax
    movl %eax, -104(%rbp)
    movl $4, %eax
    movl %eax, -112(%rbp)
    movl -104(%rbp), %eax
    cmpl -112(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    movl %eax, -80(%rbp)
    movl $5, %eax
    movl %eax, -104(%rbp)
    movl $3, %eax
    movl %eax, -112(%rbp)
    movl -104(%rbp), %eax
    cmpl -112(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    movl %eax, -88(%rbp)
    movl $2, %eax
    movl %eax, -104(%rbp)
    movl $8, %eax
    movl %eax, -112(%rbp)
    movl -104(%rbp), %eax
    cmpl -112(%rbp), %eax
    setg %al
    movzbl %al, %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    movl %eax, -96(%rbp)
    movl -8(%rbp), %eax
    addl -16(%rbp), %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    addl -24(%rbp), %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    addl -32(%rbp), %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    addl -40(%rbp), %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    addl -48(%rbp), %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    addl -56(%rbp), %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    addl -64(%rbp), %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    addl -72(%rbp), %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    addl -80(%rbp), %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    addl -88(%rbp), %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    addl -96(%rbp), %eax
    movl %eax, -104(%rbp)
    movl $4, %eax
    movl %eax, -112(%rbp)
    movl -104(%rbp), %eax
    addl -112(%rbp), %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
