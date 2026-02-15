.section .rodata

.text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
main_0:
    lea -8(%rbp), %rax
    movq %rax, -40(%rbp)
    movl $4, %eax
    movl %eax, -48(%rbp)
    movq -40(%rbp), %rax
    movl -48(%rbp), %ebx
    movl %ebx, (%rax)
    lea -4(%rbp), %rax
    movq %rax, -40(%rbp)
    movl $10, %eax
    movl %eax, -48(%rbp)
    movq -40(%rbp), %rax
    movl -48(%rbp), %ebx
    movl %ebx, (%rax)
    lea -24(%rbp), %rax
    movq %rax, -40(%rbp)
    lea -8(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rdx
    movq -40(%rbp), %rcx
    mov $8, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    lea -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    lea -20(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -48(%rbp)
    movl -40(%rbp), %eax
    addl -48(%rbp), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
