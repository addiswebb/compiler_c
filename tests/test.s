.section .note.GNU-stack,"",@progbits
.section .rodata

.text
.global foo
foo:
    push %rbp
    mov %rsp, %rbp
    subq $128, %rsp
foo_0:
    movq %rdi, %rax
    movq %rax, -8(%rbp)
    movq %rsi, %rax
    movq %rax, -24(%rbp)
    movq %rdx, %rax
    movq %rax, -40(%rbp)
    movq %rcx, %rax
    movq %rax, -56(%rbp)
    leaq -88(%rbp), %rax
    movq %rax, -120(%rbp)
    movq -120(%rbp), %rax
    movl -24(%rbp), %ecx
    movl %ecx, (%rax)
    leaq -88(%rbp), %rax
    movq %rax, -120(%rbp)
    movl $4, %eax
    movq %rax, -128(%rbp)
    movq -120(%rbp), %rax
    addq -128(%rbp), %rax
    movq %rax, -120(%rbp)
    movq -120(%rbp), %rax
    movl -40(%rbp), %ecx
    movl %ecx, (%rax)
    leaq -88(%rbp), %rax
    movq %rax, -120(%rbp)
    movq -120(%rbp), %rsi
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
    subq $128, %rsp
main_0:
    movl $5, %eax
    movl %eax, -104(%rbp)
    movl $7, %eax
    movl %eax, -112(%rbp)
    movl $2, %eax
    movl %eax, -120(%rbp)
    leaq -72(%rbp), %rax
    movq %rax, -128(%rbp)
    movq -128(%rbp), %rdi
    movl -104(%rbp), %esi
    movl -112(%rbp), %edx
    movl -120(%rbp), %ecx
    call foo
    addq $0, %rsp
    leaq -24(%rbp), %rax
    movq %rax, -104(%rbp)
    movq -128(%rbp), %rsi
    movq -104(%rbp), %rdi
    mov $20, %rdx
    call memcpy
    leaq -24(%rbp), %rax
    movq %rax, -104(%rbp)
    movq -104(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -104(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -112(%rbp)
    movl $4, %eax
    movq %rax, -120(%rbp)
    movq -112(%rbp), %rax
    addq -120(%rbp), %rax
    movq %rax, -112(%rbp)
    movq -112(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -112(%rbp)
    movl -104(%rbp), %eax
    addl -112(%rbp), %eax
    movl %eax, -104(%rbp)
    movq -104(%rbp), %rax
    mov %rbp, %rsp
    pop %rbp
    ret
