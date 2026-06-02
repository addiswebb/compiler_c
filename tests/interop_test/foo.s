.section .rodata
.align 8
.LC0:
    .quad 1
.LC1:
    .byte '%', 'd', 0
.LC2:
    .byte '%', 'f', 0
.LC3:
    .byte '%', 's', 0
.LC4:
    .byte '%', 'p', 0
.align 8
.LC5:
    .quad 1
.align 8
.LC6:
    .quad 1

.text
.global vprint
vprint:
    push %rbp
    mov %rsp, %rbp
    subq $64, %rsp
vprint_0:
    movq %rcx, -8(%rbp)
    movq %rdx, -16(%rbp)
vprint_1:
    leaq -8(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movb (%rax), %al
    movb %al, -32(%rbp)
    movsbl -32(%rbp), %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    testl %eax, %eax
    jz vprint_14
vprint_2:
    leaq -8(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movb (%rax), %al
    movb %al, -32(%rbp)
    movsbl -32(%rbp), %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    cmpl $37, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    testl %eax, %eax
    jz vprint_12
vprint_3:
    leaq -8(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -40(%rbp)
    movl $1, %eax
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    addq -48(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    movq -40(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -8(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movb (%rax), %al
    movb %al, -32(%rbp)
    movsbl -32(%rbp), %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    cmpl $37, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz vprint_4
    movl -32(%rbp), %eax
    cmpl $100, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz vprint_5
    movl -32(%rbp), %eax
    cmpl $102, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz vprint_6
    movl -32(%rbp), %eax
    cmpl $115, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz vprint_7
    movl -32(%rbp), %eax
    cmpl $112, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz vprint_8
    movl -32(%rbp), %eax
    cmpl $99, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    testl %eax, %eax
    jz vprint_9
    jmp vprint_10
vprint_4:
    subq $32, %rsp
    movl $37, %ecx
    call putchar
    addq $32, %rsp
    movl %eax, -32(%rbp)
    jmp vprint_11
vprint_5:
    leaq .LC1(%rip), %rax
    movq %rax, -32(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -56(%rbp)
    movq -48(%rbp), %rax
    addq $8, %rax
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    movq -48(%rbp), %rcx
    movq %rcx, (%rax)
    subq $32, %rsp
    movq -32(%rbp), %rcx
    movl -56(%rbp), %edx
    call printf
    addq $32, %rsp
    movl %eax, -32(%rbp)
    jmp vprint_11
vprint_6:
    leaq .LC2(%rip), %rax
    movq %rax, -32(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -56(%rbp)
    movq -48(%rbp), %rax
    addq $8, %rax
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    movq -48(%rbp), %rcx
    movq %rcx, (%rax)
    subq $32, %rsp
    movq -32(%rbp), %rcx
    movq -56(%rbp), %rdx
    movsd -56(%rbp), %xmm1
    call printf
    addq $32, %rsp
    movl %eax, -32(%rbp)
    jmp vprint_11
vprint_7:
    leaq .LC3(%rip), %rax
    movq %rax, -32(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -56(%rbp)
    movq -48(%rbp), %rax
    addq $8, %rax
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    movq -48(%rbp), %rcx
    movq %rcx, (%rax)
    subq $32, %rsp
    movq -32(%rbp), %rcx
    movq -56(%rbp), %rdx
    call printf
    addq $32, %rsp
    movl %eax, -32(%rbp)
    jmp vprint_11
vprint_8:
    leaq .LC4(%rip), %rax
    movq %rax, -32(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -56(%rbp)
    movq -48(%rbp), %rax
    addq $8, %rax
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    movq -48(%rbp), %rcx
    movq %rcx, (%rax)
    subq $32, %rsp
    movq -32(%rbp), %rcx
    movq -56(%rbp), %rdx
    call printf
    addq $32, %rsp
    movl %eax, -32(%rbp)
    jmp vprint_11
vprint_9:
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -56(%rbp)
    movq -48(%rbp), %rax
    addq $8, %rax
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    movq -48(%rbp), %rcx
    movq %rcx, (%rax)
    movl -56(%rbp), %eax
    movb %al, -40(%rbp)
    movq -32(%rbp), %rax
    movb -40(%rbp), %cl
    movb %cl, (%rax)
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movb (%rax), %al
    movb %al, -32(%rbp)
    movsbl -32(%rbp), %eax
    movl %eax, -32(%rbp)
    subq $32, %rsp
    movl -32(%rbp), %ecx
    call putchar
    addq $32, %rsp
    movl %eax, -32(%rbp)
    jmp vprint_11
vprint_10:
    jmp vprint_11
vprint_11:
    leaq -8(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -40(%rbp)
    movl $1, %eax
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    addq -48(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    movq -40(%rbp), %rcx
    movq %rcx, (%rax)
    jmp vprint_13
vprint_12:
    leaq -8(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movb (%rax), %al
    movb %al, -32(%rbp)
    movsbl -32(%rbp), %eax
    movl %eax, -32(%rbp)
    subq $32, %rsp
    movl -32(%rbp), %ecx
    call putchar
    addq $32, %rsp
    movl %eax, -32(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -40(%rbp)
    movl $1, %eax
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    addq -48(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    movq -40(%rbp), %rcx
    movq %rcx, (%rax)
    jmp vprint_13
vprint_13:
    jmp vprint_1
vprint_14:
    mov %rbp, %rsp
    pop %rbp
    ret
