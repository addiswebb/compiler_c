.section .rodata
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
main_0:
    movl $0, %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $0, %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    movl %eax, -16(%rbp)
    movl -8(%rbp), %eax
    testl %eax, %eax
    jz main_2
main_1:
    movl $1, %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    movl %eax, -16(%rbp)
    jmp main_3
main_2:
    movl $2, %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    movl %eax, -16(%rbp)
    jmp main_3
main_3:
    movl -16(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
