.section .rodata
.section .text

.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $176, %rsp
main_0:
    movl $1, %eax
    movl %eax, -168(%rbp)
    movl $2, %eax
    movl %eax, -176(%rbp)
    movl -168(%rbp), %eax
    addl -176(%rbp), %eax
    movl %eax, -168(%rbp)
    movl -168(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $5, %eax
    movl %eax, -168(%rbp)
    movl $3, %eax
    movl %eax, -176(%rbp)
    movl -168(%rbp), %eax
    subl -176(%rbp), %eax
    movl %eax, -168(%rbp)
    movl -168(%rbp), %eax
    movl %eax, -24(%rbp)
    movl $4, %eax
    movl %eax, -168(%rbp)
    movl $3, %eax
    movl %eax, -176(%rbp)
    movl -168(%rbp), %eax
    imull -176(%rbp)
    movl %eax, -168(%rbp)
    movl -168(%rbp), %eax
    movl %eax, -40(%rbp)
    movl $10, %eax
    movl %eax, -168(%rbp)
    movl $2, %eax
    movl %eax, -176(%rbp)
    movl -168(%rbp), %eax
    cltd
    idivl -176(%rbp)
    movl %eax, -168(%rbp)
    movl -168(%rbp), %eax
    movl %eax, -56(%rbp)
    movl $10, %eax
    movl %eax, -168(%rbp)
    movl $3, %eax
    movl %eax, -176(%rbp)
    movl -168(%rbp), %eax
    cltd
    idivl -176(%rbp)
    movl %edx, -168(%rbp)
    movl -168(%rbp), %eax
    movl %eax, -72(%rbp)
    movl $6, %eax
    movl %eax, -168(%rbp)
    movl $3, %eax
    movl %eax, -176(%rbp)
    movl -168(%rbp), %eax
    andl -176(%rbp), %eax
    movl %eax, -168(%rbp)
    movl -168(%rbp), %eax
    movl %eax, -88(%rbp)
    movl $6, %eax
    movl %eax, -168(%rbp)
    movl $3, %eax
    movl %eax, -176(%rbp)
    movl -168(%rbp), %eax
    orl -176(%rbp), %eax
    movl %eax, -168(%rbp)
    movl -168(%rbp), %eax
    movl %eax, -104(%rbp)
    movl $6, %eax
    movl %eax, -168(%rbp)
    movl $3, %eax
    movl %eax, -176(%rbp)
    movl -168(%rbp), %eax
    xorl -176(%rbp), %eax
    movl %eax, -168(%rbp)
    movl -168(%rbp), %eax
    movl %eax, -120(%rbp)
    movl $3, %eax
    movl %eax, -168(%rbp)
    movl $2, %eax
    movl %eax, -176(%rbp)
    movl -168(%rbp), %eax
    movl -176(%rbp), %ecx
    shll %cl, %eax
    movl %eax, -168(%rbp)
    movl -168(%rbp), %eax
    movl %eax, -136(%rbp)
    movl $16, %eax
    movl %eax, -168(%rbp)
    movl $2, %eax
    movl %eax, -176(%rbp)
    movl -168(%rbp), %eax
    movl -176(%rbp), %ecx
    sarl %cl, %eax
    movl %eax, -168(%rbp)
    movl -168(%rbp), %eax
    movl %eax, -152(%rbp)
    movl -8(%rbp), %eax
    addl -24(%rbp), %eax
    movl %eax, -168(%rbp)
    movl -168(%rbp), %eax
    addl -40(%rbp), %eax
    movl %eax, -168(%rbp)
    movl -168(%rbp), %eax
    addl -56(%rbp), %eax
    movl %eax, -168(%rbp)
    movl -168(%rbp), %eax
    addl -72(%rbp), %eax
    movl %eax, -168(%rbp)
    movl -168(%rbp), %eax
    addl -88(%rbp), %eax
    movl %eax, -168(%rbp)
    movl -168(%rbp), %eax
    addl -104(%rbp), %eax
    movl %eax, -168(%rbp)
    movl -168(%rbp), %eax
    addl -120(%rbp), %eax
    movl %eax, -168(%rbp)
    movl -168(%rbp), %eax
    addl -136(%rbp), %eax
    movl %eax, -168(%rbp)
    movl -168(%rbp), %eax
    addl -152(%rbp), %eax
    movl %eax, -168(%rbp)
    movl -168(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
