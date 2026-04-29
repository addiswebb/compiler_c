.section .note.GNU-stack,"",@progbits
.section .rodata
.LC0:
    .byte 'y', 0x0A, 0

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp
main_0:
main_1:
main_2:
    leaq .LC0(%rip), %rax
    movq %rax, -8(%rbp)
    subq $176, %rsp
    movq -8(%rbp), %rdi
    xor %eax, %eax
    call printf
    addq $176, %rsp
    movl %eax, -8(%rbp)
    jmp main_3
main_3:
    jmp main_1
main_4:
    movl $0, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
