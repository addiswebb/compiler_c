.section .rodata
.LC0:
    .string "xyz"
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $64, %rsp
main_0:
    lea .LC0(%rip), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rdx
    lea -8(%rbp), %rcx
    mov $4, %r8d
    call memcpy
    lea -8(%rbp), %rax
    movq %rax, -40(%rbp)
    movl $0, %eax
    movl %eax, -48(%rbp)
    movslq -48(%rbp), %rax
    movq %rax, -48(%rbp)
    movl $1, %eax
    movq %rax, -56(%rbp)
    movq -48(%rbp), %rax
    imulq -56(%rbp)
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    addq -48(%rbp), %rax
    movq %rax, -40(%rbp)
    lea -8(%rbp), %rax
    movq %rax, -48(%rbp)
    movl $1, %eax
    movl %eax, -56(%rbp)
    movslq -56(%rbp), %rax
    movq %rax, -56(%rbp)
    movl $1, %eax
    movq %rax, -64(%rbp)
    movq -56(%rbp), %rax
    imulq -64(%rbp)
    movq %rax, -56(%rbp)
    movq -48(%rbp), %rax
    addq -56(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movb (%rax), %al
    movb %al, -48(%rbp)
    movq -40(%rbp), %rax
    movb -48(%rbp), %bl
    movb %bl, (%rax)
    lea -8(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq %rax, -24(%rbp)
    movl $2, %eax
    movl %eax, -40(%rbp)
    movslq -40(%rbp), %rax
    movq %rax, -40(%rbp)
    movl $1, %eax
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    imulq -48(%rbp)
    movq %rax, -40(%rbp)
    movq -24(%rbp), %rax
    addq -40(%rbp), %rax
    movq %rax, -40(%rbp)
    movl $97, %eax
    movb %al, -48(%rbp)
    movq -40(%rbp), %rax
    movb -48(%rbp), %bl
    movb %bl, (%rax)
    lea -8(%rbp), %rax
    movq %rax, -40(%rbp)
    movl $0, %eax
    movl %eax, -48(%rbp)
    movslq -48(%rbp), %rax
    movq %rax, -48(%rbp)
    movl $1, %eax
    movq %rax, -56(%rbp)
    movq -48(%rbp), %rax
    imulq -56(%rbp)
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    addq -48(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movb (%rax), %al
    movb %al, -40(%rbp)
    movsbl -40(%rbp), %eax
    movl %eax, -40(%rbp)
    lea -8(%rbp), %rax
    movq %rax, -48(%rbp)
    movl $1, %eax
    movl %eax, -56(%rbp)
    movslq -56(%rbp), %rax
    movq %rax, -56(%rbp)
    movl $1, %eax
    movq %rax, -64(%rbp)
    movq -56(%rbp), %rax
    imulq -64(%rbp)
    movq %rax, -56(%rbp)
    movq -48(%rbp), %rax
    addq -56(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movb (%rax), %al
    movb %al, -48(%rbp)
    movsbl -48(%rbp), %eax
    movl %eax, -48(%rbp)
    movl -40(%rbp), %eax
    addl -48(%rbp), %eax
    movl %eax, -40(%rbp)
    lea -8(%rbp), %rax
    movq %rax, -48(%rbp)
    movl $2, %eax
    movl %eax, -56(%rbp)
    movslq -56(%rbp), %rax
    movq %rax, -56(%rbp)
    movl $1, %eax
    movq %rax, -64(%rbp)
    movq -56(%rbp), %rax
    imulq -64(%rbp)
    movq %rax, -56(%rbp)
    movq -48(%rbp), %rax
    addq -56(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movb (%rax), %al
    movb %al, -48(%rbp)
    movsbl -48(%rbp), %eax
    movl %eax, -48(%rbp)
    movl -40(%rbp), %eax
    addl -48(%rbp), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
