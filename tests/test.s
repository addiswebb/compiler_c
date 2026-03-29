
.text
.global make_large
make_large:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
make_large_0:
    movq %rcx, -8(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl $1, %ecx
    movl %ecx, (%rax)
    movq -32(%rbp), %rax
    addq $4, %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movb $2, %cl
    movb %cl, (%rax)
    movq -32(%rbp), %rax
    addq $8, %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movb $3, %cl
    movb %cl, (%rax)
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rdx
    movq -8(%rbp), %rcx
    mov $12, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    mov %rbp, %rsp
    pop %rbp
    ret
.global make_small
make_small:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
make_small_0:
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movb $4, %cl
    movb %cl, (%rax)
    movq -16(%rbp), %rax
    addq $1, %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movb $5, %cl
    movb %cl, (%rax)
    movq -16(%rbp), %rax
    addq $4, %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movl $6, %ecx
    movl %ecx, (%rax)
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    mov %rbp, %rsp
    pop %rbp
    ret
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $64, %rsp
main_0:
    leaq -16(%rbp), %rax
    movq %rax, -48(%rbp)
    leaq -32(%rbp), %rax
    movq %rax, -56(%rbp)
    subq $32, %rsp
    movq -56(%rbp), %rcx
    call make_large
    addq $32, %rsp
    movq -56(%rbp), %rdx
    movq -48(%rbp), %rcx
    mov $12, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    leaq -40(%rbp), %rax
    movq %rax, -48(%rbp)
    subq $32, %rsp
    call make_small
    addq $32, %rsp
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rdx
    movq -48(%rbp), %rcx
    mov $8, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    leaq -16(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -48(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    addq $4, %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    movb (%rax), %al
    movb %al, -56(%rbp)
    movsbl -56(%rbp), %eax
    movl %eax, -56(%rbp)
    movl -48(%rbp), %eax
    addl -56(%rbp), %eax
    movl %eax, -48(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    addq $8, %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    movb (%rax), %al
    movb %al, -56(%rbp)
    movsbl -56(%rbp), %eax
    movl %eax, -56(%rbp)
    movl -48(%rbp), %eax
    addl -56(%rbp), %eax
    movl %eax, -48(%rbp)
    leaq -40(%rbp), %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    addq $4, %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -56(%rbp)
    movl -48(%rbp), %eax
    addl -56(%rbp), %eax
    movl %eax, -48(%rbp)
    leaq -40(%rbp), %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    movb (%rax), %al
    movb %al, -56(%rbp)
    movsbl -56(%rbp), %eax
    movl %eax, -56(%rbp)
    movl -48(%rbp), %eax
    addl -56(%rbp), %eax
    movl %eax, -48(%rbp)
    leaq -40(%rbp), %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    addq $1, %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    movb (%rax), %al
    movb %al, -56(%rbp)
    movsbl -56(%rbp), %eax
    movl %eax, -56(%rbp)
    movl -48(%rbp), %eax
    addl -56(%rbp), %eax
    movl %eax, -48(%rbp)
    movl -48(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
