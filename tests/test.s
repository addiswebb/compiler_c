.section .rodata

.text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $80, %rsp
main_0:
    movl $4, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -16(%rbp)
    movl $2, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -12(%rbp)
    movl $1, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -8(%rbp)
    lea -16(%rbp), %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    movq %rax, -40(%rbp)
    movl $1, %eax
    movl %eax, -56(%rbp)
    movslq -56(%rbp), %rax
    movq %rax, -56(%rbp)
    movl $4, %eax
    movq %rax, -64(%rbp)
    movq -56(%rbp), %rax
    imulq -64(%rbp)
    movq %rax, -56(%rbp)
    movq -40(%rbp), %rax
    addq -56(%rbp), %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -56(%rbp)
    movslq -56(%rbp), %rax
    cvtsi2ss %rax, %xmm0
    movss %xmm0, -56(%rbp)
    lea -16(%rbp), %rax
    movq %rax, -64(%rbp)
    movl $2, %eax
    movl %eax, -72(%rbp)
    movslq -72(%rbp), %rax
    movq %rax, -72(%rbp)
    movl $4, %eax
    movq %rax, -80(%rbp)
    movq -72(%rbp), %rax
    imulq -80(%rbp)
    movq %rax, -72(%rbp)
    movq -64(%rbp), %rax
    addq -72(%rbp), %rax
    movq %rax, -64(%rbp)
    movq -64(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -64(%rbp)
    movslq -64(%rbp), %rax
    cvtsi2ss %rax, %xmm0
    movss %xmm0, -64(%rbp)
    movss -56(%rbp), %xmm0
    divss -64(%rbp), %xmm0
    movss %xmm0, -56(%rbp)
    cvttss2si -56(%rbp), %eax
    movl %eax, -56(%rbp)
    movl $2, %eax
    movl %eax, -64(%rbp)
    movl -56(%rbp), %eax
    cmpl -64(%rbp), %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    testl %eax, %eax
    jz main_2
main_1:
    lea -16(%rbp), %rax
    movq %rax, -56(%rbp)
    movl $0, %eax
    movl %eax, -64(%rbp)
    movslq -64(%rbp), %rax
    movq %rax, -64(%rbp)
    movl $4, %eax
    movq %rax, -72(%rbp)
    movq -64(%rbp), %rax
    imulq -72(%rbp)
    movq %rax, -64(%rbp)
    movq -56(%rbp), %rax
    addq -64(%rbp), %rax
    movq %rax, -56(%rbp)
    movl $1, %eax
    movl %eax, -64(%rbp)
    movq -56(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -72(%rbp)
    movl -72(%rbp), %eax
    movl -64(%rbp), %ecx
    shll %cl, %eax
    movl %eax, -64(%rbp)
    movq -56(%rbp), %rax
    movl -64(%rbp), %ebx
    movl %ebx, (%rax)
    jmp main_3
main_2:
    lea -16(%rbp), %rax
    movq %rax, -56(%rbp)
    movl $0, %eax
    movl %eax, -64(%rbp)
    movslq -64(%rbp), %rax
    movq %rax, -64(%rbp)
    movl $4, %eax
    movq %rax, -72(%rbp)
    movq -64(%rbp), %rax
    imulq -72(%rbp)
    movq %rax, -64(%rbp)
    movq -56(%rbp), %rax
    addq -64(%rbp), %rax
    movq %rax, -56(%rbp)
    movl $1, %eax
    movl %eax, -64(%rbp)
    movq -56(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -72(%rbp)
    movl -72(%rbp), %eax
    movl -64(%rbp), %ecx
    sarl %cl, %eax
    movl %eax, -64(%rbp)
    movq -56(%rbp), %rax
    movl -64(%rbp), %ebx
    movl %ebx, (%rax)
    jmp main_3
main_3:
    lea -16(%rbp), %rax
    movq %rax, -56(%rbp)
    movl $0, %eax
    movl %eax, -64(%rbp)
    movslq -64(%rbp), %rax
    movq %rax, -64(%rbp)
    movl $4, %eax
    movq %rax, -72(%rbp)
    movq -64(%rbp), %rax
    imulq -72(%rbp)
    movq %rax, -64(%rbp)
    movq -56(%rbp), %rax
    addq -64(%rbp), %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -56(%rbp)
    lea -16(%rbp), %rax
    movq %rax, -64(%rbp)
    movl $1, %eax
    movl %eax, -72(%rbp)
    movslq -72(%rbp), %rax
    movq %rax, -72(%rbp)
    movl $4, %eax
    movq %rax, -80(%rbp)
    movq -72(%rbp), %rax
    imulq -80(%rbp)
    movq %rax, -72(%rbp)
    movq -64(%rbp), %rax
    addq -72(%rbp), %rax
    movq %rax, -64(%rbp)
    movq -64(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -64(%rbp)
    movl -56(%rbp), %eax
    addl -64(%rbp), %eax
    movl %eax, -56(%rbp)
    lea -16(%rbp), %rax
    movq %rax, -64(%rbp)
    movl $2, %eax
    movl %eax, -72(%rbp)
    movslq -72(%rbp), %rax
    movq %rax, -72(%rbp)
    movl $4, %eax
    movq %rax, -80(%rbp)
    movq -72(%rbp), %rax
    imulq -80(%rbp)
    movq %rax, -72(%rbp)
    movq -64(%rbp), %rax
    addq -72(%rbp), %rax
    movq %rax, -64(%rbp)
    movq -64(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -64(%rbp)
    movl -56(%rbp), %eax
    addl -64(%rbp), %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
