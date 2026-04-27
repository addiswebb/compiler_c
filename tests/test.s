.section .note.GNU-stack,"",@progbits

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
main_0:
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    leaq -16(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
main_1:
    leaq -16(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    cmpl $5, %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    testl %eax, %eax
    jz main_4
main_2:
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    addl $1, %eax
    movl %eax, -32(%rbp)
    movq -24(%rbp), %rax
    movl -32(%rbp), %ecx
    movl %ecx, (%rax)
    jmp main_3
main_3:
    leaq -16(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    addl $1, %eax
    movl %eax, -32(%rbp)
    movq -24(%rbp), %rax
    movl -32(%rbp), %ecx
    movl %ecx, (%rax)
    jmp main_1
main_4:
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    testl %eax, %eax
    jz main_6
main_5:
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movl $8, %ecx
    movl %ecx, (%rax)
    jmp main_6
main_6:
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
