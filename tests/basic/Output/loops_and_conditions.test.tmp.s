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
    movl $1, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -24(%rbp)
main_1:
    movl $5, %eax
    movl %eax, -40(%rbp)
    movl -24(%rbp), %eax
    cmpl -40(%rbp), %eax
    setle %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz main_7
main_2:
    movl $2, %eax
    movl %eax, -40(%rbp)
    movl -24(%rbp), %eax
    cltd
    idivl -40(%rbp)
    movl %edx, -40(%rbp)
    movl $0, %eax
    movl %eax, -48(%rbp)
    movl -40(%rbp), %eax
    cmpl -48(%rbp), %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz main_4
main_3:
    movl -8(%rbp), %eax
    addl -24(%rbp), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -8(%rbp)
    jmp main_5
main_4:
    movl -8(%rbp), %eax
    subl -24(%rbp), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -8(%rbp)
    jmp main_5
main_5:
    jmp main_6
main_6:
    movl $1, %eax
    movl %eax, -40(%rbp)
    movl -24(%rbp), %eax
    movl %eax, -48(%rbp)
    movl -24(%rbp), %eax
    addl -40(%rbp), %eax
    movl %eax, -24(%rbp)
    jmp main_1
main_7:
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
