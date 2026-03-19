.section .rodata

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
main_0:
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -24(%rbp)
    lea -24(%rbp), %rax
    movq %rax, -24(%rbp)
    movl $0, %eax
    movl %eax, -32(%rbp)
    movslq -32(%rbp), %rax
    movq %rax, -32(%rbp)
    movl $4, %eax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    imulq -40(%rbp)
    movq %rax, -32(%rbp)
    movq -24(%rbp), %rax
    addq -32(%rbp), %rax
    movq %rax, -24(%rbp)
    movl $5, %eax
    movl %eax, -32(%rbp)
    movq -24(%rbp), %rax
    movl -32(%rbp), %ecx
    movl %ecx, (%rax)
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -24(%rbp)
    lea -24(%rbp), %rax
    movq %rax, -24(%rbp)
    movl $1, %eax
    movl %eax, -32(%rbp)
    movslq -32(%rbp), %rax
    movq %rax, -32(%rbp)
    movl $4, %eax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    imulq -40(%rbp)
    movq %rax, -32(%rbp)
    movq -24(%rbp), %rax
    addq -32(%rbp), %rax
    movq %rax, -24(%rbp)
    movl $6, %eax
    movl %eax, -32(%rbp)
    movq -24(%rbp), %rax
    movl -32(%rbp), %ecx
    movl %ecx, (%rax)
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -24(%rbp)
    lea -24(%rbp), %rax
    movq %rax, -24(%rbp)
    movl $0, %eax
    movl %eax, -32(%rbp)
    movslq -32(%rbp), %rax
    movq %rax, -32(%rbp)
    movl $4, %eax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    imulq -40(%rbp)
    movq %rax, -32(%rbp)
    movq -24(%rbp), %rax
    addq -32(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
