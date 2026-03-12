.section .rodata

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $64, %rsp
main_0:
    movl $1, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -24(%rbp)
    movl $2, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -20(%rbp)
    movl $3, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -16(%rbp)
    movl $4, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -12(%rbp)
    movl $5, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -8(%rbp)
    lea -24(%rbp), %rax
    movq %rax, -56(%rbp)
    subq $48, %rsp
    movq -56(%rbp), %rcx
    call foo
    addq $48, %rsp
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
