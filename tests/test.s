.section .note.GNU-stack,"",@progbits

.text
.global sum
sum:
    push %rbp
    mov %rsp, %rbp
    subq $248, %rsp
sum_0:
    movl %edi, -184(%rbp)
    movq %rsi, -168(%rbp)
    movq %rdx, -160(%rbp)
    movq %rcx, -152(%rbp)
    movq %r8, -144(%rbp)
    movq %r9, -136(%rbp)
    movb %al, %al
    cmpb $0, %al
    sete %al
    movzbl %al, %eax
    movl %eax, -224(%rbp)
    movl -224(%rbp), %eax
    testl %eax, %eax
    jnz sum_1
    movsd %xmm0, -128(%rbp)
    movsd %xmm1, -112(%rbp)
    movsd %xmm2, -96(%rbp)
    movsd %xmm3, -80(%rbp)
    movsd %xmm4, -64(%rbp)
    movsd %xmm5, -48(%rbp)
    movsd %xmm6, %xmm0
    movsd %xmm0, -32(%rbp)
    movsd %xmm7, %xmm0
    movsd %xmm0, -16(%rbp)
sum_1:
    leaq -192(%rbp), %rax
    movq %rax, -224(%rbp)
    movq -224(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    leaq -216(%rbp), %rax
    movq %rax, -224(%rbp)
    movq -224(%rbp), %rax
    movl $8, %ecx
    movl %ecx, (%rax)
    movq -224(%rbp), %rax
    addq $4, %rax
    movq %rax, -224(%rbp)
    movq -224(%rbp), %rax
    movl $48, %ecx
    movl %ecx, (%rax)
    movq -224(%rbp), %rax
    addq $4, %rax
    movq %rax, -224(%rbp)
    leaq 16(%rbp), %rax
    movq %rax, -232(%rbp)
    movq -224(%rbp), %rax
    movq -232(%rbp), %rcx
    movq %rcx, (%rax)
    movq -224(%rbp), %rax
    addq $8, %rax
    movq %rax, -224(%rbp)
    leaq -176(%rbp), %rax
    movq %rax, -232(%rbp)
    movq -224(%rbp), %rax
    movq -232(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -192(%rbp), %rax
    movq %rax, -224(%rbp)
    leaq -184(%rbp), %rax
    movq %rax, -232(%rbp)
    movq -232(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -232(%rbp)
    leaq -216(%rbp), %rax
    movq %rax, -240(%rbp)
    subq $8, %rsp
    movl -232(%rbp), %edi
    movq -240(%rbp), %rsi
    call vsum
    addq $8, %rsp
    movl %eax, -232(%rbp)
    movq -224(%rbp), %rax
    movl -232(%rbp), %ecx
    movl %ecx, (%rax)
    leaq -192(%rbp), %rax
    movq %rax, -224(%rbp)
    movq -224(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -224(%rbp)
    movl -224(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
.global vsum
vsum:
    push %rbp
    mov %rsp, %rbp
    subq $88, %rsp
vsum_0:
    movl %edi, -8(%rbp)
    movq %rsi, -16(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    leaq -32(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
vsum_1:
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
    jz vsum_6
vsum_2:
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    cmpl $48, %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -64(%rbp)
    movl -64(%rbp), %eax
    testl %eax, %eax
    jz vsum_3
    movq -48(%rbp), %rax
    addq $16, %rax
    movq %rax, -64(%rbp)
    movq -64(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -64(%rbp)
    movl -64(%rbp), %eax
    addl -56(%rbp), %eax
    movl %eax, -64(%rbp)
    movq -64(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -64(%rbp)
    movl -56(%rbp), %eax
    addl $8, %eax
    movl %eax, -56(%rbp)
    movq -48(%rbp), %rax
    movl -56(%rbp), %ecx
    movl %ecx, (%rax)
    jmp vsum_4
vsum_3:
    movq -48(%rbp), %rax
    addq $8, %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -72(%rbp)
    movq -56(%rbp), %rax
    addq $8, %rax
    movq %rax, -56(%rbp)
    movq -48(%rbp), %rax
    movq -56(%rbp), %rcx
    movq %rcx, (%rax)
    movq -72(%rbp), %rax
    movq %rax, -64(%rbp)
vsum_4:
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -48(%rbp)
    movl -48(%rbp), %eax
    addl -64(%rbp), %eax
    movl %eax, -48(%rbp)
    movq -40(%rbp), %rax
    movl -48(%rbp), %ecx
    movl %ecx, (%rax)
    jmp vsum_5
vsum_5:
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
    jmp vsum_1
vsum_6:
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $24, %rsp
main_0:
    subq $184, %rsp
    movl $5, %edi
    movl $1, %esi
    movl $2, %edx
    movl $3, %ecx
    movl $4, %r8d
    movl $5, %r9d
    xor %eax, %eax
    call sum
    addq $184, %rsp
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
