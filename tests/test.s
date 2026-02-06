.section .rodata
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
main_0:
    lea -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movl $69, %eax
    movl %eax, -32(%rbp)
    movq -24(%rbp), %rax
    movb -32(%rbp), %bl
    movb %bl, (%rax)
    lea -12(%rbp), %rax
    movq %rax, -24(%rbp)
    movl $12, %eax
    movl %eax, -32(%rbp)
    movq -24(%rbp), %rax
    movl -32(%rbp), %ebx
    movl %ebx, (%rax)
    lea -12(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
