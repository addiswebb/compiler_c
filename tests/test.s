.section .note.GNU-stack,"",@progbits
.section .rodata
.LC0:
    .byte 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', '1', '2', '3', '1', '2', '4', '5', '3', '1', 0

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
main_0:
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    leaq .LC0(%rip), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rsi
    movq -32(%rbp), %rdi
    mov $19, %rdx
    call memcpy
    movl $19, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
