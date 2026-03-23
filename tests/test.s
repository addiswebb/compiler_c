.section .rodata
.LC0:
    .byte '%', 'd', ' ', 0
.LC1:
    .byte 0x0A, 0

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $64, %rsp
main_0:
    leaq -8(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl $1, %ecx
    movl %ecx, (%rax)
    movq -40(%rbp), %rax
    subq $4, %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl $2, %ecx
    movl %ecx, (%rax)
    movq -40(%rbp), %rax
    subq $8, %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl $3, %ecx
    movl %ecx, (%rax)
    movq -40(%rbp), %rax
    subq $12, %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl $4, %ecx
    movl %ecx, (%rax)
    movq -40(%rbp), %rax
    subq $16, %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl $5, %ecx
    movl %ecx, (%rax)
    leaq -32(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
main_1:
    leaq -32(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    cmpl $5, %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz main_4
main_2:
    movq .LC0(%rip), %rax
    movq %rax, .LC0(%rip)
    leaq .LC0(%rip), %rax
    movq %rax, -40(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -48(%rbp)
    leaq -32(%rbp), %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -56(%rbp)
    movslq -56(%rbp), %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    imulq $4, %rax
    movq %rax, -56(%rbp)
    movq -48(%rbp), %rax
    subq -56(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -48(%rbp)
    subq $48, %rsp
    movq -40(%rbp), %rcx
    movl -48(%rbp), %edx
    call printf
    addq $48, %rsp
    movl %eax, -40(%rbp)
    jmp main_3
main_3:
    leaq -32(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -48(%rbp)
    movl -48(%rbp), %eax
    addl $1, %eax
    movl %eax, -48(%rbp)
    movq -40(%rbp), %rax
    movl -48(%rbp), %ecx
    movl %ecx, (%rax)
    jmp main_1
main_4:
    movq .LC1(%rip), %rax
    movq %rax, .LC1(%rip)
    leaq .LC1(%rip), %rax
    movq %rax, -40(%rbp)
    subq $48, %rsp
    movq -40(%rbp), %rcx
    call printf
    addq $48, %rsp
    movl %eax, -40(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -40(%rbp)
    movq $2, %rax
    imulq $4, %rax
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    subq -48(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
