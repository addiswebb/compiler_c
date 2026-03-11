.section .rodata
.LC0:
    .byte 'b', 'u', 'z', 'z', 0x0A, 0
.LC1:
    .byte 'f', 'o', 'o', ' ', 0

.text
.global buzz
buzz:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp
buzz_0:
    lea .LC0(%rip), %rax
    movq %rax, -8(%rbp)
    subq $48, %rsp
    movq -8(%rbp), %rcx
    call printf
    addq $48, %rsp
    movl %eax, -8(%rbp)
    mov %rbp, %rsp
    pop %rbp
    ret
.global foo
foo:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp
foo_0:
    lea .LC1(%rip), %rax
    movq %rax, -8(%rbp)
    subq $48, %rsp
    movq -8(%rbp), %rcx
    call printf
    addq $48, %rsp
    movl %eax, -8(%rbp)
    subq $48, %rsp
    call buzz
    addq $48, %rsp
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
    mov %rbp, %rsp
    pop %rbp
    ret
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp
main_0:
    subq $48, %rsp
    call foo
    addq $48, %rsp
    movl $0, %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
