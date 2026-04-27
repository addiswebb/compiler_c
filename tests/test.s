.section .note.GNU-stack,"",@progbits
.global arr
.data
.align 4
arr:
    .long 1
    .long 0
    .long 0
    .long 13
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp
main_0:
    leaq arr(%rip), %rax
    movq %rax, -8(%rbp)
    movl $7, %eax
    cltd
    movl $2, %ecx
    idivl %ecx
    movl %eax, -16(%rbp)
    movslq -16(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
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
