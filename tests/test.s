.section .note.GNU-stack,"",@progbits

.text
.global foo
foo:
    push %rbp
    mov %rsp, %rbp
    subq $96, %rsp
foo_0:
    movl %edi, -8(%rbp)
    movl %esi, -16(%rbp)
    movl %edx, -24(%rbp)
    movl %ecx, -32(%rbp)
    movl %r8d, -40(%rbp)
    movl %r9d, -48(%rbp)
    movl 16(%rbp), %eax
    movl %eax, -56(%rbp)
    movl 24(%rbp), %eax
    movl %eax, -64(%rbp)
    movl 32(%rbp), %eax
    movl %eax, -72(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -80(%rbp)
    movq -80(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -80(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -88(%rbp)
    movq -88(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -88(%rbp)
    movl -80(%rbp), %eax
    addl -88(%rbp), %eax
    movl %eax, -80(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -88(%rbp)
    movq -88(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -88(%rbp)
    movl -80(%rbp), %eax
    addl -88(%rbp), %eax
    movl %eax, -80(%rbp)
    leaq -32(%rbp), %rax
    movq %rax, -88(%rbp)
    movq -88(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -88(%rbp)
    movl -80(%rbp), %eax
    addl -88(%rbp), %eax
    movl %eax, -80(%rbp)
    leaq -40(%rbp), %rax
    movq %rax, -88(%rbp)
    movq -88(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -88(%rbp)
    movl -80(%rbp), %eax
    addl -88(%rbp), %eax
    movl %eax, -80(%rbp)
    leaq -48(%rbp), %rax
    movq %rax, -88(%rbp)
    movq -88(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -88(%rbp)
    movl -80(%rbp), %eax
    addl -88(%rbp), %eax
    movl %eax, -80(%rbp)
    leaq -56(%rbp), %rax
    movq %rax, -88(%rbp)
    movq -88(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -88(%rbp)
    movl -80(%rbp), %eax
    addl -88(%rbp), %eax
    movl %eax, -80(%rbp)
    leaq -64(%rbp), %rax
    movq %rax, -88(%rbp)
    movq -88(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -88(%rbp)
    movl -80(%rbp), %eax
    addl -88(%rbp), %eax
    movl %eax, -80(%rbp)
    leaq -72(%rbp), %rax
    movq %rax, -88(%rbp)
    movq -88(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -88(%rbp)
    movl -80(%rbp), %eax
    addl -88(%rbp), %eax
    movl %eax, -80(%rbp)
    movl -80(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp
main_0:
    subq $24, %rsp
    movl $1, %edi
    movl $2, %esi
    movl $3, %edx
    movl $4, %ecx
    movl $5, %r8d
    movl $6, %r9d
    movl $7, %eax
    movl %eax, 0(%rsp)
    movl $8, %eax
    movl %eax, 8(%rsp)
    movl $9, %eax
    movl %eax, 16(%rsp)
    call foo
    addq $24, %rsp
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
