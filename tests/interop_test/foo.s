
.text
.global foo
foo:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
foo_0:
    movq %rcx, -8(%rbp)
    movq %rdx, %rax
    movq %rax, -16(%rbp)
    movq %r8, %rax
    movq %rax, -24(%rbp)
    leaq 16(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rcx
    movq -16(%rbp), %rdx
    mov $20, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    leaq 40(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rcx
    movq -24(%rbp), %rdx
    mov $20, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    leaq 16(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -16(%rbp)
    leaq 16(%rbp), %rax
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
    leaq 16(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    addq $8, %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -24(%rbp)
    movl -16(%rbp), %eax
    addl -24(%rbp), %eax
    movl %eax, -16(%rbp)
    leaq 40(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    addq $12, %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -24(%rbp)
    movl -16(%rbp), %eax
    addl -24(%rbp), %eax
    movl %eax, -16(%rbp)
    leaq 40(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    addq $16, %rax
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
