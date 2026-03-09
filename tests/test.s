.section .rodata
.LC6:
    .byte '%', 'd', ',', ' ', 0

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $80, %rsp
main_0:
    movl $3, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -16(%rbp)
    movl $0, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -12(%rbp)
    movl $3, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $1, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -4(%rbp)
    movl $0, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -40(%rbp)
main_1:
    movl $4, %eax
    movl %eax, -56(%rbp)
    movl -40(%rbp), %eax
    cmpl -56(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    testl %eax, %eax
    jz main_4
main_2:
    lea .LC6(%rip), %rax
    movq %rax, -56(%rbp)
    lea -16(%rbp), %rax
    movq %rax, -64(%rbp)
    movslq -40(%rbp), %rax
    movq %rax, -72(%rbp)
    movl $4, %eax
    movq %rax, -80(%rbp)
    movq -72(%rbp), %rax
    imulq -80(%rbp)
    movq %rax, -72(%rbp)
    movq -64(%rbp), %rax
    addq -72(%rbp), %rax
    movq %rax, -64(%rbp)
    movq -64(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -64(%rbp)
    subq $48, %rsp
    movq -56(%rbp), %rcx
    movl -64(%rbp), %edx
    call printf
    addq $48, %rsp
    movl %eax, -56(%rbp)
    jmp main_3
main_3:
    movl $1, %eax
    movl %eax, -56(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -64(%rbp)
    movl -40(%rbp), %eax
    addl -56(%rbp), %eax
    movl %eax, -40(%rbp)
    jmp main_1
main_4:
    movl $0, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
