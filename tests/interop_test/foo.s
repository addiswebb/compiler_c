.section .rodata

.text

.global test
test:
    push %rbp
    mov %rsp, %rbp
    subq $128, %rsp
test_0:
    movl %ecx, %eax
    movl %eax, -8(%rbp)
    movl %edx, %eax
    movl %eax, -24(%rbp)
    movl %r8d, %eax
    movl %eax, -40(%rbp)
    movl %r9d, %eax
    movl %eax, -56(%rbp)
    movl 48(%rbp), %eax
    movl %eax, -72(%rbp)
    movl 56(%rbp), %eax
    movl %eax, -88(%rbp)
    movl 64(%rbp), %eax
    movl %eax, -104(%rbp)
    movl $97, %eax
    movb %al, -120(%rbp)
    movsbl -120(%rbp), %eax
    movl %eax, -120(%rbp)
    movl -120(%rbp), %eax
    addl -8(%rbp), %eax
    movl %eax, -120(%rbp)
    movl $1, %eax
    movl %eax, -128(%rbp)
    movl -120(%rbp), %eax
    subl -128(%rbp), %eax
    movl %eax, -120(%rbp)
    subq $48, %rsp
    movl -120(%rbp), %ecx
    call putchar
    addq $48, %rsp
    movl %eax, -120(%rbp)
    movl $97, %eax
    movb %al, -120(%rbp)
    movsbl -120(%rbp), %eax
    movl %eax, -120(%rbp)
    movl -120(%rbp), %eax
    addl -24(%rbp), %eax
    movl %eax, -120(%rbp)
    movl $1, %eax
    movl %eax, -128(%rbp)
    movl -120(%rbp), %eax
    subl -128(%rbp), %eax
    movl %eax, -120(%rbp)
    subq $48, %rsp
    movl -120(%rbp), %ecx
    call putchar
    addq $48, %rsp
    movl %eax, -120(%rbp)
    movl $97, %eax
    movb %al, -120(%rbp)
    movsbl -120(%rbp), %eax
    movl %eax, -120(%rbp)
    movl -120(%rbp), %eax
    addl -40(%rbp), %eax
    movl %eax, -120(%rbp)
    movl $1, %eax
    movl %eax, -128(%rbp)
    movl -120(%rbp), %eax
    subl -128(%rbp), %eax
    movl %eax, -120(%rbp)
    subq $48, %rsp
    movl -120(%rbp), %ecx
    call putchar
    addq $48, %rsp
    movl %eax, -120(%rbp)
    movl $97, %eax
    movb %al, -120(%rbp)
    movsbl -120(%rbp), %eax
    movl %eax, -120(%rbp)
    movl -120(%rbp), %eax
    addl -56(%rbp), %eax
    movl %eax, -120(%rbp)
    movl $1, %eax
    movl %eax, -128(%rbp)
    movl -120(%rbp), %eax
    subl -128(%rbp), %eax
    movl %eax, -120(%rbp)
    subq $48, %rsp
    movl -120(%rbp), %ecx
    call putchar
    addq $48, %rsp
    movl %eax, -120(%rbp)
    movl $97, %eax
    movb %al, -120(%rbp)
    movsbl -120(%rbp), %eax
    movl %eax, -120(%rbp)
    movl -120(%rbp), %eax
    addl -72(%rbp), %eax
    movl %eax, -120(%rbp)
    movl $1, %eax
    movl %eax, -128(%rbp)
    movl -120(%rbp), %eax
    subl -128(%rbp), %eax
    movl %eax, -120(%rbp)
    subq $48, %rsp
    movl -120(%rbp), %ecx
    call putchar
    addq $48, %rsp
    movl %eax, -120(%rbp)
    movl $97, %eax
    movb %al, -120(%rbp)
    movsbl -120(%rbp), %eax
    movl %eax, -120(%rbp)
    movl -120(%rbp), %eax
    addl -88(%rbp), %eax
    movl %eax, -120(%rbp)
    movl $1, %eax
    movl %eax, -128(%rbp)
    movl -120(%rbp), %eax
    subl -128(%rbp), %eax
    movl %eax, -120(%rbp)
    subq $48, %rsp
    movl -120(%rbp), %ecx
    call putchar
    addq $48, %rsp
    movl %eax, -120(%rbp)
    movl $97, %eax
    movb %al, -120(%rbp)
    movsbl -120(%rbp), %eax
    movl %eax, -120(%rbp)
    movl -120(%rbp), %eax
    addl -104(%rbp), %eax
    movl %eax, -120(%rbp)
    movl $1, %eax
    movl %eax, -128(%rbp)
    movl -120(%rbp), %eax
    subl -128(%rbp), %eax
    movl %eax, -120(%rbp)
    subq $48, %rsp
    movl -120(%rbp), %ecx
    call putchar
    addq $48, %rsp
    movl %eax, -120(%rbp)
    movl -8(%rbp), %eax
    addl -24(%rbp), %eax
    movl %eax, -120(%rbp)
    movl -120(%rbp), %eax
    addl -40(%rbp), %eax
    movl %eax, -120(%rbp)
    movl -120(%rbp), %eax
    addl -56(%rbp), %eax
    movl %eax, -120(%rbp)
    movl -120(%rbp), %eax
    addl -72(%rbp), %eax
    movl %eax, -120(%rbp)
    movl -120(%rbp), %eax
    addl -88(%rbp), %eax
    movl %eax, -120(%rbp)
    movl -120(%rbp), %eax
    addl -104(%rbp), %eax
    movl %eax, -120(%rbp)
    movl -120(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
