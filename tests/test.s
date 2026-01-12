.global main
    jmp main
add:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
add_0:
    movl 24(%rbp), %eax
    movl %eax, -8(%rbp)
    movl 16(%rbp), %eax
    movl %eax, -16(%rbp)
    movl -8(%rbp), %eax
    addl -16(%rbp), %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
mul:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
mul_0:
    movl 24(%rbp), %eax
    movl %eax, -8(%rbp)
    movl 16(%rbp), %eax
    movl %eax, -16(%rbp)
    movl -8(%rbp), %eax
    imull -16(%rbp)
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
sumToN:
    push %rbp
    mov %rsp, %rbp
    subq $96, %rsp
sumToN_0:
    movl 16(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $0, -24(%rbp)
    movl -24(%rbp), %eax
    movl %eax, -16(%rbp)
    movl $0, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -32(%rbp)
    movl $1, -48(%rbp)
    movl -48(%rbp), %eax
    movl %eax, -32(%rbp)
sumToN_1:
    movl -8(%rbp), %eax
    subl -32(%rbp), %eax
    movl %eax, -56(%rbp)
    movl $1, -64(%rbp)
    movl -56(%rbp), %eax
    addl -64(%rbp), %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    testl %eax, %eax
    jz sumToN_3
sumToN_2:
    movl -16(%rbp), %eax
    addl -32(%rbp), %eax
    movl %eax, -80(%rbp)
    movl -80(%rbp), %eax
    movl %eax, -16(%rbp)
    movl $1, -80(%rbp)
    movl -32(%rbp), %eax
    addl -80(%rbp), %eax
    movl %eax, -88(%rbp)
    movl -88(%rbp), %eax
    movl %eax, -32(%rbp)
    jmp sumToN_1
sumToN_3:
    movl -16(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
mixed:
    push %rbp
    mov %rsp, %rbp
    subq $96, %rsp
mixed_0:
    movl 32(%rbp), %eax
    movl %eax, -8(%rbp)
    movl 24(%rbp), %eax
    movl %eax, -16(%rbp)
    movl 16(%rbp), %eax
    movl %eax, -24(%rbp)
    movl -8(%rbp), %eax
    push %rax
    movl -16(%rbp), %eax
    push %rax
    call add
    add $16, %rsp
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    push %rax
    movl -24(%rbp), %eax
    push %rax
    call mul
    add $16, %rsp
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -48(%rbp)
    movl $1, -72(%rbp)
    movl -48(%rbp), %eax
    addl -72(%rbp), %eax
    movl %eax, -80(%rbp)
    movl -80(%rbp), %eax
    movl %eax, -64(%rbp)
    movl -64(%rbp), %eax
    addl -48(%rbp), %eax
    movl %eax, -88(%rbp)
    movl -88(%rbp), %eax
    movl %eax, -48(%rbp)
    movl -48(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
main:
    push %rbp
    mov %rsp, %rbp
    subq $144, %rsp
main_0:
    movl $3, -16(%rbp)
    movl -16(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $4, -32(%rbp)
    movl -32(%rbp), %eax
    movl %eax, -24(%rbp)
    movl $5, -48(%rbp)
    movl -48(%rbp), %eax
    movl %eax, -40(%rbp)
    movl -8(%rbp), %eax
    push %rax
    movl -24(%rbp), %eax
    push %rax
    call add
    add $16, %rsp
    movl %eax, -64(%rbp)
    movl -64(%rbp), %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    push %rax
    movl -40(%rbp), %eax
    push %rax
    call mul
    add $16, %rsp
    movl %eax, -80(%rbp)
    movl -80(%rbp), %eax
    movl %eax, -72(%rbp)
    movl $10, -96(%rbp)
    movl -96(%rbp), %eax
    push %rax
    call sumToN
    add $8, %rsp
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    movl %eax, -88(%rbp)
    movl -8(%rbp), %eax
    push %rax
    movl -24(%rbp), %eax
    push %rax
    movl -40(%rbp), %eax
    push %rax
    call mixed
    add $24, %rsp
    movl %eax, -120(%rbp)
    movl -120(%rbp), %eax
    movl %eax, -112(%rbp)
    movl -72(%rbp), %eax
    addl -88(%rbp), %eax
    movl %eax, -136(%rbp)
    movl -136(%rbp), %eax
    addl -112(%rbp), %eax
    movl %eax, -144(%rbp)
    movl -144(%rbp), %eax
    movl %eax, -128(%rbp)
    movl -128(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
