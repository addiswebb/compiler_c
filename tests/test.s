.section .rodata
.LC3:
    .byte 'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'o', 0
.LC10:
    .byte '%', 's', 0x0A, 0

.text
.global is_num
is_num:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
is_num_0:
    movb %cl, %al
    movb %al, -8(%rbp)
    movsbl -8(%rbp), %eax
    movl %eax, -24(%rbp)
    movl $48, %eax
    movl %eax, -32(%rbp)
    movl -24(%rbp), %eax
    cmpl -32(%rbp), %eax
    setge %al
    movzbl %al, %eax
    movl %eax, -24(%rbp)
    movl $0, %eax
    movl %eax, -32(%rbp)
    movl -24(%rbp), %eax
    cmpl -32(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -24(%rbp)
    movsbl -8(%rbp), %eax
    movl %eax, -40(%rbp)
    movl $57, %eax
    movl %eax, -48(%rbp)
    movl -40(%rbp), %eax
    cmpl -48(%rbp), %eax
    setle %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    cmpl -32(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -32(%rbp)
    movl -24(%rbp), %eax
    andl -32(%rbp), %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $64, %rsp
main_0:
    leaq -16(%rbp), %rax
    movq %rax, -40(%rbp)
    lea .LC3(%rip), %rdx
    movq -40(%rbp), %rcx
    mov $12, %r8
    sub $40, %rsp
    call memcpy
    add $40, %rsp
    lea -16(%rbp), %rax
    movq %rax, -40(%rbp)
    lea -16(%rbp), %rax
    movq %rax, -48(%rbp)
    subq $48, %rsp
    movq -48(%rbp), %rcx
    call strlen
    addq $48, %rsp
    movq %rax, -48(%rbp)
    movl $1, %eax
    movl %eax, -56(%rbp)
    movslq -56(%rbp), %rax
    movq %rax, -56(%rbp)
    movq -48(%rbp), %rax
    subq -56(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movq %rax, -48(%rbp)
    movl $1, %eax
    movq %rax, -56(%rbp)
    movq -48(%rbp), %rax
    imulq -56(%rbp)
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    addq -48(%rbp), %rax
    movq %rax, -40(%rbp)
    movl $100, %eax
    movl %eax, -48(%rbp)
    movq -40(%rbp), %rax
    movb -48(%rbp), %cl
    movb %cl, (%rax)
    movl $97, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movb %al, -40(%rbp)
    subq $48, %rsp
    movb -40(%rbp), %cl
    call is_num
    addq $48, %rsp
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
    jz main_2
main_1:
    movl $5, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp main_2
main_2:
    lea .LC10(%rip), %rax
    movq %rax, -40(%rbp)
    lea -16(%rbp), %rax
    movq %rax, -48(%rbp)
    subq $48, %rsp
    movq -40(%rbp), %rcx
    movq -48(%rbp), %rdx
    call printf
    addq $48, %rsp
    movl %eax, -40(%rbp)
    movl $0, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
