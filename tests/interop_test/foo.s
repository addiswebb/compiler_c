
.text

.global test
test:
    push %rbp
    mov %rsp, %rbp
    subq $112, %rsp
test_0:
    movl 56(%rbp), %eax
    movl %eax, -8(%rbp)
    movl 48(%rbp), %eax
    movl %eax, -24(%rbp)
    movl %r9d, %eax
    movl %eax, -40(%rbp)
    movl %r8d, %eax
    movl %eax, -56(%rbp)
    movl %edx, %eax
    movl %eax, -72(%rbp)
    movl %ecx, %eax
    movl %eax, -88(%rbp)
    movl -8(%rbp), %eax
    addl -24(%rbp), %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    addl -40(%rbp), %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    addl -56(%rbp), %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    addl -72(%rbp), %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    addl -88(%rbp), %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
