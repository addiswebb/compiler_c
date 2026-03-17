.section .note.GNU-stack,"",@progbits
.section .rodata

.text
.global foo
foo:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
foo_0:
    movl %edi, %eax
    movl %eax, -8(%rbp)
    movl $5, %eax
    movl %eax, -24(%rbp)
    movl -8(%rbp), %eax
    imull -24(%rbp)
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
    leaq foo(%rip), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq %rax, -8(%rbp)
    movl $4, %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %edi
    movq -8(%rbp), %rax
    call *%rax
    addq $0, %rsp
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
