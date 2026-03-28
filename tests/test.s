.section .note.GNU-stack,"",@progbits

.text
.global add1
add1:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp
add1_0:
    movl %edi, -8(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    addl $1, %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
main_0:
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq $0, %rax
    imulq $8, %rax
    movq %rax, -24(%rbp)
    movq -16(%rbp), %rax
    addq -24(%rbp), %rax
    movq %rax, -16(%rbp)
    lea add1(%rip), %rax
    movq %rax, -24(%rbp)
    movq -16(%rbp), %rax
    movq -24(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -16(%rbp)
    movl $5, %edi
    movq -16(%rbp), %rax
    call *%rax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
