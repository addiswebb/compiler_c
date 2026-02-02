.section .rodata
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $80, %rsp
main_0:
    movl $1, %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $2, %eax
    movl %eax, -72(%rbp)
    movslq -72(%rbp), %rax
    movq %rax, -72(%rbp)
    movq -72(%rbp), %rax
    movq %rax, -24(%rbp)
    movl $3, %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    movw %ax, -72(%rbp)
    movw -72(%rbp), %ax
    movw %ax, -40(%rbp)
    movl $4, %eax
    movl %eax, -72(%rbp)
    movl $8, %eax
    movl %eax, -80(%rbp)
    movl -72(%rbp), %eax
    addl -80(%rbp), %eax
    movl %eax, -72(%rbp)
    movl $2, %eax
    movl %eax, -80(%rbp)
    movl -72(%rbp), %eax
    addl -80(%rbp), %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
