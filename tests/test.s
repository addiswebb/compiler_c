.section .rodata

.text
.global foo
foo:
    push %rbp
    mov %rsp, %rbp
    subq $80, %rsp
foo_0:
    leaq -24(%rbp), %rax
    movq %rax, -56(%rbp)
    movq %rcx, %rdx
    movq -56(%rbp), %rcx
    mov $20, %r8
    sub $40, %rsp
    call memcpy
    add $40, %rsp
    leaq -24(%rbp), %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -56(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -64(%rbp)
    movl $4, %eax
    movq %rax, -72(%rbp)
    movq -64(%rbp), %rax
    addq -72(%rbp), %rax
    movq %rax, -64(%rbp)
    movq -64(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -64(%rbp)
    movl -56(%rbp), %eax
    addl -64(%rbp), %eax
    movl %eax, -56(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -64(%rbp)
    movl $8, %eax
    movq %rax, -72(%rbp)
    movq -64(%rbp), %rax
    addq -72(%rbp), %rax
    movq %rax, -64(%rbp)
    movq -64(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -64(%rbp)
    movl -56(%rbp), %eax
    addl -64(%rbp), %eax
    movl %eax, -56(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -64(%rbp)
    movl $12, %eax
    movq %rax, -72(%rbp)
    movq -64(%rbp), %rax
    addq -72(%rbp), %rax
    movq %rax, -64(%rbp)
    movq -64(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -64(%rbp)
    movl -56(%rbp), %eax
    addl -64(%rbp), %eax
    movl %eax, -56(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -64(%rbp)
    movl $16, %eax
    movq %rax, -72(%rbp)
    movq -64(%rbp), %rax
    addq -72(%rbp), %rax
    movq %rax, -64(%rbp)
    movq -64(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -64(%rbp)
    movl -56(%rbp), %eax
    addl -64(%rbp), %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
.global buzz
buzz:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
buzz_0:
    movq %rcx, %rax
    movq %rax, -8(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -24(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -32(%rbp)
    movl $4, %eax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    addq -40(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -32(%rbp)
    movl -24(%rbp), %eax
    addl -32(%rbp), %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $80, %rsp
main_0:
    movl $1, %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    movl %eax, -24(%rbp)
    movl $2, %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    movl %eax, -20(%rbp)
    movl $3, %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    movl %eax, -16(%rbp)
    movl $4, %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    movl %eax, -12(%rbp)
    movl $5, %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $10, %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    movl %eax, -56(%rbp)
    movl $20, %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    movl %eax, -52(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -72(%rbp)
    subq $48, %rsp
    movq -72(%rbp), %rcx
    call foo
    addq $48, %rsp
    movl %eax, -72(%rbp)
    subq $48, %rsp
    movq -56(%rbp), %rcx
    call buzz
    addq $48, %rsp
    movl %eax, -80(%rbp)
    movl -72(%rbp), %eax
    addl -80(%rbp), %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
