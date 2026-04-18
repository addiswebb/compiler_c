.section .note.GNU-stack,"",@progbits

.text
.global foo
foo:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
foo_0:
    movq %rdi, %rax
    movq %rax, -32(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rsi
    movq -40(%rbp), %rdi
    mov $20, %rdx
    call memcpy
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -32(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    addq $4, %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    movl -32(%rbp), %eax
    addl -40(%rbp), %eax
    movl %eax, -32(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    addq $8, %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    movl -32(%rbp), %eax
    addl -40(%rbp), %eax
    movl %eax, -32(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    addq $12, %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    movl -32(%rbp), %eax
    addl -40(%rbp), %eax
    movl %eax, -32(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    addq $16, %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    movl -32(%rbp), %eax
    addl -40(%rbp), %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
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
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl $4, %ecx
    movl %ecx, (%rax)
    movq -32(%rbp), %rax
    addq $16, %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl $5, %ecx
    movl %ecx, (%rax)
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rdi
    call foo
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
