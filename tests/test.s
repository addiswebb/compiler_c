.section .rodata
.LC4:
    .byte '%', 'd', ' ', 0
.LC5:
    .byte '%', 'd', ' ', 0
.LC6:
    .byte '%', 'd', ' ', 0
.LC7:
    .byte '%', 'd', ' ', 0
.LC8:
    .byte '%', 'd', ' ', 0
.LC9:
    .byte '%', 'd', ' ', 0
.LC10:
    .byte '%', 'd', ' ', 0
.LC11:
    .byte '%', 'd', ' ', 0
.LC12:
    .byte '%', 'd', ' ', 0
.LC13:
    .byte '%', 'd', ' ', 0
.LC14:
    .byte '%', 'd', ' ', 0
.LC15:
    .byte '%', 'd', ' ', 0
.LC16:
    .byte '%', 'd', ' ', 0
.LC17:
    .byte '%', 'u', ' ', 0
.LC18:
    .byte '%', 'd', ' ', 0
.LC19:
    .byte '%', 'u', ' ', 0
.LC20:
    .byte '%', 'd', ' ', 0
.LC21:
    .byte '%', 'u', 0x0A, 0

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $80, %rsp
main_0:
    movl $-20, %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $6, %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    movl %eax, -24(%rbp)
    movl $20, %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    movl %eax, -40(%rbp)
    movl $6, %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    movl %eax, -56(%rbp)
    lea .LC4(%rip), %rax
    movq %rax, -72(%rbp)
    movl -8(%rbp), %eax
    cmpl -24(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -80(%rbp)
    subq $48, %rsp
    movq -72(%rbp), %rcx
    movl -80(%rbp), %edx
    call printf
    addq $48, %rsp
    movl %eax, -72(%rbp)
    lea .LC5(%rip), %rax
    movq %rax, -72(%rbp)
    movl -8(%rbp), %eax
    cmpl -24(%rbp), %eax
    setle %al
    movzbl %al, %eax
    movl %eax, -80(%rbp)
    subq $48, %rsp
    movq -72(%rbp), %rcx
    movl -80(%rbp), %edx
    call printf
    addq $48, %rsp
    movl %eax, -72(%rbp)
    lea .LC6(%rip), %rax
    movq %rax, -72(%rbp)
    movl -8(%rbp), %eax
    cmpl -24(%rbp), %eax
    setg %al
    movzbl %al, %eax
    movl %eax, -80(%rbp)
    subq $48, %rsp
    movq -72(%rbp), %rcx
    movl -80(%rbp), %edx
    call printf
    addq $48, %rsp
    movl %eax, -72(%rbp)
    lea .LC7(%rip), %rax
    movq %rax, -72(%rbp)
    movl -8(%rbp), %eax
    cmpl -24(%rbp), %eax
    setge %al
    movzbl %al, %eax
    movl %eax, -80(%rbp)
    subq $48, %rsp
    movq -72(%rbp), %rcx
    movl -80(%rbp), %edx
    call printf
    addq $48, %rsp
    movl %eax, -72(%rbp)
    lea .LC8(%rip), %rax
    movq %rax, -72(%rbp)
    movl -8(%rbp), %eax
    cmpl -24(%rbp), %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -80(%rbp)
    subq $48, %rsp
    movq -72(%rbp), %rcx
    movl -80(%rbp), %edx
    call printf
    addq $48, %rsp
    movl %eax, -72(%rbp)
    lea .LC9(%rip), %rax
    movq %rax, -72(%rbp)
    movl -8(%rbp), %eax
    cmpl -24(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -80(%rbp)
    subq $48, %rsp
    movq -72(%rbp), %rcx
    movl -80(%rbp), %edx
    call printf
    addq $48, %rsp
    movl %eax, -72(%rbp)
    lea .LC10(%rip), %rax
    movq %rax, -72(%rbp)
    movl -40(%rbp), %eax
    cmpl -56(%rbp), %eax
    setb %al
    movzbl %al, %eax
    movl %eax, -80(%rbp)
    subq $48, %rsp
    movq -72(%rbp), %rcx
    movl -80(%rbp), %edx
    call printf
    addq $48, %rsp
    movl %eax, -72(%rbp)
    lea .LC11(%rip), %rax
    movq %rax, -72(%rbp)
    movl -40(%rbp), %eax
    cmpl -56(%rbp), %eax
    setbe %al
    movzbl %al, %eax
    movl %eax, -80(%rbp)
    subq $48, %rsp
    movq -72(%rbp), %rcx
    movl -80(%rbp), %edx
    call printf
    addq $48, %rsp
    movl %eax, -72(%rbp)
    lea .LC12(%rip), %rax
    movq %rax, -72(%rbp)
    movl -40(%rbp), %eax
    cmpl -56(%rbp), %eax
    seta %al
    movzbl %al, %eax
    movl %eax, -80(%rbp)
    subq $48, %rsp
    movq -72(%rbp), %rcx
    movl -80(%rbp), %edx
    call printf
    addq $48, %rsp
    movl %eax, -72(%rbp)
    lea .LC13(%rip), %rax
    movq %rax, -72(%rbp)
    movl -40(%rbp), %eax
    cmpl -56(%rbp), %eax
    setae %al
    movzbl %al, %eax
    movl %eax, -80(%rbp)
    subq $48, %rsp
    movq -72(%rbp), %rcx
    movl -80(%rbp), %edx
    call printf
    addq $48, %rsp
    movl %eax, -72(%rbp)
    lea .LC14(%rip), %rax
    movq %rax, -72(%rbp)
    movl -40(%rbp), %eax
    cmpl -56(%rbp), %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -80(%rbp)
    subq $48, %rsp
    movq -72(%rbp), %rcx
    movl -80(%rbp), %edx
    call printf
    addq $48, %rsp
    movl %eax, -72(%rbp)
    lea .LC15(%rip), %rax
    movq %rax, -72(%rbp)
    movl -40(%rbp), %eax
    cmpl -56(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -80(%rbp)
    subq $48, %rsp
    movq -72(%rbp), %rcx
    movl -80(%rbp), %edx
    call printf
    addq $48, %rsp
    movl %eax, -72(%rbp)
    lea .LC16(%rip), %rax
    movq %rax, -72(%rbp)
    movl -8(%rbp), %eax
    imull -24(%rbp)
    movl %eax, -80(%rbp)
    subq $48, %rsp
    movq -72(%rbp), %rcx
    movl -80(%rbp), %edx
    call printf
    addq $48, %rsp
    movl %eax, -72(%rbp)
    lea .LC17(%rip), %rax
    movq %rax, -72(%rbp)
    movl -40(%rbp), %eax
    mull -56(%rbp)
    movl %eax, -80(%rbp)
    subq $48, %rsp
    movq -72(%rbp), %rcx
    movl -80(%rbp), %edx
    call printf
    addq $48, %rsp
    movl %eax, -72(%rbp)
    lea .LC18(%rip), %rax
    movq %rax, -72(%rbp)
    movl -8(%rbp), %eax
    cltd
    idivl -24(%rbp)
    movl %eax, -80(%rbp)
    subq $48, %rsp
    movq -72(%rbp), %rcx
    movl -80(%rbp), %edx
    call printf
    addq $48, %rsp
    movl %eax, -72(%rbp)
    lea .LC19(%rip), %rax
    movq %rax, -72(%rbp)
    movl -40(%rbp), %eax
    cltd
    divl -56(%rbp)
    movl %eax, -80(%rbp)
    subq $48, %rsp
    movq -72(%rbp), %rcx
    movl -80(%rbp), %edx
    call printf
    addq $48, %rsp
    movl %eax, -72(%rbp)
    lea .LC20(%rip), %rax
    movq %rax, -72(%rbp)
    movl -8(%rbp), %eax
    cltd
    idivl -24(%rbp)
    movl %edx, -80(%rbp)
    subq $48, %rsp
    movq -72(%rbp), %rcx
    movl -80(%rbp), %edx
    call printf
    addq $48, %rsp
    movl %eax, -72(%rbp)
    lea .LC21(%rip), %rax
    movq %rax, -72(%rbp)
    movl -40(%rbp), %eax
    cltd
    divl -56(%rbp)
    movl %edx, -80(%rbp)
    subq $48, %rsp
    movq -72(%rbp), %rcx
    movl -80(%rbp), %edx
    call printf
    addq $48, %rsp
    movl %eax, -72(%rbp)
    movl $0, %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
