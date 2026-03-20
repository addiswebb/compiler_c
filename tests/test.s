.section .rodata

.text
.global getSum
getSum:
    push %rbp
    mov %rsp, %rbp
    subq $80, %rsp
getSum_0:
    movl %ecx, %eax
    movl %eax, -8(%rbp)
    movq %rdx, %rax
    movq %rax, -16(%rbp)
    movq %r8, %rax
    movq %rax, -24(%rbp)
    movq %r9, %rax
    movq %rax, -32(%rbp)
    movl $0, %eax
    movl %eax, -64(%rbp)
    movl -64(%rbp), %eax
    movl %eax, -40(%rbp)
    leaq -8(%rbp), %rax
    subq $8, %rax
    movq %rax, -48(%rbp)
    movl $0, %eax
    movl %eax, -64(%rbp)
    movl -64(%rbp), %eax
    movl %eax, -56(%rbp)
getSum_1:
    movl -56(%rbp), %eax
    cmpl -8(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -64(%rbp)
    movl -64(%rbp), %eax
    testl %eax, %eax
    jz getSum_4
getSum_2:
    movq -48(%rbp), %rax
    subq $4, %rax
    movq %rax, -64(%rbp)
    movl -40(%rbp), %eax
    addl -64(%rbp), %eax
    movl %eax, -64(%rbp)
    movl -64(%rbp), %eax
    movl %eax, -40(%rbp)
    jmp getSum_3
getSum_3:
    movl $1, %eax
    movl %eax, -64(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -72(%rbp)
    movl -56(%rbp), %eax
    addl -64(%rbp), %eax
    movl %eax, -56(%rbp)
    jmp getSum_1
getSum_4:
    movl -40(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
main_0:
    movl $4, %eax
    movl %eax, -8(%rbp)
    movl $1, %eax
    movl %eax, -16(%rbp)
    movl $2, %eax
    movl %eax, -24(%rbp)
    movl $3, %eax
    movl %eax, -32(%rbp)
    movl $4, %eax
    movl %eax, -40(%rbp)
    subq $48, %rsp
    movl -8(%rbp), %ecx
    movl -16(%rbp), %edx
    movl -24(%rbp), %r8d
    movl -32(%rbp), %r9d
    movl -40(%rbp), %eax
    movl %eax, 32(%rsp)
    call getSum
    addq $48, %rsp
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
