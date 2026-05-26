.section .note.GNU-stack,"",@progbits

.text
.global foo
foo:
    push %rbp
    mov %rsp, %rbp
    subq $24, %rsp
foo_0:
    movq %rdi, -8(%rbp)
    leaq 56(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
