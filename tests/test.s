.section .rodata
.LC0:
    .byte 'H', 'e', 'l', 'l', 'o', ' ', '%', 'd', 0x0A, 0

.text
.global foo
foo:
    push %rbp
    mov %rsp, %rbp
    subq $64, %rsp
foo_0:
    movq %rdi, %rax
    movq %rax, -16(%rbp)
    movq %rsi, %rax
    movq %rax, -8(%rbp)
    lea .LC0(%rip), %rax
    movq %rax, -40(%rbp)
    lea -16(%rbp), %rax
    movq %rax, -48(%rbp)
    movl $4, %eax
    movq %rax, -56(%rbp)
    movq -48(%rbp), %rax
    addq -56(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -48(%rbp)
    subq $32, %rsp
    movq -40(%rbp), %rdi
    movl -48(%rbp), %esi
    mov $0, %al
    call printf
    addq $32, %rsp
    movl %eax, -40(%rbp)
    lea -16(%rbp), %rax
    movq %rax, -40(%rbp)
    movl $4, %eax
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    addq -48(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
main_0:
    movl $5, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -16(%rbp)
    movl $98, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -12(%rbp)
    movl $97, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -8(%rbp)
    subq $32, %rsp
    movq -16(%rbp), %rdi
    movq -8(%rbp), %rsi
    call foo
    addq $32, %rsp
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
