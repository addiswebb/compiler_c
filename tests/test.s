
.text
.global foo
foo:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
foo_0:
    movl %ecx, -8(%rbp)
    movl %edx, -16(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    movq -32(%rbp), %rax
    movl -40(%rbp), %ecx
    movl %ecx, (%rax)
    leaq -16(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    movq -32(%rbp), %rax
    addq $4, %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl -40(%rbp), %ecx
    movl %ecx, (%rax)
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
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
    movq %rax, -24(%rbp)
    subq $32, %rsp
    movl $1, %ecx
    movl $5, %edx
    call foo
    addq $32, %rsp
    movq %rax, -16(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -16(%rbp), %rdx
    movq -24(%rbp), %rcx
    mov $8, %r8
    sub $40, %rsp
    call memcpy
    add $40, %rsp
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
