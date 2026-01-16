.section .rodata
.LC2:
    .string "hello world"
.section .text

.global main
test:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
test_0:
    movq 16(%rbp), %rax
    movq %rax, -8(%rbp)
    movl $3, %eax
    movl %eax, -16(%rbp)
    movl $1, %eax
    movl %eax, -24(%rbp)
    movq -8(%rbp), %rax
    addq -16(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movb (%rax), %al
    movb %al, -40(%rbp)
    movzbl -40(%rbp), %eax
    movl %eax, -48(%rbp)
    movl -48(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
main:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
main_0:
    lea .LC2(%rip), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movq %rax, -8(%rbp)
    movq -8(%rbp), %rax
    push %rax
    call test
    add $8, %rsp
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
