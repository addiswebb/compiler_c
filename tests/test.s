.section .rodata
.section .text

.global main
test:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp
test_0:
    movl 16(%rbp), %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
main:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp
main_0:
    movl $40, %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    push %rax
    call test
    add $8, %rsp
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
