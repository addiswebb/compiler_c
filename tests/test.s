.section .note.GNU-stack,"",@progbits

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $0, %rsp
main_0:
    movl $0, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
