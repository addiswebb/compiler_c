.section .rodata
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $80, %rsp
main_0:
    movl $10, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -8(%rbp)
    lea -8(%rbp), %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    movq %rax, -24(%rbp)
    movl $5, %eax
    movl %eax, -56(%rbp)
    movq -24(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -64(%rbp)
    movl -64(%rbp), %eax
    addl -56(%rbp), %eax
    movl %eax, -56(%rbp)
    movq -24(%rbp), %rax
    movl -56(%rbp), %ebx
    movl %ebx, (%rax)
    lea -24(%rbp), %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -56(%rbp)
    movl $3, %eax
    movl %eax, -64(%rbp)
    movq -56(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -72(%rbp)
    movl -72(%rbp), %eax
    subl -64(%rbp), %eax
    movl %eax, -64(%rbp)
    movq -56(%rbp), %rax
    movl -64(%rbp), %ebx
    movl %ebx, (%rax)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
