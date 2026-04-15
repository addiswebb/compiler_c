.section .note.GNU-stack,"",@progbits
.section .rodata
.align 8
.LC0:
    .quad 0x4004000000000000
.align 8
.LC1:
    .quad 0x4012000000000000

.text
.global sum_mix
sum_mix:
    push %rbp
    mov %rsp, %rbp
    subq $272, %rsp
sum_mix_0:
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
    mov $0, %eax
    movl %eax, -240(%rbp)
    movq -232(%rbp), %rax
    movsd -240(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -224(%rbp), %rax
    movq %rax, -232(%rbp)
    movq -232(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
sum_mix_1:
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
    jz sum_mix_11
sum_mix_2:
    leaq -224(%rbp), %rax
    movq %rax, -232(%rbp)
    movq -232(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -232(%rbp)
    movl -232(%rbp), %eax
    cltd
    movl $2, %ecx
    idivl %ecx
    movl %edx, -232(%rbp)
    movl -232(%rbp), %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -232(%rbp)
    movl -232(%rbp), %eax
    testl %eax, %eax
    jz sum_mix_6
sum_mix_3:
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
    jz sum_mix_4
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
    jmp sum_mix_5
sum_mix_4:
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
sum_mix_5:
    movslq -256(%rbp), %rax
    cvtsi2sd %rax, %xmm0
    movsd %xmm0, -240(%rbp)
    movq -232(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -248(%rbp)
    movsd -248(%rbp), %xmm0
    addsd -240(%rbp), %xmm0
    movsd %xmm0, -240(%rbp)
    movq -232(%rbp), %rax
    movsd -240(%rbp), %xmm2
    movsd %xmm2, (%rax)
    jmp sum_mix_9
sum_mix_6:
    leaq -216(%rbp), %rax
    movq %rax, -232(%rbp)
    leaq -208(%rbp), %rax
    movq %rax, -240(%rbp)
    movl -240(%rbp), %eax
    addl $4, %eax
    movl %eax, -240(%rbp)
    movq -240(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -248(%rbp)
    movl -248(%rbp), %eax
    cmpl $176, %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -256(%rbp)
    movl -256(%rbp), %eax
    testl %eax, %eax
    jz sum_mix_7
    movq -240(%rbp), %rax
    addq $12, %rax
    movq %rax, -256(%rbp)
    movq -256(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -256(%rbp)
    movl -256(%rbp), %eax
    addl -248(%rbp), %eax
    movl %eax, -256(%rbp)
    movq -256(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -256(%rbp)
    movl -248(%rbp), %eax
    addl $16, %eax
    movl %eax, -248(%rbp)
    movq -240(%rbp), %rax
    movl -248(%rbp), %ecx
    movl %ecx, (%rax)
    jmp sum_mix_8
sum_mix_7:
    movq -240(%rbp), %rax
    addq $4, %rax
    movq %rax, -240(%rbp)
    movq -240(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -248(%rbp)
    movq -248(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -264(%rbp)
    movq -248(%rbp), %rax
    addq $8, %rax
    movq %rax, -248(%rbp)
    movq -240(%rbp), %rax
    movq -248(%rbp), %rcx
    movq %rcx, (%rax)
    movq -264(%rbp), %rax
    movq %rax, -256(%rbp)
sum_mix_8:
    movq -232(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -240(%rbp)
    movsd -240(%rbp), %xmm0
    addsd -256(%rbp), %xmm0
    movsd %xmm0, -240(%rbp)
    movq -232(%rbp), %rax
    movsd -240(%rbp), %xmm2
    movsd %xmm2, (%rax)
    jmp sum_mix_9
sum_mix_9:
    jmp sum_mix_10
sum_mix_10:
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
    jmp sum_mix_1
sum_mix_11:
    leaq -216(%rbp), %rax
    movq %rax, -232(%rbp)
    movq -232(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -232(%rbp)
    movsd -232(%rbp), %xmm0
    mov %rbp, %rsp
    pop %rbp
    ret
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp
main_0:
    movsd .LC0(%rip), %xmm0
    movsd %xmm0, -8(%rbp)
    movsd .LC1(%rip), %xmm0
    movsd %xmm0, -16(%rbp)
    subq $176, %rsp
    movl $4, %edi
    movl $1, %esi
    movsd -8(%rbp), %xmm0
    movl $3, %edx
    movsd -16(%rbp), %xmm1
    movl $2, %eax
    call sum_mix
    addq $176, %rsp
    movsd %xmm0, -8(%rbp)
    cvttsd2si -8(%rbp), %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
