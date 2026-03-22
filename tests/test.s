.section .rodata

.text
.global foo
foo:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
foo_0:
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    movq %rcx, %rdx
    movq -32(%rbp), %rcx
    mov $20, %r8
    sub $40, %rsp
    call memcpy
    add $40, %rsp
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -32(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movl $4, %eax
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    addq -48(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    movl -32(%rbp), %eax
    addl -40(%rbp), %eax
    movl %eax, -32(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movl $8, %eax
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    addq -48(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    movl -32(%rbp), %eax
    addl -40(%rbp), %eax
    movl %eax, -32(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movl $12, %eax
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    addq -48(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    movl -32(%rbp), %eax
    addl -40(%rbp), %eax
    movl %eax, -32(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movl $16, %eax
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    addq -48(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    movl -32(%rbp), %eax
    addl -40(%rbp), %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
.global buzz
buzz:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
buzz_0:
    movq %rcx, %rax
    movq %rax, -8(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -16(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movl $4, %eax
    movq %rax, -32(%rbp)
    movq -24(%rbp), %rax
    addq -32(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -24(%rbp)
    movl -16(%rbp), %eax
    addl -24(%rbp), %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
main_0:
    movl $1, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -24(%rbp)
    movl $2, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -20(%rbp)
    movl $3, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -16(%rbp)
    movl $4, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -12(%rbp)
    movl $5, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $10, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -32(%rbp)
    movl $20, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -28(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    subq $48, %rsp
    movq -40(%rbp), %rcx
    call foo
    addq $48, %rsp
    movl %eax, -40(%rbp)
    subq $48, %rsp
    movq -32(%rbp), %rcx
    call buzz
    addq $48, %rsp
    movl %eax, -48(%rbp)
    movl -40(%rbp), %eax
    addl -48(%rbp), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
