.section .rodata

.text
.global foo
foo:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
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
    movq -24(%rbp), %rax
    movb (%rax), %al
    movb %al, -24(%rbp)
    movsbl -24(%rbp), %eax
    movl %eax, -24(%rbp)
    lea -8(%rbp), %rax
    movq %rax, -32(%rbp)
    movl $1, %eax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    addq -40(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movb (%rax), %al
    movb %al, -32(%rbp)
    movsbl -32(%rbp), %eax
    movl %eax, -32(%rbp)
    movl -24(%rbp), %eax
    addl -32(%rbp), %eax
    movl %eax, -24(%rbp)
    lea -8(%rbp), %rax
    movq %rax, -32(%rbp)
    movl $2, %eax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    addq -40(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movb (%rax), %al
    movb %al, -32(%rbp)
    movsbl -32(%rbp), %eax
    movl %eax, -32(%rbp)
    movl -24(%rbp), %eax
    addl -32(%rbp), %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
