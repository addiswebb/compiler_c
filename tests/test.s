.section .rodata

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $80, %rsp
main_0:
    movl $1, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -16(%rbp)
    movl $2, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -12(%rbp)
    movl $3, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -8(%rbp)
    lea -16(%rbp), %rax
    movq %rax, -56(%rbp)
    movl $2, %eax
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
    lea -16(%rbp), %rax
    movq %rax, -64(%rbp)
    movq -56(%rbp), %rax
    subq -64(%rbp), %rax
    movq %rax, -56(%rbp)
    movl $4, %eax
    movq %rax, -64(%rbp)
    movq -56(%rbp), %rax
    cqo
    idivq -64(%rbp)
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
