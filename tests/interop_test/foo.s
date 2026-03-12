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
