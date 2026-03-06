.section .rodata
.LC2:
    .byte 'P', 'o', 'i', 'n', 't', ' ', '[', '%', 'd', ',', ' ', '%', 'd', ']', 0x0A, 0

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $64, %rsp
main_0:
    movl $1, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $2, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -4(%rbp)
    lea -8(%rbp), %rax
    movq %rax, -40(%rbp)
    lea -24(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rdx
    movq -48(%rbp), %rcx
    mov $8, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    lea .LC2(%rip), %rax
    movq %rax, -40(%rbp)
    lea -24(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -48(%rbp)
    lea -24(%rbp), %rax
    movq %rax, -56(%rbp)
    movl $4, %eax
    movq %rax, -64(%rbp)
    movq -56(%rbp), %rax
    addq -64(%rbp), %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -56(%rbp)
    subq $48, %rsp
    movq -40(%rbp), %rcx
    movl -48(%rbp), %edx
    movl -56(%rbp), %r8d
    call printf
    addq $48, %rsp
    movl %eax, -40(%rbp)
    movl $0, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
