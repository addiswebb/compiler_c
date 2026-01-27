.section .rodata
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
main_0:
    movl $0, %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $10, %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    movl %eax, -16(%rbp)
    movl $0, %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    movl %eax, -24(%rbp)
main_1:
    movl -24(%rbp), %eax
    cmpl -16(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    testl %eax, %eax
    jz main_7
main_2:
    movl $2, %eax
    movl %eax, -32(%rbp)
    movl -24(%rbp), %eax
    cltd
    idivl -32(%rbp)
    movl %edx, -32(%rbp)
    movl $0, %eax
    movl %eax, -40(%rbp)
    movl -32(%rbp), %eax
    cmpl -40(%rbp), %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    testl %eax, %eax
    jz main_6
main_3:
    movl $3, %eax
    movl %eax, -32(%rbp)
    movl -24(%rbp), %eax
    cltd
    idivl -32(%rbp)
    movl %edx, -32(%rbp)
    movl $0, %eax
    movl %eax, -40(%rbp)
    movl -32(%rbp), %eax
    cmpl -40(%rbp), %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    testl %eax, %eax
    jz main_5
main_4:
    movl -24(%rbp), %eax
    movl %eax, -8(%rbp)
    jmp main_5
main_5:
    jmp main_6
main_6:
    movl $1, %eax
    movl %eax, -32(%rbp)
    movl -24(%rbp), %eax
    movl %eax, -40(%rbp)
    movl -24(%rbp), %eax
    addl -32(%rbp), %eax
    movl %eax, -24(%rbp)
    jmp main_1
main_7:
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
