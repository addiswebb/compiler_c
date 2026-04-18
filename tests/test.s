.section .note.GNU-stack,"",@progbits

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
main_0:
    leaq -8(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl $1, %ecx
    movl %ecx, (%rax)
    leaq -16(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq $2, %rcx
    movq %rcx, (%rax)
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movw $3, %cx
    movw %cx, (%rax)
    leaq -32(%rbp), %rax
    movq %rax, -40(%rbp)
    movl $4, %eax
    addl $8, %eax
    movl %eax, -48(%rbp)
    movl -48(%rbp), %eax
    addl $2, %eax
    movl %eax, -48(%rbp)
    movq -40(%rbp), %rax
    movl -48(%rbp), %ecx
    movl %ecx, (%rax)
    leaq -32(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
