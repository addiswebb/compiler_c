.section .note.GNU-stack,"",@progbits
.section .rodata
.align 8
.LC0:
    .byte 'h', 'e', 'l', 'l', 'o', 0
    .byte 'w', 'o', 'r', 'l', 'd', 0
.LC1:
    .byte '%', 'c', 0x0A, 0
.global strs
.data
.align 8
strs:
    .quad .LC0

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
main_0:
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
main_1:
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    cmpl $12, %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    testl %eax, %eax
    jz main_4
main_2:
    leaq .LC1(%rip), %rax
    movq %rax, -16(%rbp)
    leaq strs(%rip), %rax
    movq %rax, -24(%rbp)
    movq $0, %rax
    imulq $8, %rax
    movq %rax, -32(%rbp)
    movq -24(%rbp), %rax
    addq -32(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -24(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -32(%rbp)
    movslq -32(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    imulq $1, %rax
    movq %rax, -32(%rbp)
    movq -24(%rbp), %rax
    addq -32(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movb (%rax), %al
    movb %al, -24(%rbp)
    movsbl -24(%rbp), %eax
    movl %eax, -24(%rbp)
    subq $176, %rsp
    movq -16(%rbp), %rdi
    movl -24(%rbp), %esi
    xor %eax, %eax
    call printf
    addq $176, %rsp
    movl %eax, -16(%rbp)
    jmp main_3
main_3:
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    addl $1, %eax
    movl %eax, -24(%rbp)
    movq -16(%rbp), %rax
    movl -24(%rbp), %ecx
    movl %ecx, (%rax)
    jmp main_1
main_4:
    movl $0, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
