.section .rodata
.section .text

.global main
mix:
    push %rbp
    mov %rsp, %rbp
    subq $96, %rsp
mix_0:
    movl 40(%rbp), %eax
    movl %eax, -8(%rbp)
    movl 32(%rbp), %eax
    movl %eax, -24(%rbp)
    movq 24(%rbp), %rax
    movq %rax, -40(%rbp)
    movb 16(%rbp), %al
    movb %al, -56(%rbp)
    movl -8(%rbp), %eax
    addl -24(%rbp), %eax
    movl %eax, -88(%rbp)
    movl -88(%rbp), %eax
    movl %eax, -72(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -88(%rbp)
    movl -72(%rbp), %eax
    addl -88(%rbp), %eax
    movl %eax, -88(%rbp)
    movl -88(%rbp), %eax
    movl %eax, -72(%rbp)
    movsbl -56(%rbp), %eax
    movl %eax, -88(%rbp)
    movl -72(%rbp), %eax
    xorl -88(%rbp), %eax
    movl %eax, -88(%rbp)
    movl -88(%rbp), %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
main:
    push %rbp
    mov %rsp, %rbp
    subq $64, %rsp
main_0:
    movl $10, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $10, %eax
    movl %eax, -40(%rbp)
    movl $10, %eax
    movl %eax, -48(%rbp)
    lea -8(%rbp), %rax
    movq %rax, -56(%rbp)
    movl $10, %eax
    movl %eax, -64(%rbp)
    movl -64(%rbp), %eax
    movb %al, -64(%rbp)
    movl -40(%rbp), %eax
    push %rax
    movl -48(%rbp), %eax
    push %rax
    movq -56(%rbp), %rax
    push %rax
    movb -64(%rbp), %al
    push %rax
    call mix
    add $32, %rsp
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
