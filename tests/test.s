.section .note.GNU-stack,"",@progbits

.text
.global add
add:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
add_0:
    movl %edi, -8(%rbp)
    movl %esi, -16(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -24(%rbp)
    leaq -16(%rbp), %rax
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
.global get_op
get_op:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
get_op_0:
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    leaq add(%rip), %rax
    movq %rax, -24(%rbp)
    movq -16(%rbp), %rax
    movq -24(%rbp), %rcx
    movq %rcx, (%rax)
    movq -8(%rbp), %rax
    mov %rbp, %rsp
    pop %rbp
    ret
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp
main_0:
    call get_op
    movq %rax, -8(%rbp)
    movl $2, %edi
    movl $4, %esi
    movq -8(%rbp), %rax
    call *%rax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
