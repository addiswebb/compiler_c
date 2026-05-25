.section .note.GNU-stack,"",@progbits

.text
.global foo
foo:
    push %rbp
    mov %rsp, %rbp
    subq $72, %rsp
foo_0:
    movq %rdi, %rax
    movq %rax, -48(%rbp)
    leaq -40(%rbp), %rax
    movq %rax, -56(%rbp)
    movq -48(%rbp), %rsi
    movq -56(%rbp), %rdi
    movq $40, %rdx
    call memcpy
    leaq -40(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -48(%rbp)
    leaq -40(%rbp), %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    addq $8, %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -56(%rbp)
    movq -48(%rbp), %rax
    addq -56(%rbp), %rax
    movq %rax, -48(%rbp)
    leaq -40(%rbp), %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    addq $16, %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -56(%rbp)
    movq -48(%rbp), %rax
    addq -56(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl %eax, -48(%rbp)
    movl -48(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
