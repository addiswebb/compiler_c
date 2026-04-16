
.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
main_0:
    leaq -16(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl $1, %ecx
    movl %ecx, (%rax)
    movq -32(%rbp), %rax
    addq $4, %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl $2, %ecx
    movl %ecx, (%rax)
    movq -32(%rbp), %rax
    addq $8, %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl $3, %ecx
    movl %ecx, (%rax)
    movq -32(%rbp), %rax
    addq $12, %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl $4, %ecx
    movl %ecx, (%rax)
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    movq -40(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -40(%rbp)
    movq $1, %rax
    imulq $4, %rax
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    addq -48(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    movq -40(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -32(%rbp)
    leaq -32(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl $5, %ecx
    movl %ecx, (%rax)
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
