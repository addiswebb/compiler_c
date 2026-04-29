.section .note.GNU-stack,"",@progbits

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp
main_0:
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    movl $3, %eax
    cmpl $1, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    testl %eax, %eax
    jz main_1
    movl $3, %eax
    cmpl $2, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    testl %eax, %eax
    jz main_2
    jmp main_3
main_1:
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movl $1, %ecx
    movl %ecx, (%rax)
    jmp main_3
main_2:
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movl $2, %ecx
    movl %ecx, (%rax)
    jmp main_3
main_3:
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
