.section .rodata

.text
foo:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp
foo_0:
    movl %ecx, %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp
main_0:
    movl $10, %eax
    movl %eax, -8(%rbp)
    subq $48, %rsp
    movl -8(%rbp), %ecx
    call foo
    addq $48, %rsp
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
