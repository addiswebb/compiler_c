.section .note.GNU-stack,"",@progbits

.text
.global make_small
make_small:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp
make_small_0:
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movl $1, %ecx
    movl %ecx, (%rax)
    movq -16(%rbp), %rax
    addq $4, %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movl $2, %ecx
    movl %ecx, (%rax)
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    mov %rbp, %rsp
    pop %rbp
    ret
.global make_large
make_large:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
make_large_0:
    movq %rdi, -8(%rbp)
    leaq -32(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl $1, %ecx
    movl %ecx, (%rax)
    movq -40(%rbp), %rax
    addq $4, %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl $2, %ecx
    movl %ecx, (%rax)
    movq -40(%rbp), %rax
    addq $8, %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl $3, %ecx
    movl %ecx, (%rax)
    movq -40(%rbp), %rax
    addq $12, %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl $4, %ecx
    movl %ecx, (%rax)
    movq -40(%rbp), %rax
    addq $16, %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl $5, %ecx
    movl %ecx, (%rax)
    leaq -32(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rsi
    movq -8(%rbp), %rdi
    mov $20, %rdx
    call memcpy
    mov %rbp, %rsp
    pop %rbp
    ret
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $80, %rsp
main_0:
    leaq -8(%rbp), %rax
    movq %rax, -64(%rbp)
    call make_small
    movq %rax, -72(%rbp)
    movq -72(%rbp), %rsi
    movq -64(%rbp), %rdi
    mov $8, %rdx
    call memcpy
    leaq -32(%rbp), %rax
    movq %rax, -64(%rbp)
    leaq -56(%rbp), %rax
    movq %rax, -72(%rbp)
    movq -72(%rbp), %rdi
    call make_large
    movq -72(%rbp), %rsi
    movq -64(%rbp), %rdi
    mov $20, %rdx
    call memcpy
    leaq -32(%rbp), %rax
    movq %rax, -64(%rbp)
    movq -64(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -64(%rbp)
    leaq -32(%rbp), %rax
    movq %rax, -72(%rbp)
    movq -72(%rbp), %rax
    addq $4, %rax
    movq %rax, -72(%rbp)
    movq -72(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -72(%rbp)
    movl -64(%rbp), %eax
    addl -72(%rbp), %eax
    movl %eax, -64(%rbp)
    leaq -32(%rbp), %rax
    movq %rax, -72(%rbp)
    movq -72(%rbp), %rax
    addq $8, %rax
    movq %rax, -72(%rbp)
    movq -72(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -72(%rbp)
    movl -64(%rbp), %eax
    addl -72(%rbp), %eax
    movl %eax, -64(%rbp)
    leaq -32(%rbp), %rax
    movq %rax, -72(%rbp)
    movq -72(%rbp), %rax
    addq $12, %rax
    movq %rax, -72(%rbp)
    movq -72(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -72(%rbp)
    movl -64(%rbp), %eax
    addl -72(%rbp), %eax
    movl %eax, -64(%rbp)
    leaq -32(%rbp), %rax
    movq %rax, -72(%rbp)
    movq -72(%rbp), %rax
    addq $16, %rax
    movq %rax, -72(%rbp)
    movq -72(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -72(%rbp)
    movl -64(%rbp), %eax
    addl -72(%rbp), %eax
    movl %eax, -64(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -72(%rbp)
    movq -72(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -72(%rbp)
    movl -64(%rbp), %eax
    addl -72(%rbp), %eax
    movl %eax, -64(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -72(%rbp)
    movq -72(%rbp), %rax
    addq $4, %rax
    movq %rax, -72(%rbp)
    movq -72(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -72(%rbp)
    movl -64(%rbp), %eax
    addl -72(%rbp), %eax
    movl %eax, -64(%rbp)
    movl -64(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
