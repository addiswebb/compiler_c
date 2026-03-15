.section .rodata
.LC0:
    .byte '%', 'd', ' ', '%', 'd', ' ', '%', 'd', ' ', '%', 'd', ' ', '%', 'd', ' ', '%', 'd', ' ', '%', 'd', ' ', '%', 'd', 0x0A, 0

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $80, %rsp
main_0:
    lea .LC0(%rip), %rax
    movq %rax, -8(%rbp)
    movl $1, %eax
    movl %eax, -16(%rbp)
    movl $2, %eax
    movl %eax, -24(%rbp)
    movl $3, %eax
    movl %eax, -32(%rbp)
    movl $4, %eax
    movl %eax, -40(%rbp)
    movl $5, %eax
    movl %eax, -48(%rbp)
    movl $6, %eax
    movl %eax, -56(%rbp)
    movl $7, %eax
    movl %eax, -64(%rbp)
    movl $8, %eax
    movl %eax, -72(%rbp)
    subq $80, %rsp
    movq -8(%rbp), %rcx
    movl -16(%rbp), %edx
    movl -24(%rbp), %r8d
    movl -32(%rbp), %r9d
    movl -40(%rbp), %eax
    movl %eax, 32(%rsp)
    movl -48(%rbp), %eax
    movl %eax, 40(%rsp)
    movl -56(%rbp), %eax
    movl %eax, 48(%rsp)
    movl -64(%rbp), %eax
    movl %eax, 56(%rsp)
    movl -72(%rbp), %eax
    movl %eax, 64(%rsp)
    call printf
    addq $80, %rsp
    movl %eax, -8(%rbp)
    movl $0, %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
