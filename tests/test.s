.section .note.GNU-stack,"",@progbits
.global arr
.data

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp
main_0:
    leaq arr(%rip), %rax
    movq %rax, -8(%rbp)
    movq $0, %rax
    imulq $4, %rax
    movq %rax, -16(%rbp)
    movq -8(%rbp), %rax
    addq -16(%rbp), %rax
    movq %rax, -8(%rbp)
    movq -8(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
