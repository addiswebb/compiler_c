
.text
.global sum64
sum64:
    push %rbp
    mov %rsp, %rbp
    subq $64, %rsp
sum64_0:
    movl %ecx, -8(%rbp)
    movq %rcx, 16(%rbp)
    movq %rdx, 24(%rbp)
    movq %r8, 32(%rbp)
    movq %r9, 40(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -40(%rbp)
    leaq 16(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    movq -48(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq $0, %rcx
    movq %rcx, (%rax)
    leaq -32(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
sum64_1:
    leaq -32(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -48(%rbp)
    movl -40(%rbp), %eax
    cmpl -48(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz sum64_4
sum64_2:
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    addq $8, %rax
    movq %rax, -56(%rbp)
    movq -48(%rbp), %rax
    movq -56(%rbp), %rcx
    movq %rcx, (%rax)
    movq -56(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    addq -48(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    movq -48(%rbp), %rcx
    movq %rcx, (%rax)
    jmp sum64_3
sum64_3:
    leaq -32(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -48(%rbp)
    movl -48(%rbp), %eax
    addl $1, %eax
    movl %eax, -48(%rbp)
    movq -40(%rbp), %rax
    movl -48(%rbp), %ecx
    movl %ecx, (%rax)
    jmp sum64_1
sum64_4:
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    mov %rbp, %rsp
    pop %rbp
    ret
