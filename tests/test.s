.section .rodata
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
main_0:
    movl $0, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $10, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -16(%rbp)
    movl $0, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -24(%rbp)
main_1:
    movl -24(%rbp), %eax
    cmpl -16(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz main_10
    jnz main_2
main_2:
    movl $0, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -32(%rbp)
main_3:
    movl -32(%rbp), %eax
    cmpl -16(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz main_8
    jnz main_4
main_4:
    movl $1, %eax
    movl %eax, -40(%rbp)
    movl -8(%rbp), %eax
    movl %eax, -48(%rbp)
    movl -8(%rbp), %eax
    addl -40(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $4, %eax
    movl %eax, -40(%rbp)
    movl -32(%rbp), %eax
    cltd
    idivl -40(%rbp)
    movl %eax, -40(%rbp)
    movl $1, %eax
    movl %eax, -48(%rbp)
    movl -40(%rbp), %eax
    cmpl -48(%rbp), %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz main_6
    jnz main_5
main_5:
    jmp main_8
    jmp main_6
main_6:
    jmp main_7
main_7:
    movl $1, %eax
    movl %eax, -40(%rbp)
    movl -32(%rbp), %eax
    movl %eax, -48(%rbp)
    movl -32(%rbp), %eax
    addl -40(%rbp), %eax
    movl %eax, -32(%rbp)
    jmp main_3
main_8:
    jmp main_9
    jmp main_9
main_9:
    movl $1, %eax
    movl %eax, -40(%rbp)
    movl -24(%rbp), %eax
    movl %eax, -48(%rbp)
    movl -24(%rbp), %eax
    addl -40(%rbp), %eax
    movl %eax, -24(%rbp)
    jmp main_1
main_10:
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
