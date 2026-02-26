.section .rodata
.LC0:
    .byte 'f', 'a', 'a', 0x0A, 0

.text
.global faa
faa:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp
faa_0:
    lea .LC0(%rip), %rax
    movq %rax, -8(%rbp)
    subq $48, %rsp
    movq -8(%rbp), %rcx
    call printf
    addq $48, %rsp
    movl %eax, -8(%rbp)
