.global main
    jmp main
main:
    push %rbp
    mov %rsp, %rbp
    subq $128, %rsp
main_0:
    movl $10, %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $0, %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    movl %eax, -24(%rbp)
    movl $0, %eax
    movl %eax, -48(%rbp)
    movl -48(%rbp), %eax
    movl %eax, -40(%rbp)
main_1:
    movl -40(%rbp), %eax
    cmpl -8(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    testl %eax, %eax
    jz main_3
main_2:
    movl $1, %eax
    movl %eax, -64(%rbp)
    movl -24(%rbp), %eax
    movl %eax, -72(%rbp)
    movl -24(%rbp), %eax
    addl -64(%rbp), %eax
    movl %eax, -24(%rbp)
    movl $1, %eax
    movl %eax, -64(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -72(%rbp)
    movl -40(%rbp), %eax
    addl -64(%rbp), %eax
    movl %eax, -40(%rbp)
    jmp main_1
main_3:
main_4:
    movl $2, %eax
    movl %eax, -80(%rbp)
    movl -8(%rbp), %eax
    imull -80(%rbp)
    movl %eax, -88(%rbp)
    movl -24(%rbp), %eax
    cmpl -88(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -96(%rbp)
    movl -96(%rbp), %eax
    testl %eax, %eax
    jz main_6
main_5:
    movl $1, %eax
    movl %eax, -104(%rbp)
    movl -24(%rbp), %eax
    movl %eax, -112(%rbp)
    movl -24(%rbp), %eax
    addl -104(%rbp), %eax
    movl %eax, -24(%rbp)
    jmp main_4
main_6:
    movl $20, %eax
    movl %eax, -104(%rbp)
    movl -24(%rbp), %eax
    cmpl -104(%rbp), %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -112(%rbp)
    movl -112(%rbp), %eax
    testl %eax, %eax
    jz main_8
main_7:
    movl $2000, %eax
    movl %eax, -120(%rbp)
    movl -120(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp main_9
main_8:
    movl $2000, %eax
    movl %eax, -120(%rbp)
    movl -120(%rbp), %eax
    negl %eax
    movl %eax, -128(%rbp)
    movl -128(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp main_9
main_9:
    movl -24(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
