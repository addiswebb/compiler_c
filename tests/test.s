.section .note.GNU-stack,"",@progbits

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
main_0:
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movl $1, %ecx
    movl %ecx, (%rax)
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    addl $1, %eax
    movl %eax, -24(%rbp)
    movq -16(%rbp), %rax
    movl -24(%rbp), %ecx
    movl %ecx, (%rax)
    movl -24(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
