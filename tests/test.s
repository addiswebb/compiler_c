.section .note.GNU-stack,"",@progbits
.section .rodata
.align 8
.LC0:
    .byte 'h', 'e', 'l', 'l', 'o', 0
.align 8
.LC1:
    .byte 'w', 'o', 'r', 'l', 'd', 0
.LC2:
    .byte '%', 's', ' ', '%', 's', ' ', '%', 'd', 0x0A, 0
.global strs
.data
.align 8
strs:
    .quad .LC0
    .quad .LC1
.global a
.data
.align 4
a:
    .long 10
.global c
.data
.align 4
c:
    .long 15
.global b
.data
.align 8
b:
    .quad a + 4

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
main_0:
    leaq .LC2(%rip), %rax
    movq %rax, -8(%rbp)
    leaq strs(%rip), %rax
    movq %rax, -16(%rbp)
    movq $0, %rax
    imulq $8, %rax
    movq %rax, -24(%rbp)
    movq -16(%rbp), %rax
    addq -24(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -16(%rbp)
    leaq strs(%rip), %rax
    movq %rax, -24(%rbp)
    movq $1, %rax
    imulq $8, %rax
    movq %rax, -32(%rbp)
    movq -24(%rbp), %rax
    addq -32(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -24(%rbp)
    leaq b(%rip), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -32(%rbp)
    subq $176, %rsp
    movq -8(%rbp), %rdi
    movq -16(%rbp), %rsi
    movq -24(%rbp), %rdx
    movl -32(%rbp), %ecx
    xor %eax, %eax
    call printf
    addq $176, %rsp
    movl %eax, -8(%rbp)
    movl $0, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
