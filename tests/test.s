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
    subq $240, %rsp
sum_mix_0:
    movl %ecx, -184(%rbp)
    movq %rdx, 24(%rbp)
    movq %r8, 32(%rbp)
    movq %r9, 40(%rbp)
    leaq -192(%rbp), %rax
    movq %rax, -216(%rbp)
    leaq 16(%rbp), %rax
    movq %rax, -224(%rbp)
    movq -216(%rbp), %rax
    movq -224(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -200(%rbp), %rax
    movq %rax, -216(%rbp)
    mov $0, %eax
    movl %eax, -224(%rbp)
    movq -216(%rbp), %rax
    movsd -224(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -208(%rbp), %rax
    movq %rax, -216(%rbp)
    movq -216(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
sum_mix_1:
    leaq -208(%rbp), %rax
    movq %rax, -216(%rbp)
    movq -216(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -216(%rbp)
    leaq -184(%rbp), %rax
    movq %rax, -224(%rbp)
    movq -224(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -224(%rbp)
    movl -216(%rbp), %eax
    cmpl -224(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -216(%rbp)
    movl -216(%rbp), %eax
    testl %eax, %eax
    jz sum_mix_7
sum_mix_2:
    leaq -208(%rbp), %rax
    movq %rax, -216(%rbp)
    movq -216(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -216(%rbp)
    movl -216(%rbp), %eax
    cltd
    movl $2, %ecx
    idivl %ecx
    movl %edx, -216(%rbp)
    movl -216(%rbp), %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -216(%rbp)
    movl -216(%rbp), %eax
    testl %eax, %eax
    jz sum_mix_4
sum_mix_3:
    leaq -200(%rbp), %rax
    movq %rax, -216(%rbp)
    leaq -192(%rbp), %rax
    movq %rax, -224(%rbp)
    movq -224(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -232(%rbp)
    movq -232(%rbp), %rax
    addq $8, %rax
    movq %rax, -232(%rbp)
    movq -224(%rbp), %rax
    movq -232(%rbp), %rcx
    movq %rcx, (%rax)
    movq -232(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -224(%rbp)
    movslq -224(%rbp), %rax
    cvtsi2sd %rax, %xmm0
    movsd %xmm0, -224(%rbp)
    movq -216(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -232(%rbp)
    movsd -232(%rbp), %xmm0
    addsd -224(%rbp), %xmm0
    movsd %xmm0, -224(%rbp)
    movq -216(%rbp), %rax
    movsd -224(%rbp), %xmm2
    movsd %xmm2, (%rax)
    jmp sum_mix_5
sum_mix_4:
    leaq -200(%rbp), %rax
    movq %rax, -216(%rbp)
    leaq -192(%rbp), %rax
    movq %rax, -224(%rbp)
    movq -224(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -232(%rbp)
    movq -232(%rbp), %rax
    addq $8, %rax
    movq %rax, -232(%rbp)
    movq -224(%rbp), %rax
    movq -232(%rbp), %rcx
    movq %rcx, (%rax)
    movq -232(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -224(%rbp)
    movq -216(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -232(%rbp)
    movsd -232(%rbp), %xmm0
    addsd -224(%rbp), %xmm0
    movsd %xmm0, -224(%rbp)
    movq -216(%rbp), %rax
    movsd -224(%rbp), %xmm2
    movsd %xmm2, (%rax)
    jmp sum_mix_5
sum_mix_5:
    jmp sum_mix_6
sum_mix_6:
    leaq -208(%rbp), %rax
    movq %rax, -216(%rbp)
    movq -216(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -224(%rbp)
    movl -224(%rbp), %eax
    addl $1, %eax
    movl %eax, -224(%rbp)
    movq -216(%rbp), %rax
    movl -224(%rbp), %ecx
    movl %ecx, (%rax)
    jmp sum_mix_1
sum_mix_7:
    leaq -200(%rbp), %rax
    movq %rax, -216(%rbp)
    movq -216(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -216(%rbp)
    movsd -216(%rbp), %xmm0
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
