.section .rodata

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
main_0:
    movl $10, %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    movl %eax, -8(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movq %rax, -16(%rbp)
    movl $5, %eax
    movl %eax, -32(%rbp)
    movq -16(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -40(%rbp)
    movl -40(%rbp), %eax
    addl -32(%rbp), %eax
    movl %eax, -32(%rbp)
    movq -16(%rbp), %rax
    movl -32(%rbp), %ecx
    movl %ecx, (%rax)
    leaq -16(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -32(%rbp)
    movl $3, %eax
    movl %eax, -40(%rbp)
    movq -32(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -48(%rbp)
    movl -48(%rbp), %eax
    subl -40(%rbp), %eax
    movl %eax, -40(%rbp)
    movq -32(%rbp), %rax
    movl -40(%rbp), %ecx
    movl %ecx, (%rax)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
