.section .rodata
.LC0:
    .string "abc"
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $80, %rsp
main_0:
    lea .LC0(%rip), %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rdx
    lea -8(%rbp), %rcx
    mov $4, %r8d
    call memcpy
    lea -8(%rbp), %rax
    movq %rax, -56(%rbp)
    movl $0, %eax
    movl %eax, -64(%rbp)
    movslq -64(%rbp), %rax
    movq %rax, -64(%rbp)
    movl $1, %eax
    movq %rax, -72(%rbp)
    movq -64(%rbp), %rax
    imulq -72(%rbp)
    movq %rax, -64(%rbp)
    movq -56(%rbp), %rax
    addq -64(%rbp), %rax
    movq %rax, -56(%rbp)
    movl $122, %eax
    movb %al, -64(%rbp)
    movq -56(%rbp), %rax
    movb -64(%rbp), %bl
    movb %bl, (%rax)
    movl $1, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -32(%rbp)
    movl $2, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -28(%rbp)
    movl $3, %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    movl %eax, -24(%rbp)
    lea -32(%rbp), %rax
    movq %rax, -56(%rbp)
    movl $1, %eax
    movl %eax, -64(%rbp)
    movslq -64(%rbp), %rax
    movq %rax, -64(%rbp)
    movl $4, %eax
    movq %rax, -72(%rbp)
    movq -64(%rbp), %rax
    imulq -72(%rbp)
    movq %rax, -64(%rbp)
    movq -56(%rbp), %rax
    addq -64(%rbp), %rax
    movq %rax, -56(%rbp)
    movl $5, %eax
    movl %eax, -64(%rbp)
    movq -56(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -72(%rbp)
    movl -72(%rbp), %eax
    imull -64(%rbp)
    movl %eax, -64(%rbp)
    movq -56(%rbp), %rax
    movl -64(%rbp), %ebx
    movl %ebx, (%rax)
    lea -32(%rbp), %rax
    movq %rax, -56(%rbp)
    movl $0, %eax
    movl %eax, -64(%rbp)
    movslq -64(%rbp), %rax
    movq %rax, -64(%rbp)
    movl $4, %eax
    movq %rax, -72(%rbp)
    movq -64(%rbp), %rax
    imulq -72(%rbp)
    movq %rax, -64(%rbp)
    movq -56(%rbp), %rax
    addq -64(%rbp), %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -56(%rbp)
    lea -32(%rbp), %rax
    movq %rax, -64(%rbp)
    movl $1, %eax
    movl %eax, -72(%rbp)
    movslq -72(%rbp), %rax
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
    movl -56(%rbp), %eax
    addl -64(%rbp), %eax
    movl %eax, -56(%rbp)
    lea -32(%rbp), %rax
    movq %rax, -64(%rbp)
    movl $2, %eax
    movl %eax, -72(%rbp)
    movslq -72(%rbp), %rax
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
    movl -56(%rbp), %eax
    addl -64(%rbp), %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
