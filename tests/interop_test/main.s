
.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
main_0:
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl $1, %ecx
    movl %ecx, (%rax)
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    addq $4, %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl $2, %ecx
    movl %ecx, (%rax)
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    addq $8, %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl $3, %ecx
    movl %ecx, (%rax)
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    addq $12, %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl $4, %ecx
    movl %ecx, (%rax)
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    addq $16, %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl $5, %ecx
    movl %ecx, (%rax)
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    subq $32, %rsp
    movq $123, %rcx
    movq -32(%rbp), %rdx
    movq -40(%rbp), %r8
    call foo
    addq $32, %rsp
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
