.section .rodata
.LC0:
    .byte 'H', 'e', 'l', 'l', 'o', 0
.LC3:
    .byte 'W', 'o', 'r', 'l', 'd', 0
.LC4:
    .byte 'P', 'o', 'i', 'n', 't', ' ', '[', '%', 's', ' ', '%', 'd', ' ', '%', 'd', ' ', '%', 's', ']', 0x0A, 0

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $112, %rsp
main_0:
    lea .LC0(%rip), %rax
    movq %rax, -72(%rbp)
    movq -72(%rbp), %rax
    movq %rax, -32(%rbp)
    movl $2, %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    movl %eax, -24(%rbp)
    movl $3, %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    movl %eax, -16(%rbp)
    lea .LC3(%rip), %rax
    movq %rax, -72(%rbp)
    movq -72(%rbp), %rax
    movq %rax, -8(%rbp)
    lea .LC4(%rip), %rax
    movq %rax, -72(%rbp)
    lea -32(%rbp), %rax
    movq %rax, -80(%rbp)
    movq -80(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -80(%rbp)
    lea -32(%rbp), %rax
    movq %rax, -88(%rbp)
    movl $8, %eax
    movq %rax, -96(%rbp)
    movq -88(%rbp), %rax
    addq -96(%rbp), %rax
    movq %rax, -88(%rbp)
    movq -88(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -88(%rbp)
    lea -32(%rbp), %rax
    movq %rax, -96(%rbp)
    movl $16, %eax
    movq %rax, -104(%rbp)
    movq -96(%rbp), %rax
    addq -104(%rbp), %rax
    movq %rax, -96(%rbp)
    movq -96(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -96(%rbp)
    lea -32(%rbp), %rax
    movq %rax, -104(%rbp)
    movl $24, %eax
    movq %rax, -112(%rbp)
    movq -104(%rbp), %rax
    addq -112(%rbp), %rax
    movq %rax, -104(%rbp)
    movq -104(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -104(%rbp)
    subq $48, %rsp
    movq -72(%rbp), %rcx
    movq -80(%rbp), %rdx
    movl -88(%rbp), %r8d
    movl -96(%rbp), %r9d
    movq -104(%rbp), %rax
    push %rax
    call printf
    addq $48, %rsp
    movl %eax, -72(%rbp)
    movl $0, %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
