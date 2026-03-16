.section .rodata
.LC12:
    .byte '%', 'd', ' ', 0
.LC15:
    .byte 0x0A, 0

.text
.global cmp_int
cmp_int:
    push %rbp
    mov %rsp, %rbp
    subq $80, %rsp
cmp_int_0:
    movq %rcx, %rax
    movq %rax, -8(%rbp)
    movq %rdx, %rax
    movq %rax, -24(%rbp)
    movq -8(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    movl %eax, -40(%rbp)
    movq -24(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    movl %eax, -56(%rbp)
    movl -40(%rbp), %eax
    cmpl -56(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    testl %eax, %eax
    jz cmp_int_2
cmp_int_1:
    movl $-1, %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp cmp_int_2
cmp_int_2:
    movl -40(%rbp), %eax
    cmpl -56(%rbp), %eax
    setg %al
    movzbl %al, %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    testl %eax, %eax
    jz cmp_int_4
cmp_int_3:
    movl $1, %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp cmp_int_4
cmp_int_4:
    movl $0, %eax
    movl %eax, -72(%rbp)
    movl -72(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $128, %rsp
main_0:
    movl $5, %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    movl %eax, -24(%rbp)
    movl $1, %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    movl %eax, -20(%rbp)
    movl $4, %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    movl %eax, -16(%rbp)
    movl $2, %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    movl %eax, -12(%rbp)
    movl $3, %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $5, %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    movl %eax, -56(%rbp)
    lea -24(%rbp), %rax
    movq %rax, -104(%rbp)
    movslq -56(%rbp), %rax
    movq %rax, -112(%rbp)
    movl $4, %eax
    movl %eax, -120(%rbp)
    movslq -120(%rbp), %rax
    movq %rax, -120(%rbp)
    leaq cmp_int(%rip), %rax
    movq %rax, -128(%rbp)
    subq $48, %rsp
    movq -104(%rbp), %rcx
    movq -112(%rbp), %rdx
    movq -120(%rbp), %r8
    movq -128(%rbp), %r9
    call qsort
    addq $48, %rsp
    movl $0, %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    movl %eax, -88(%rbp)
main_1:
    movl -88(%rbp), %eax
    cmpl -56(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    testl %eax, %eax
    jz main_4
main_2:
    lea .LC12(%rip), %rax
    movq %rax, -104(%rbp)
    lea -24(%rbp), %rax
    movq %rax, -112(%rbp)
    movslq -88(%rbp), %rax
    movq %rax, -120(%rbp)
    movl $4, %eax
    movq %rax, -128(%rbp)
    movq -120(%rbp), %rax
    imulq -128(%rbp)
    movq %rax, -120(%rbp)
    movq -112(%rbp), %rax
    addq -120(%rbp), %rax
    movq %rax, -112(%rbp)
    movq -112(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -112(%rbp)
    subq $48, %rsp
    movq -104(%rbp), %rcx
    movl -112(%rbp), %edx
    call printf
    addq $48, %rsp
    movl %eax, -104(%rbp)
    jmp main_3
main_3:
    movl $1, %eax
    movl %eax, -104(%rbp)
    movl -88(%rbp), %eax
    movl %eax, -112(%rbp)
    movl -88(%rbp), %eax
    addl -104(%rbp), %eax
    movl %eax, -88(%rbp)
    jmp main_1
main_4:
    lea .LC15(%rip), %rax
    movq %rax, -104(%rbp)
    subq $48, %rsp
    movq -104(%rbp), %rcx
    call printf
    addq $48, %rsp
    movl %eax, -104(%rbp)
    movl $0, %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
