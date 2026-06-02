.section .rodata
.LC0:
    .byte '%', 'd', ' ', '%', 'd', ' ', '%', 'd', ' ', '%', 'd', ' ', '%', 'd', ' ', '%', 'd', ' ', '%', 'd', ' ', '%', 'd', ' ', '%', 'd', 0x0A, 0

.text
.global print
print:
    push %rbp
    mov %rsp, %rbp
    subq $208, %rsp
print_0:
    movq %rcx, -184(%rbp)
    movq %rdx, 24(%rbp)
    movq %r8, 32(%rbp)
    movq %r9, 40(%rbp)
    leaq -192(%rbp), %rax
    movq %rax, -200(%rbp)
    leaq 24(%rbp), %rax
    movq %rax, -208(%rbp)
    movq -200(%rbp), %rax
    movq -208(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -184(%rbp), %rax
    movq %rax, -200(%rbp)
    movq -200(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -200(%rbp)
    leaq -192(%rbp), %rax
    movq %rax, -208(%rbp)
    movq -208(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -208(%rbp)
    subq $32, %rsp
    movq -200(%rbp), %rcx
    movq -208(%rbp), %rdx
    call vprint
    addq $32, %rsp
    mov %rbp, %rsp
    pop %rbp
    ret
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp
main_0:
    leaq .LC0(%rip), %rax
    movq %rax, -8(%rbp)
    subq $80, %rsp
    movq -8(%rbp), %rcx
    movl $1, %edx
    movl $2, %r8d
    movl $3, %r9d
    movl $4, %eax
    movl %eax, 32(%rsp)
    movl $5, %eax
    movl %eax, 40(%rsp)
    movl $6, %eax
    movl %eax, 48(%rsp)
    movl $7, %eax
    movl %eax, 56(%rsp)
    movl $8, %eax
    movl %eax, 64(%rsp)
    movl $9, %eax
    movl %eax, 72(%rsp)
    call print
    addq $80, %rsp
    movl $0, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
