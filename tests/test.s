.section .rodata
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $64, %rsp
main_0:
    movl $1, %eax
    movl %eax, -48(%rbp)
    movl -48(%rbp), %eax
    movl %eax, -24(%rbp)
    movl $2, %eax
    movl %eax, -48(%rbp)
    movl -48(%rbp), %eax
    movl %eax, -20(%rbp)
    movl $3, %eax
    movl %eax, -48(%rbp)
    movl -48(%rbp), %eax
    movl %eax, -16(%rbp)
    movl $4, %eax
    movl %eax, -48(%rbp)
    movl -48(%rbp), %eax
    movl %eax, -12(%rbp)
    movl $5, %eax
    movl %eax, -48(%rbp)
    movl -48(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $0, %eax
    movl %eax, -48(%rbp)
    movl -48(%rbp), %eax
    movl %eax, -32(%rbp)
    movl $0, %eax
    movl %eax, -48(%rbp)
    movl -48(%rbp), %eax
    movl %eax, -40(%rbp)
main_1:
    movl $5, %eax
    movl %eax, -48(%rbp)
    movl -40(%rbp), %eax
    cmpl -48(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -48(%rbp)
    movl -48(%rbp), %eax
    testl %eax, %eax
    jz main_4
    jnz main_2
main_2:
    movslq -40(%rbp), %rax
    movq %rax, -48(%rbp)
    movl $4, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    imull -48(%rbp)
    movl %eax, -48(%rbp)
    lea -24(%rbp), %rax
    addq -48(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -48(%rbp)
    movl -32(%rbp), %eax
    addl -48(%rbp), %eax
    movl %eax, -48(%rbp)
    movl -48(%rbp), %eax
    movl %eax, -32(%rbp)
    jmp main_3
main_3:
    movl $1, %eax
    movl %eax, -48(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -56(%rbp)
    movl -40(%rbp), %eax
    addl -48(%rbp), %eax
    movl %eax, -40(%rbp)
    jmp main_1
main_4:
    movl -32(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
