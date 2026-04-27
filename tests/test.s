.section .note.GNU-stack,"",@progbits

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
main_0:
    movl $1, %eax
    subl $2, %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    cmpl $0, %eax
    setg %al
    movzbl %al, %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    testl %eax, %eax
    jnz main_1
    movl $3, %eax
    imull $4, %eax
    movl %eax, -8(%rbp)
    movq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    jmp main_4
main_1:
    movl $5, %eax
    addl $6, %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    testl %eax, %eax
    jnz main_2
    movl $7, %eax
    subl $1, %eax
    movl %eax, -8(%rbp)
    movq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    jmp main_3
main_2:
    movq $8, %rax
    movq %rax, -24(%rbp)
main_3:
    movq -24(%rbp), %rax
    movq %rax, -16(%rbp)
main_4:
    movl -16(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
