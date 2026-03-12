.section .rodata

.text
.global foo
foo:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
foo_0:
    movq %rcx, %rax
    movq %rax, -8(%rbp)
    lea -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movl $4, %eax
    movq %rax, -32(%rbp)
    movq -24(%rbp), %rax
    addq -32(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
main_0:
    movl $1234569, %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $6, %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    movl %eax, -4(%rbp)
    subq $48, %rsp
    movq -8(%rbp), %rcx
    call foo
    addq $48, %rsp
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
