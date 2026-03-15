.section .note.GNU-stack,"",@progbits
.section .rodata
.LC3:
    .byte '%', 'c', ' ', '%', 'c', ' ', '%', 'c', 0x0A, 0

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $64, %rsp
main_0:
    movl $98, %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    movb %al, -24(%rbp)
    movb -24(%rbp), %al
    movb %al, -8(%rbp)
    movl $97, %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    movb %al, -24(%rbp)
    movb -24(%rbp), %al
    movb %al, -7(%rbp)
    movl $100, %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    movb %al, -24(%rbp)
    movb -24(%rbp), %al
    movb %al, -6(%rbp)
    lea .LC3(%rip), %rax
    movq %rax, -24(%rbp)
    lea -8(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movb (%rax), %al
    movb %al, -32(%rbp)
    movsbl -32(%rbp), %eax
    movl %eax, -32(%rbp)
    lea -8(%rbp), %rax
    movq %rax, -40(%rbp)
    movl $1, %eax
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
    movl $2, %eax
    movq %rax, -56(%rbp)
    movq -48(%rbp), %rax
    addq -56(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movb (%rax), %al
    movb %al, -48(%rbp)
    movsbl -48(%rbp), %eax
    movl %eax, -48(%rbp)
    movq -24(%rbp), %rdi
    movl -32(%rbp), %esi
    movl -40(%rbp), %edx
    movl -48(%rbp), %ecx
    xor %eax, %eax
    call printf
    addq $0, %rsp
    movl %eax, -24(%rbp)
    movq -8(%rbp), %rdi
    call foo
    addq $0, %rsp
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
