.section .rodata

.text
.global foo
foo:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
foo_0:
    lea -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq %rcx, %rdx
    movq -24(%rbp), %rcx
    mov $3, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    lea -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movl $1, %eax
    movq %rax, -32(%rbp)
    movq -24(%rbp), %rax
    addq -32(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movb (%rax), %al
    movb %al, -24(%rbp)
    movsbl -24(%rbp), %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
main_0:
    movl $5, %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    movb %al, -24(%rbp)
    movb -24(%rbp), %al
    movb %al, -8(%rbp)
    movl $98, %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    movb %al, -24(%rbp)
    movb -24(%rbp), %al
    movb %al, -7(%rbp)
    movl $97, %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    movb %al, -24(%rbp)
    movb -24(%rbp), %al
    movb %al, -6(%rbp)
    lea -8(%rbp), %rax
    movq %rax, -24(%rbp)
    subq $48, %rsp
    movq -24(%rbp), %rcx
    call foo
    addq $48, %rsp
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
