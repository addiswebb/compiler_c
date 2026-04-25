.section .note.GNU-stack,"",@progbits
.global a
.data
.align 4
a:
    .long 4

.text
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
    movq $4, %rax
    imulq $4, %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    addq -40(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
