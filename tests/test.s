.section .note.GNU-stack,"",@progbits
.global a
.data
.align 4
a:
    .long 20
.global b
.data
.align 4
b:
    .long 1
    .long 0
    .long 0
    .long 13
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
main_0:
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    movq -32(%rbp), %rax
    addq $4, %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    movq -32(%rbp), %rax
    addq $8, %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    movq -32(%rbp), %rax
    addq $12, %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl $21, %ecx
    movl %ecx, (%rax)
    movq -32(%rbp), %rax
    addq $16, %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    leaq a(%rip), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    cmpl $20, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    testl %eax, %eax
    jz main_1
    jmp main_2
main_1:
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    movl $2, %eax
    imull $2, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    subl $1, %eax
    movl %eax, -40(%rbp)
    movslq -40(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    imulq $4, %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    addq -40(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -32(%rbp)
    leaq a(%rip), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    movl -32(%rbp), %eax
    addl -40(%rbp), %eax
    movl %eax, -32(%rbp)
    leaq b(%rip), %rax
    movq %rax, -40(%rbp)
    movl $7, %eax
    cltd
    movl $2, %ecx
    idivl %ecx
    movl %eax, -48(%rbp)
    movslq -48(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    imulq $4, %rax
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    addq -48(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    movl -32(%rbp), %eax
    addl -40(%rbp), %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
main_2:
    movl $1, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
main_3:
