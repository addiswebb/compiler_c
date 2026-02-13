.section .rodata
.align 8
.LC4:
    .quad 0x4004000000000000
.align 8
.LC5:
    .quad 0x4008000000000000
.LC15:
    .string "abc"

.text

.global mix
mix:
    push %rbp
    mov %rsp, %rbp
    subq $96, %rsp
mix_0:
    movl %r9d, %eax
    movl %eax, -8(%rbp)
    movl %r8d, %eax
    movl %eax, -24(%rbp)
    movq %rdx, %rax
    movq %rax, -40(%rbp)
    movb %cl, %al
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
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $384, %rsp
main_0:
    movl $10, %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $16, %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -24(%rbp)
    movl $8, %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -40(%rbp)
    movl $10, %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -56(%rbp)
    movsd .LC4(%rip), %xmm0
    movsd %xmm0, -360(%rbp)
    movsd -360(%rbp), %xmm0
    cvtsd2ss %xmm0, %xmm1
    movss %xmm1, -360(%rbp)
    movss -360(%rbp), %xmm0
    movss %xmm0, -72(%rbp)
    movsd .LC5(%rip), %xmm0
    movsd %xmm0, -360(%rbp)
    movsd -360(%rbp), %xmm0
    movsd %xmm0, -88(%rbp)
    movl $65, %eax
    movb %al, -360(%rbp)
    movb -360(%rbp), %al
    movb %al, -104(%rbp)
    movl $4, %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movw %ax, -360(%rbp)
    movw -360(%rbp), %ax
    movw %ax, -120(%rbp)
    movl $100, %eax
    movl %eax, -360(%rbp)
    movslq -360(%rbp), %rax
    movq %rax, -360(%rbp)
    movq -360(%rbp), %rax
    movq %rax, -136(%rbp)
    movl $1, %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -168(%rbp)
    movl $2, %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -164(%rbp)
    movl $3, %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -160(%rbp)
    movl $4, %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -156(%rbp)
    movl $5, %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -152(%rbp)
    lea .LC15(%rip), %rax
    movq %rax, -360(%rbp)
    movq -360(%rbp), %rdx
    lea -200(%rbp), %rcx
    mov $4, %r8d
    call memcpy
    lea -168(%rbp), %rax
    movq %rax, -360(%rbp)
    movq -360(%rbp), %rax
    movq %rax, -216(%rbp)
    movl $2, %eax
    movl %eax, -360(%rbp)
    movslq -360(%rbp), %rax
    movq %rax, -360(%rbp)
    movl $4, %eax
    movq %rax, -368(%rbp)
    movq -360(%rbp), %rax
    imulq -368(%rbp)
    movq %rax, -360(%rbp)
    movq -216(%rbp), %rax
    addq -360(%rbp), %rax
    movq %rax, -360(%rbp)
    movl $5, %eax
    movl %eax, -368(%rbp)
    movq -360(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -376(%rbp)
    movl -376(%rbp), %eax
    addl -368(%rbp), %eax
    movl %eax, -368(%rbp)
    movq -360(%rbp), %rax
    movl -368(%rbp), %ebx
    movl %ebx, (%rax)
    movl -8(%rbp), %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -232(%rbp)
    movl -232(%rbp), %eax
    negl %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -232(%rbp)
    movl -232(%rbp), %eax
    testl %eax, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -232(%rbp)
    movl -232(%rbp), %eax
    notl %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -232(%rbp)
    movl $1, %eax
    movl %eax, -360(%rbp)
    movl -232(%rbp), %eax
    movl %eax, -368(%rbp)
    movl -232(%rbp), %eax
    addl -360(%rbp), %eax
    movl %eax, -232(%rbp)
    movl $20, %eax
    movl %eax, -360(%rbp)
    movl $4, %eax
    movl %eax, -368(%rbp)
    movl -360(%rbp), %eax
    cltd
    idivl -368(%rbp)
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -248(%rbp)
    movl $0, %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -264(%rbp)
    movl -8(%rbp), %eax
    addl -24(%rbp), %eax
    movl %eax, -360(%rbp)
    movl -264(%rbp), %eax
    addl -360(%rbp), %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -264(%rbp)
    movl -264(%rbp), %eax
    subl -40(%rbp), %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -264(%rbp)
    movl -264(%rbp), %eax
    imull -56(%rbp)
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -264(%rbp)
    movl $2, %eax
    movl %eax, -360(%rbp)
    movl -264(%rbp), %eax
    cltd
    idivl -360(%rbp)
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -264(%rbp)
    movl $255, %eax
    movl %eax, -360(%rbp)
    movl -264(%rbp), %eax
    andl -360(%rbp), %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -264(%rbp)
    movl $16, %eax
    movl %eax, -360(%rbp)
    movl -264(%rbp), %eax
    orl -360(%rbp), %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -264(%rbp)
    movl $3, %eax
    movl %eax, -360(%rbp)
    movl -264(%rbp), %eax
    xorl -360(%rbp), %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -264(%rbp)
    movl $1, %eax
    movl %eax, -360(%rbp)
    movl -264(%rbp), %eax
    movl -360(%rbp), %ecx
    shll %cl, %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -264(%rbp)
    movl $1, %eax
    movl %eax, -360(%rbp)
    movl -264(%rbp), %eax
    movl -360(%rbp), %ecx
    sarl %cl, %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -264(%rbp)
    movl -8(%rbp), %eax
    cmpl -24(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -360(%rbp)
    movl $0, %eax
    movl %eax, -368(%rbp)
    movl -360(%rbp), %eax
    cmpl -368(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    testl %eax, %eax
    jz main_2
    movl -40(%rbp), %eax
    cmpl -56(%rbp), %eax
    setle %al
    movzbl %al, %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    cmpl -368(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -360(%rbp)
    movl $0, %eax
    movl %eax, -368(%rbp)
    movl -360(%rbp), %eax
    cmpl -368(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    testl %eax, %eax
    movl -24(%rbp), %eax
    cmpl -56(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    cmpl -368(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    testl %eax, %eax
    jz main_2
main_1:
    movl $5, %eax
    movl %eax, -360(%rbp)
    movl -264(%rbp), %eax
    addl -360(%rbp), %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -264(%rbp)
    jmp main_3
main_2:
    movl $5, %eax
    movl %eax, -360(%rbp)
    movl -264(%rbp), %eax
    subl -360(%rbp), %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -264(%rbp)
    jmp main_3
main_3:
    movl $0, %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -280(%rbp)
main_4:
    movl $5, %eax
    movl %eax, -360(%rbp)
    movl -280(%rbp), %eax
    cmpl -360(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    testl %eax, %eax
    jz main_7
main_5:
    lea -168(%rbp), %rax
    movq %rax, -360(%rbp)
    movslq -280(%rbp), %rax
    movq %rax, -368(%rbp)
    movl $4, %eax
    movq %rax, -376(%rbp)
    movq -368(%rbp), %rax
    imulq -376(%rbp)
    movq %rax, -368(%rbp)
    movq -360(%rbp), %rax
    addq -368(%rbp), %rax
    movq %rax, -360(%rbp)
    movq -360(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -360(%rbp)
    movl -264(%rbp), %eax
    addl -360(%rbp), %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -264(%rbp)
    jmp main_6
main_6:
    movl $1, %eax
    movl %eax, -360(%rbp)
    movl -280(%rbp), %eax
    movl %eax, -368(%rbp)
    movl -280(%rbp), %eax
    addl -360(%rbp), %eax
    movl %eax, -280(%rbp)
    jmp main_4
main_7:
    movl $0, %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -296(%rbp)
main_8:
    movl $3, %eax
    movl %eax, -360(%rbp)
    movl -296(%rbp), %eax
    cmpl -360(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    testl %eax, %eax
    jz main_10
main_9:
    movl $2, %eax
    movl %eax, -360(%rbp)
    movl -296(%rbp), %eax
    imull -360(%rbp)
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -312(%rbp)
    movl -264(%rbp), %eax
    addl -312(%rbp), %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -264(%rbp)
    movl $1, %eax
    movl %eax, -360(%rbp)
    movl -296(%rbp), %eax
    movl %eax, -368(%rbp)
    movl -296(%rbp), %eax
    addl -360(%rbp), %eax
    movl %eax, -296(%rbp)
    jmp main_8
main_10:
    movss -72(%rbp), %xmm0
    cvtss2sd %xmm0, %xmm1
    movsd %xmm1, -360(%rbp)
    movsd -360(%rbp), %xmm0
    addsd -88(%rbp), %xmm0
    movsd %xmm0, -360(%rbp)
    cvttsd2si -360(%rbp), %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -328(%rbp)
    lea -168(%rbp), %rax
    movq %rax, -360(%rbp)
    movl $1, %eax
    movl %eax, -368(%rbp)
    movslq -368(%rbp), %rax
    movq %rax, -368(%rbp)
    movl $4, %eax
    movq %rax, -376(%rbp)
    movq -368(%rbp), %rax
    imulq -376(%rbp)
    movq %rax, -368(%rbp)
    movq -360(%rbp), %rax
    addq -368(%rbp), %rax
    movq %rax, -360(%rbp)
    subq $48, %rsp
    movl -264(%rbp), %ecx
    movl -328(%rbp), %edx
    movq -360(%rbp), %r8
    movb -104(%rbp), %r9b
    call mix
    addq $48, %rsp
    movl %eax, -360(%rbp)
    movl -264(%rbp), %eax
    addl -360(%rbp), %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -264(%rbp)
    lea -200(%rbp), %rax
    movq %rax, -360(%rbp)
    movl $0, %eax
    movl %eax, -368(%rbp)
    movslq -368(%rbp), %rax
    movq %rax, -368(%rbp)
    movl $1, %eax
    movq %rax, -376(%rbp)
    movq -368(%rbp), %rax
    imulq -376(%rbp)
    movq %rax, -368(%rbp)
    movq -360(%rbp), %rax
    addq -368(%rbp), %rax
    movq %rax, -360(%rbp)
    movq -360(%rbp), %rax
    movb (%rax), %al
    movb %al, -360(%rbp)
    movsbl -360(%rbp), %eax
    movl %eax, -360(%rbp)
    movl -264(%rbp), %eax
    addl -360(%rbp), %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -264(%rbp)
    movl -264(%rbp), %eax
    addl -248(%rbp), %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -264(%rbp)
    movswl -120(%rbp), %eax
    movl %eax, -360(%rbp)
    movl -264(%rbp), %eax
    addl -360(%rbp), %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -264(%rbp)
    movq -136(%rbp), %rax
    movl %eax, -360(%rbp)
    movl -264(%rbp), %eax
    addl -360(%rbp), %eax
    movl %eax, -360(%rbp)
    movl -360(%rbp), %eax
    movl %eax, -264(%rbp)
    lea -264(%rbp), %rax
    movq %rax, -360(%rbp)
    movq -360(%rbp), %rax
    movq %rax, -344(%rbp)
    movl $1, %eax
    movl %eax, -360(%rbp)
    movq -344(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -368(%rbp)
    movl -368(%rbp), %eax
    addl -360(%rbp), %eax
    movl %eax, -360(%rbp)
    movq -344(%rbp), %rax
    movl -360(%rbp), %ebx
    movl %ebx, (%rax)
    movl -264(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
