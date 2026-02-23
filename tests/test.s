.section .rodata
.LC0:
    .byte ' ', 'W', 'o', 'r', 'd', 0x0A, 0
.LC2:
    .byte 'H', 'e', 'l', 'l', 'o', 0

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp
main_0:
    jmp a
b:
    lea .LC0(%rip), %rax
    movq %rax, -8(%rbp)
    subq $48, %rsp
    movq -8(%rbp), %rcx
    call printf
    addq $48, %rsp
    movl %eax, -8(%rbp)
    movl $0, %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
a:
    lea .LC2(%rip), %rax
    movq %rax, -8(%rbp)
    subq $48, %rsp
    movq -8(%rbp), %rcx
    call printf
    addq $48, %rsp
    movl %eax, -8(%rbp)
    jmp b
