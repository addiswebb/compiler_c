
.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
main_0:
    leaq -16(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq $1, %rcx
    movq %rcx, (%rax)
    movq -40(%rbp), %rax
    addq $8, %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq $2, %rcx
    movq %rcx, (%rax)
    leaq -32(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq $3, %rcx
    movq %rcx, (%rax)
    movq -40(%rbp), %rax
    addq $8, %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq $4, %rcx
    movq %rcx, (%rax)
    leaq -16(%rbp), %rax
    movq %rax, -40(%rbp)
    leaq -32(%rbp), %rax
    movq %rax, -48(%rbp)
    subq $32, %rsp
    movl $2, %ecx
