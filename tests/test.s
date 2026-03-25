
.text
.global foo
foo:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
foo_0:
    movq %rcx, -32(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rdx
    movq -40(%rbp), %rcx
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
    movq -40(%rbp), %rax
    addq $4, %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    movl -32(%rbp), %eax
    addl -40(%rbp), %eax
    movl %eax, -32(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    addq $8, %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    movl -32(%rbp), %eax
    addl -40(%rbp), %eax
    movl %eax, -32(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    addq $12, %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    movl -32(%rbp), %eax
    addl -40(%rbp), %eax
    movl %eax, -32(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    addq $16, %rax
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
    movq %rcx, -8(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -16(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    addq $4, %rax
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
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl $1, %ecx
    movl %ecx, (%rax)
    movq -40(%rbp), %rax
    addq $4, %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl $2, %ecx
    movl %ecx, (%rax)
    movq -40(%rbp), %rax
    addq $8, %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl $3, %ecx
    movl %ecx, (%rax)
    movq -40(%rbp), %rax
    addq $12, %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl $4, %ecx
    movl %ecx, (%rax)
    movq -40(%rbp), %rax
    addq $16, %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl $5, %ecx
    movl %ecx, (%rax)
    leaq -32(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl $10, %ecx
    movl %ecx, (%rax)
    movq -40(%rbp), %rax
    addq $4, %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl $20, %ecx
    movl %ecx, (%rax)
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    subq $32, %rsp
    movq -40(%rbp), %rcx
    call foo
    addq $32, %rsp
    movl %eax, -40(%rbp)
    leaq -32(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -48(%rbp)
    subq $32, %rsp
    movq -48(%rbp), %rcx
    call buzz
    addq $32, %rsp
    movl %eax, -48(%rbp)
    movl -40(%rbp), %eax
    addl -48(%rbp), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
