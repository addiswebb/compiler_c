.section .rodata
.LC0:
    .byte '%', 'd', ' ', '%', 'd', ' ', '%', 'd', 0x0A, 0

.text
.global print_ir_value
print_ir_value:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
print_ir_value_0:
    movq %rcx, -8(%rbp)
    leaq .LC0(%rip), %rax
    movq %rax, -16(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -24(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    addq $8, %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -32(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    addq $4, %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    subq $32, %rsp
    movq -16(%rbp), %rcx
    movl -24(%rbp), %edx
    movl -32(%rbp), %r8d
    movl -40(%rbp), %r9d
    call printf
    addq $32, %rsp
    movl %eax, -16(%rbp)
    mov %rbp, %rsp
    pop %rbp
    ret
.global ir_binary
ir_binary:
    push %rbp
    mov %rsp, %rbp
    subq $112, %rsp
ir_binary_0:
    movq %rcx, -8(%rbp)
    movq %rdx, -16(%rbp)
    movl %r8d, -24(%rbp)
    movq %r9, %rax
    movq %rax, -80(%rbp)
    movq 48(%rbp), %rax
    movq %rax, -88(%rbp)
    movq 56(%rbp), %rax
    movq %rax, -96(%rbp)
    movq 64(%rbp), %rax
    movq %rax, -32(%rbp)
    leaq 16(%rbp), %rax
    movq %rax, -104(%rbp)
    movq -104(%rbp), %rcx
    movq -80(%rbp), %rdx
    mov $36, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    leaq 56(%rbp), %rax
    movq %rax, -80(%rbp)
    movq -80(%rbp), %rcx
    movq -88(%rbp), %rdx
    mov $36, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    leaq 96(%rbp), %rax
    movq %rax, -80(%rbp)
    movq -80(%rbp), %rcx
    movq -96(%rbp), %rdx
    mov $36, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    leaq 16(%rbp), %rax
    movq %rax, -80(%rbp)
    subq $32, %rsp
    movq -80(%rbp), %rcx
    call print_ir_value
    addq $32, %rsp
    leaq 56(%rbp), %rax
    movq %rax, -80(%rbp)
    subq $32, %rsp
    movq -80(%rbp), %rcx
    call print_ir_value
    addq $32, %rsp
    leaq 96(%rbp), %rax
    movq %rax, -80(%rbp)
    subq $32, %rsp
    movq -80(%rbp), %rcx
    call print_ir_value
    addq $32, %rsp
    leaq -72(%rbp), %rax
    movq %rax, -80(%rbp)
    movq -80(%rbp), %rax
    movl $1, %ecx
    movl %ecx, (%rax)
    movq -80(%rbp), %rax
    addq $4, %rax
    movq %rax, -88(%rbp)
    movq -88(%rbp), %rax
    movl $2, %ecx
    movl %ecx, (%rax)
    movq -80(%rbp), %rax
    addq $8, %rax
    movq %rax, -88(%rbp)
    movq -88(%rbp), %rax
    movl $3, %ecx
    movl %ecx, (%rax)
    movq -80(%rbp), %rax
    addq $12, %rax
    movq %rax, -80(%rbp)
    movq -80(%rbp), %rcx
    mov $0, %rdx
    movq $24, %r8
    sub $32, %rsp
    call memset
    add $32, %rsp
    leaq -72(%rbp), %rax
    movq %rax, -80(%rbp)
    movq -8(%rbp), %rcx
    movq -80(%rbp), %rdx
    mov $36, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    mov %rbp, %rsp
    pop %rbp
    ret
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $240, %rsp
main_0:
    leaq -40(%rbp), %rax
    movq %rax, -208(%rbp)
    movq -208(%rbp), %rax
    movl $1, %ecx
    movl %ecx, (%rax)
    movq -208(%rbp), %rax
    addq $4, %rax
    movq %rax, -216(%rbp)
    movq -216(%rbp), %rax
    movl $2, %ecx
    movl %ecx, (%rax)
    movq -208(%rbp), %rax
    addq $8, %rax
    movq %rax, -216(%rbp)
    movq -216(%rbp), %rax
    movl $3, %ecx
    movl %ecx, (%rax)
    movq -208(%rbp), %rax
    addq $12, %rax
    movq %rax, -208(%rbp)
    movq -208(%rbp), %rcx
    mov $0, %rdx
    movq $24, %r8
    sub $32, %rsp
    call memset
    add $32, %rsp
    leaq -80(%rbp), %rax
    movq %rax, -208(%rbp)
    movq -208(%rbp), %rax
    movl $4, %ecx
    movl %ecx, (%rax)
    movq -208(%rbp), %rax
    addq $4, %rax
    movq %rax, -216(%rbp)
    movq -216(%rbp), %rax
    movl $5, %ecx
    movl %ecx, (%rax)
    movq -208(%rbp), %rax
    addq $8, %rax
    movq %rax, -216(%rbp)
    movq -216(%rbp), %rax
    movl $6, %ecx
    movl %ecx, (%rax)
    movq -208(%rbp), %rax
    addq $12, %rax
    movq %rax, -208(%rbp)
    movq -208(%rbp), %rcx
    mov $0, %rdx
    movq $24, %r8
    sub $32, %rsp
    call memset
    add $32, %rsp
    leaq -120(%rbp), %rax
    movq %rax, -208(%rbp)
    movq -208(%rbp), %rax
    movl $7, %ecx
    movl %ecx, (%rax)
    movq -208(%rbp), %rax
    addq $4, %rax
    movq %rax, -216(%rbp)
    movq -216(%rbp), %rax
    movl $8, %ecx
    movl %ecx, (%rax)
    movq -208(%rbp), %rax
    addq $8, %rax
    movq %rax, -216(%rbp)
    movq -216(%rbp), %rax
    movl $9, %ecx
    movl %ecx, (%rax)
    movq -208(%rbp), %rax
    addq $12, %rax
    movq %rax, -208(%rbp)
    movq -208(%rbp), %rcx
    mov $0, %rdx
    movq $24, %r8
    sub $32, %rsp
    call memset
    add $32, %rsp
    leaq -160(%rbp), %rax
    movq %rax, -208(%rbp)
    leaq -200(%rbp), %rax
    movq %rax, -216(%rbp)
    leaq -40(%rbp), %rax
    movq %rax, -224(%rbp)
    leaq -80(%rbp), %rax
    movq %rax, -232(%rbp)
    leaq -120(%rbp), %rax
    movq %rax, -240(%rbp)
    subq $64, %rsp
    movq -216(%rbp), %rcx
    movq $123, %rdx
    movl $4, %r8d
    movq -224(%rbp), %r9
    movq -232(%rbp), %rax
    movq %rax, 32(%rsp)
    movq -240(%rbp), %rax
    movq %rax, 40(%rsp)
    movq $567, %rax
    movq %rax, 48(%rsp)
    call ir_binary
    addq $64, %rsp
    movq -216(%rbp), %rax
    movq %rax, -224(%rbp)
    movq -208(%rbp), %rcx
    movq -224(%rbp), %rdx
    mov $36, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    movl $0, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
