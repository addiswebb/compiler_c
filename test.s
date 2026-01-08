.global main
main:
    push %rbp
    mov %rbp, %rsp
    sub $16, %rsp

    movq $1, -16(%rbp)
    mov %rax, -16(%rbp)
    mov -8(%rbp), %rax
    mov -8(%rbp), %rax

    mov %rsp, %rbp
    pop %rbp
    ret
