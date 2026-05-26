.section .note.GNU-stack,"",@progbits

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $56, %rsp
main_0:
    leaq -40(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movq $1, %rcx
    movq %rcx, (%rax)
    leaq -40(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    addq $8, %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movq $2, %rcx
    movq %rcx, (%rax)
    leaq -40(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    addq $16, %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movq $3, %rcx
    movq %rcx, (%rax)
    leaq -40(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    addq $24, %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movq $4, %rcx
    movq %rcx, (%rax)
    leaq -40(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    addq $32, %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movq $5, %rcx
    movq %rcx, (%rax)
    leaq -40(%rbp), %rax
    movq %rax, -48(%rbp)
    subq $40, %rsp
    movq -48(%rbp), %rsi
    leaq 0(%rsp), %rdi
    movq $40, %rdx
    call memcpy
    call foo
    addq $40, %rsp
    movl %eax, -48(%rbp)
    movl -48(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
