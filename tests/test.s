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
    subq $64, %rsp
sum_mix_0:
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
    mov $0, %eax
    movl %eax, -48(%rbp)
    movq -40(%rbp), %rax
    movsd -48(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -32(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
sum_mix_1:
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
    jz sum_mix_7
sum_mix_2:
    leaq -32(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    cltd
    movl $2, %ecx
    idivl %ecx
    movl %edx, -40(%rbp)
    movl -40(%rbp), %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz sum_mix_4
sum_mix_3:
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
    movl (%rax), %eax
    movl %eax, -48(%rbp)
    movslq -48(%rbp), %rax
    cvtsi2sd %rax, %xmm0
    movsd %xmm0, -48(%rbp)
    movq -40(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -56(%rbp)
    movsd -56(%rbp), %xmm0
    addsd -48(%rbp), %xmm0
    movsd %xmm0, -48(%rbp)
    movq -40(%rbp), %rax
    movsd -48(%rbp), %xmm2
    movsd %xmm2, (%rax)
    jmp sum_mix_5
sum_mix_4:
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
    movsd (%rax), %xmm0
    movsd %xmm0, -48(%rbp)
    movq -40(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -56(%rbp)
    movsd -56(%rbp), %xmm0
    addsd -48(%rbp), %xmm0
    movsd %xmm0, -48(%rbp)
    movq -40(%rbp), %rax
    movsd -48(%rbp), %xmm2
    movsd %xmm2, (%rax)
    jmp sum_mix_5
sum_mix_5:
    jmp sum_mix_6
sum_mix_6:
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
    jmp sum_mix_1
sum_mix_7:
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -40(%rbp)
    movsd -40(%rbp), %xmm0
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
    subq $48, %rsp
    movl $4, %ecx
    movl $1, %edx
    movq -8(%rbp), %r8
    movsd -8(%rbp), %xmm2
    movl $3, %r9d
    movsd -16(%rbp), %xmm0
    movsd %xmm0, 32(%rsp)
    call sum_mix
    addq $48, %rsp
    movsd %xmm0, -8(%rbp)
    cvttsd2si -8(%rbp), %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
