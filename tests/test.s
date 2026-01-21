.section .rodata
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp
main_0:
    movl $0, %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $0, %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    movl %eax, -24(%rbp)
    movl -8(%rbp), %eax
    testl %eax, %eax
    jz main_2
main_1:
    movl $1, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -24(%rbp)
    jmp main_3
main_2:
    movl $2, %eax
    movl %eax, -48(%rbp)
    movl -48(%rbp), %eax
    movl %eax, -24(%rbp)
    jmp main_3
main_3:
    movl -24(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
