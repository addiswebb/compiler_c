.global main
    jmp main
main:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
main_0:
    movl $3, %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    testl %eax, %eax
    jz main_2
main_1:
    movl $10, %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp main_2
main_2:
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
