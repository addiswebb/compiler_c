.section .note.GNU-stack,"",@progbits

.text
.global sum
sum:
    push %rbp
    mov %rsp, %rbp
    subq $272, %rsp
sum_0:
    movl %edi, -184(%rbp)
    movq %rsi, -168(%rbp)
    movq %rdx, -160(%rbp)
    movq %rcx, -152(%rbp)
    movq %r8, -144(%rbp)
    movq %r9, -136(%rbp)
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
    leaq -208(%rbp), %rax
    movq %rax, -232(%rbp)
    movq -232(%rbp), %rax
    movl $8, %ecx
    movl %ecx, (%rax)
    movq -232(%rbp), %rax
    addq $4, %rax
    movq %rax, -232(%rbp)
    movq -232(%rbp), %rax
    movl $48, %ecx
    movl %ecx, (%rax)
    movq -232(%rbp), %rax
    addq $4, %rax
    movq %rax, -232(%rbp)
    leaq 16(%rbp), %rax
    movq %rax, -240(%rbp)
    movq -232(%rbp), %rax
    movq -240(%rbp), %rcx
    movq %rcx, (%rax)
    movq -232(%rbp), %rax
    addq $8, %rax
    movq %rax, -232(%rbp)
    leaq -176(%rbp), %rax
    movq %rax, -240(%rbp)
    movq -232(%rbp), %rax
    movq -240(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -216(%rbp), %rax
    movq %rax, -232(%rbp)
    movq -232(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    leaq -224(%rbp), %rax
    movq %rax, -232(%rbp)
    movq -232(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
sum_1:
    leaq -224(%rbp), %rax
    movq %rax, -232(%rbp)
    movq -232(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -232(%rbp)
    leaq -184(%rbp), %rax
    movq %rax, -240(%rbp)
    movq -240(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -240(%rbp)
    movl -232(%rbp), %eax
    cmpl -240(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -232(%rbp)
    movl -232(%rbp), %eax
    testl %eax, %eax
    jz sum_6
sum_2:
    leaq -216(%rbp), %rax
    movq %rax, -232(%rbp)
    leaq -208(%rbp), %rax
    movq %rax, -240(%rbp)
    movq -240(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -248(%rbp)
    movl -248(%rbp), %eax
    cmpl $48, %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -256(%rbp)
    movl -256(%rbp), %eax
    testl %eax, %eax
    jz sum_3
    movq -240(%rbp), %rax
    addq $16, %rax
    movq %rax, -256(%rbp)
    movq -256(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -256(%rbp)
    movl -256(%rbp), %eax
    addl -248(%rbp), %eax
    movl %eax, -256(%rbp)
    movq -256(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -256(%rbp)
    movl -248(%rbp), %eax
    addl $8, %eax
    movl %eax, -248(%rbp)
    movq -240(%rbp), %rax
    movl -248(%rbp), %ecx
    movl %ecx, (%rax)
    jmp sum_4
sum_3:
    movq -240(%rbp), %rax
    addq $8, %rax
    movq %rax, -240(%rbp)
    movq -240(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -248(%rbp)
    movq -248(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -264(%rbp)
    movq -248(%rbp), %rax
    addq $8, %rax
    movq %rax, -248(%rbp)
    movq -240(%rbp), %rax
    movq -248(%rbp), %rcx
    movq %rcx, (%rax)
    movq -264(%rbp), %rax
    movq %rax, -256(%rbp)
sum_4:
    movq -232(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -240(%rbp)
    movl -240(%rbp), %eax
    addl -256(%rbp), %eax
    movl %eax, -240(%rbp)
    movq -232(%rbp), %rax
    movl -240(%rbp), %ecx
    movl %ecx, (%rax)
    jmp sum_5
sum_5:
    leaq -224(%rbp), %rax
    movq %rax, -232(%rbp)
    movq -232(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -240(%rbp)
    movl -240(%rbp), %eax
    addl $1, %eax
    movl %eax, -240(%rbp)
    movq -232(%rbp), %rax
    movl -240(%rbp), %ecx
    movl %ecx, (%rax)
    jmp sum_1
sum_6:
    leaq -216(%rbp), %rax
    movq %rax, -232(%rbp)
    movq -232(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -232(%rbp)
    movl -232(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp
main_0:
    subq $208, %rsp
    movl $9, %edi
    movl $1, %esi
    movl $2, %edx
    movl $3, %ecx
    movl $4, %r8d
    movl $5, %r9d
    movl $6, %eax
    movl %eax, 0(%rsp)
    movl $7, %eax
    movl %eax, 8(%rsp)
    movl $8, %eax
    movl %eax, 16(%rsp)
    movl $9, %eax
    movl %eax, 24(%rsp)
    xor %eax, %eax
    call sum
    addq $208, %rsp
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
