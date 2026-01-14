.global main
    jmp main
main:
    push %rbp
    mov %rsp, %rbp
    subq $544, %rsp
main_0:
    movl $10, %eax
    movl %eax, -16(%rbp)
    movl $10, %eax
    movl %eax, -24(%rbp)
    movl -16(%rbp), %eax
    cmpl -24(%rbp), %eax
    sete %al
    setne %al
    movzbl %al, %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $10, %eax
    movl %eax, -48(%rbp)
    movl $5, %eax
    movl %eax, -56(%rbp)
    movl -48(%rbp), %eax
    cmpl -56(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -64(%rbp)
    movl -64(%rbp), %eax
    movl %eax, -40(%rbp)
    movl $3, %eax
    movl %eax, -80(%rbp)
    movl $5, %eax
    movl %eax, -88(%rbp)
    movl -80(%rbp), %eax
    cmpl -88(%rbp), %eax
    setl %al
    setle %al
    setg %al
    setge %al
    sete %al
    setne %al
    movzbl %al, %eax
    movl %eax, -96(%rbp)
    movl -96(%rbp), %eax
    movl %eax, -72(%rbp)
    movl $5, %eax
    movl %eax, -112(%rbp)
    movl $5, %eax
    movl %eax, -120(%rbp)
    movl -112(%rbp), %eax
    cmpl -120(%rbp), %eax
    setle %al
    setg %al
    setge %al
    sete %al
    setne %al
    movzbl %al, %eax
    movl %eax, -128(%rbp)
    movl -128(%rbp), %eax
    movl %eax, -104(%rbp)
    movl $7, %eax
    movl %eax, -144(%rbp)
    movl $3, %eax
    movl %eax, -152(%rbp)
    movl -144(%rbp), %eax
    cmpl -152(%rbp), %eax
    setg %al
    setge %al
    sete %al
    setne %al
    movzbl %al, %eax
    movl %eax, -160(%rbp)
    movl -160(%rbp), %eax
    movl %eax, -136(%rbp)
    movl $5, %eax
    movl %eax, -176(%rbp)
    movl $5, %eax
    movl %eax, -184(%rbp)
    movl -176(%rbp), %eax
    cmpl -184(%rbp), %eax
    setge %al
    sete %al
    setne %al
    movzbl %al, %eax
    movl %eax, -192(%rbp)
    movl -192(%rbp), %eax
    movl %eax, -168(%rbp)
    movl $0, %eax
    movl %eax, -208(%rbp)
    movl $2, %eax
    movl %eax, -216(%rbp)
    movl -208(%rbp), %eax
    subl -216(%rbp), %eax
    movl %eax, -224(%rbp)
    movl $0, %eax
    movl %eax, -232(%rbp)
    movl -224(%rbp), %eax
    cmpl -232(%rbp), %eax
    setl %al
    setle %al
    setg %al
    setge %al
    sete %al
    setne %al
    movzbl %al, %eax
    movl %eax, -240(%rbp)
    movl -240(%rbp), %eax
    movl %eax, -200(%rbp)
    movl $0, %eax
    movl %eax, -256(%rbp)
    movl $1, %eax
    movl %eax, -264(%rbp)
    movl -256(%rbp), %eax
    subl -264(%rbp), %eax
    movl %eax, -272(%rbp)
    movl $0, %eax
    movl %eax, -280(%rbp)
    movl $5, %eax
    movl %eax, -288(%rbp)
    movl -280(%rbp), %eax
    subl -288(%rbp), %eax
    movl %eax, -296(%rbp)
    movl -272(%rbp), %eax
    cmpl -296(%rbp), %eax
    setg %al
    setge %al
    sete %al
    setne %al
    movzbl %al, %eax
    movl %eax, -304(%rbp)
    movl -304(%rbp), %eax
    movl %eax, -248(%rbp)
    movl $3, %eax
    movl %eax, -320(%rbp)
    movl $5, %eax
    movl %eax, -328(%rbp)
    movl -320(%rbp), %eax
    cmpl -328(%rbp), %eax
    sete %al
    setne %al
    movzbl %al, %eax
    movl %eax, -336(%rbp)
    movl -336(%rbp), %eax
    movl %eax, -312(%rbp)
    movl $4, %eax
    movl %eax, -352(%rbp)
    movl $4, %eax
    movl %eax, -360(%rbp)
    movl -352(%rbp), %eax
    cmpl -360(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -368(%rbp)
    movl -368(%rbp), %eax
    movl %eax, -344(%rbp)
    movl $5, %eax
    movl %eax, -384(%rbp)
    movl $3, %eax
    movl %eax, -392(%rbp)
    movl -384(%rbp), %eax
    cmpl -392(%rbp), %eax
    setl %al
    setle %al
    setg %al
    setge %al
    sete %al
    setne %al
    movzbl %al, %eax
    movl %eax, -400(%rbp)
    movl -400(%rbp), %eax
    movl %eax, -376(%rbp)
    movl $2, %eax
    movl %eax, -416(%rbp)
    movl $8, %eax
    movl %eax, -424(%rbp)
    movl -416(%rbp), %eax
    cmpl -424(%rbp), %eax
    setg %al
    setge %al
    sete %al
    setne %al
    movzbl %al, %eax
    movl %eax, -432(%rbp)
    movl -432(%rbp), %eax
    movl %eax, -408(%rbp)
    movl -8(%rbp), %eax
    addl -40(%rbp), %eax
    movl %eax, -440(%rbp)
    movl -440(%rbp), %eax
    addl -72(%rbp), %eax
    movl %eax, -448(%rbp)
    movl -448(%rbp), %eax
    addl -104(%rbp), %eax
    movl %eax, -456(%rbp)
    movl -456(%rbp), %eax
    addl -136(%rbp), %eax
    movl %eax, -464(%rbp)
    movl -464(%rbp), %eax
    addl -168(%rbp), %eax
    movl %eax, -472(%rbp)
    movl -472(%rbp), %eax
    addl -200(%rbp), %eax
    movl %eax, -480(%rbp)
    movl -480(%rbp), %eax
    addl -248(%rbp), %eax
    movl %eax, -488(%rbp)
    movl -488(%rbp), %eax
    addl -312(%rbp), %eax
    movl %eax, -496(%rbp)
    movl -496(%rbp), %eax
    addl -344(%rbp), %eax
    movl %eax, -504(%rbp)
    movl -504(%rbp), %eax
    addl -376(%rbp), %eax
    movl %eax, -512(%rbp)
    movl -512(%rbp), %eax
    addl -408(%rbp), %eax
    movl %eax, -520(%rbp)
    movl $4, %eax
    movl %eax, -528(%rbp)
    movl -520(%rbp), %eax
    addl -528(%rbp), %eax
    movl %eax, -536(%rbp)
    movl -536(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
