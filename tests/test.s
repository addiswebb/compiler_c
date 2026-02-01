.section .rodata
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $64, %rsp
main_0:
    movl $1, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -16(%rbp)
    movl $2, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -12(%rbp)
    movl $3, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -8(%rbp)
    lea -16(%rbp), %rax
    movq %rax, -40(%rbp)
    movl $1, %eax
    movl %eax, -48(%rbp)
    movslq -48(%rbp), %rax
    movq %rax, -48(%rbp)
    movl $4, %eax
    movq %rax, -56(%rbp)
    movq -48(%rbp), %rax
    imulq -56(%rbp)
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    addq -48(%rbp), %rax
    movq %rax, -40(%rbp)
    movl $5, %eax
    movl %eax, -48(%rbp)
    movq -40(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -56(%rbp)
    movl -56(%rbp), %eax
    addl -48(%rbp), %eax
    movl %eax, -48(%rbp)
    movq -40(%rbp), %rax
    movq -48(%rbp), %rbx
    movq %rbx, (%rax)
    movl $2, %eax
    movl %eax, -40(%rbp)
    movslq -40(%rbp), %rax
    movq %rax, -40(%rbp)
    lea -16(%rbp), %rax
    movq %rax, -48(%rbp)
    movl $4, %eax
    movq %rax, -56(%rbp)
    movq -40(%rbp), %rax
    imulq -56(%rbp)
    movq %rax, -40(%rbp)
    movq -48(%rbp), %rax
    addq -40(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
