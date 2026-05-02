.section .note.GNU-stack,"",@progbits
.section .rodata
.LC0:
    .byte '%', 'd', ' ', 0
.LC1:
    .byte 0x0A, 0

.text
.global cmp_int
cmp_int:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
cmp_int_0:
    movq %rdi, -8(%rbp)
    movq %rsi, -16(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -48(%rbp)
    movq -40(%rbp), %rax
    movl -48(%rbp), %ecx
    movl %ecx, (%rax)
    leaq -32(%rbp), %rax
    movq %rax, -40(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -48(%rbp)
    movq -40(%rbp), %rax
    movl -48(%rbp), %ecx
    movl %ecx, (%rax)
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    leaq -32(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -48(%rbp)
    movl -40(%rbp), %eax
    cmpl -48(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz cmp_int_2
cmp_int_1:
    movl $-1, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp cmp_int_2
cmp_int_2:
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    leaq -32(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -48(%rbp)
    movl -40(%rbp), %eax
    cmpl -48(%rbp), %eax
    setg %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz cmp_int_4
cmp_int_3:
    movl $1, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp cmp_int_4
cmp_int_4:
    movl $0, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $64, %rsp
main_0:
    leaq -24(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl $5, %ecx
    movl %ecx, (%rax)
    movq -48(%rbp), %rax
    addq $4, %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    movl $1, %ecx
    movl %ecx, (%rax)
    movq -48(%rbp), %rax
    addq $8, %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    movl $4, %ecx
    movl %ecx, (%rax)
    movq -48(%rbp), %rax
    addq $12, %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    movl $2, %ecx
    movl %ecx, (%rax)
    movq -48(%rbp), %rax
    addq $16, %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl $3, %ecx
    movl %ecx, (%rax)
    leaq -32(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl $5, %ecx
    movl %ecx, (%rax)
    leaq -24(%rbp), %rax
    movq %rax, -48(%rbp)
    leaq -32(%rbp), %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -56(%rbp)
    movslq -56(%rbp), %rax
    movq %rax, -56(%rbp)
    leaq cmp_int(%rip), %rax
    movq %rax, -64(%rbp)
    movq -48(%rbp), %rdi
    movq -56(%rbp), %rsi
    movq $4, %rdx
    movq -64(%rbp), %rcx
    call qsort
    leaq -40(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
main_1:
    leaq -40(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -48(%rbp)
    leaq -32(%rbp), %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -56(%rbp)
    movl -48(%rbp), %eax
    cmpl -56(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -48(%rbp)
    movl -48(%rbp), %eax
    testl %eax, %eax
    jz main_4
main_2:
    leaq .LC0(%rip), %rax
    movq %rax, -48(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -56(%rbp)
    leaq -40(%rbp), %rax
    movq %rax, -64(%rbp)
    movq -64(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -64(%rbp)
    movslq -64(%rbp), %rax
    movq %rax, -64(%rbp)
    movq -64(%rbp), %rax
    imulq $4, %rax
    movq %rax, -64(%rbp)
    movq -56(%rbp), %rax
    addq -64(%rbp), %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -56(%rbp)
    subq $176, %rsp
    movq -48(%rbp), %rdi
    movl -56(%rbp), %esi
    xor %eax, %eax
    call printf
    addq $176, %rsp
    movl %eax, -48(%rbp)
    jmp main_3
main_3:
    leaq -40(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -56(%rbp)
    movl -56(%rbp), %eax
    addl $1, %eax
    movl %eax, -56(%rbp)
    movq -48(%rbp), %rax
    movl -56(%rbp), %ecx
    movl %ecx, (%rax)
    jmp main_1
main_4:
    leaq .LC1(%rip), %rax
    movq %rax, -48(%rbp)
    subq $176, %rsp
    movq -48(%rbp), %rdi
    xor %eax, %eax
    call printf
    addq $176, %rsp
    movl %eax, -48(%rbp)
    movl $0, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
