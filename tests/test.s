.section .rodata

.text
.global foo
foo:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
foo_0:
    lea -16(%rbp), %rax
    movq %rax, -40(%rbp)
    movq %rcx, %rdx
    movq -40(%rbp), %rcx
    mov $12, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    lea -16(%rbp), %rax
    movq %rax, -40(%rbp)
    movl $4, %eax
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    addq -48(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
main_0:
    movl $5, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -16(%rbp)
    movl $98, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -12(%rbp)
    movl $97, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -8(%rbp)
    lea -16(%rbp), %rax
    movq %rax, -40(%rbp)
    subq $48, %rsp
    movq -40(%rbp), %rcx
    call foo
    addq $48, %rsp
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
