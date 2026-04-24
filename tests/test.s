.section .note.GNU-stack,"",@progbits
.section .rodata
.LC0:
    .byte 'a', 'b', 'C', 'D', 0
.LC1:
    .byte 0x5C, 0x22, 0x0A, 0
.LC2:
    .byte '%', 'd', 0x0A, 0

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $112, %rsp
main_0:
    leaq -8(%rbp), %rax
    movq %rax, -88(%rbp)
    movq -88(%rbp), %rax
    movl $10, %ecx
    movl %ecx, (%rax)
    leaq -16(%rbp), %rax
    movq %rax, -88(%rbp)
    movq -88(%rbp), %rax
    movl $92, %ecx
    movl %ecx, (%rax)
    leaq -24(%rbp), %rax
    movq %rax, -88(%rbp)
    movq -88(%rbp), %rax
    movl $9, %ecx
    movl %ecx, (%rax)
    leaq -32(%rbp), %rax
    movq %rax, -88(%rbp)
    movq -88(%rbp), %rax
    movl $65, %ecx
    movl %ecx, (%rax)
    leaq -40(%rbp), %rax
    movq %rax, -88(%rbp)
    movq -88(%rbp), %rax
    movl $65, %ecx
    movl %ecx, (%rax)
    leaq -48(%rbp), %rax
    movq %rax, -88(%rbp)
    leaq .LC0(%rip), %rax
    movq %rax, -96(%rbp)
    movq -96(%rbp), %rsi
    movq -88(%rbp), %rdi
    mov $5, %rdx
    call memcpy
    leaq -56(%rbp), %rax
    movq %rax, -88(%rbp)
    leaq .LC1(%rip), %rax
    movq %rax, -96(%rbp)
    movq -96(%rbp), %rsi
    movq -88(%rbp), %rdi
    mov $4, %rdx
    call memcpy
    leaq -64(%rbp), %rax
    movq %rax, -88(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -96(%rbp)
    movq -96(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -96(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -104(%rbp)
    movq -104(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -104(%rbp)
    movl -96(%rbp), %eax
    addl -104(%rbp), %eax
    movl %eax, -96(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -104(%rbp)
    movq -104(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -104(%rbp)
    movl -96(%rbp), %eax
    addl -104(%rbp), %eax
    movl %eax, -96(%rbp)
    leaq -32(%rbp), %rax
    movq %rax, -104(%rbp)
    movq -104(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -104(%rbp)
    movl -96(%rbp), %eax
    addl -104(%rbp), %eax
    movl %eax, -96(%rbp)
    leaq -40(%rbp), %rax
    movq %rax, -104(%rbp)
    movq -104(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -104(%rbp)
    movl -96(%rbp), %eax
    addl -104(%rbp), %eax
    movl %eax, -96(%rbp)
    movq -88(%rbp), %rax
    movl -96(%rbp), %ecx
    movl %ecx, (%rax)
    leaq -72(%rbp), %rax
    movq %rax, -88(%rbp)
    movq -88(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
main_1:
    leaq -48(%rbp), %rax
    movq %rax, -88(%rbp)
    leaq -72(%rbp), %rax
    movq %rax, -96(%rbp)
    movq -96(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -96(%rbp)
    movslq -96(%rbp), %rax
    movq %rax, -96(%rbp)
    movq -96(%rbp), %rax
    imulq $1, %rax
    movq %rax, -96(%rbp)
    movq -88(%rbp), %rax
    addq -96(%rbp), %rax
    movq %rax, -88(%rbp)
    movq -88(%rbp), %rax
    movb (%rax), %al
    movb %al, -88(%rbp)
    movsbl -88(%rbp), %eax
    movl %eax, -88(%rbp)
    movl -88(%rbp), %eax
    testl %eax, %eax
    jz main_4
main_2:
    leaq -64(%rbp), %rax
    movq %rax, -88(%rbp)
    leaq -48(%rbp), %rax
    movq %rax, -96(%rbp)
    leaq -72(%rbp), %rax
    movq %rax, -104(%rbp)
    movq -104(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -104(%rbp)
    movslq -104(%rbp), %rax
    movq %rax, -104(%rbp)
    movq -104(%rbp), %rax
    imulq $1, %rax
    movq %rax, -104(%rbp)
    movq -96(%rbp), %rax
    addq -104(%rbp), %rax
    movq %rax, -96(%rbp)
    movq -96(%rbp), %rax
    movb (%rax), %al
    movb %al, -96(%rbp)
    movsbl -96(%rbp), %eax
    movl %eax, -96(%rbp)
    movq -88(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    addl -96(%rbp), %eax
    movl %eax, -96(%rbp)
    movq -88(%rbp), %rax
    movl -96(%rbp), %ecx
    movl %ecx, (%rax)
    jmp main_3
main_3:
    leaq -72(%rbp), %rax
    movq %rax, -88(%rbp)
    movq -88(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -96(%rbp)
    movl -96(%rbp), %eax
    addl $1, %eax
    movl %eax, -96(%rbp)
    movq -88(%rbp), %rax
    movl -96(%rbp), %ecx
    movl %ecx, (%rax)
    jmp main_1
main_4:
    leaq -80(%rbp), %rax
    movq %rax, -88(%rbp)
    movq -88(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
main_5:
    leaq -56(%rbp), %rax
    movq %rax, -88(%rbp)
    leaq -80(%rbp), %rax
    movq %rax, -96(%rbp)
    movq -96(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -96(%rbp)
    movslq -96(%rbp), %rax
    movq %rax, -96(%rbp)
    movq -96(%rbp), %rax
    imulq $1, %rax
    movq %rax, -96(%rbp)
    movq -88(%rbp), %rax
    addq -96(%rbp), %rax
    movq %rax, -88(%rbp)
    movq -88(%rbp), %rax
    movb (%rax), %al
    movb %al, -88(%rbp)
    movsbl -88(%rbp), %eax
    movl %eax, -88(%rbp)
    movl -88(%rbp), %eax
    testl %eax, %eax
    jz main_8
main_6:
    leaq -64(%rbp), %rax
    movq %rax, -88(%rbp)
    leaq -56(%rbp), %rax
    movq %rax, -96(%rbp)
    leaq -80(%rbp), %rax
    movq %rax, -104(%rbp)
    movq -104(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -104(%rbp)
    movslq -104(%rbp), %rax
    movq %rax, -104(%rbp)
    movq -104(%rbp), %rax
    imulq $1, %rax
    movq %rax, -104(%rbp)
    movq -96(%rbp), %rax
    addq -104(%rbp), %rax
    movq %rax, -96(%rbp)
    movq -96(%rbp), %rax
    movb (%rax), %al
    movb %al, -96(%rbp)
    movsbl -96(%rbp), %eax
    movl %eax, -96(%rbp)
    movq -88(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    addl -96(%rbp), %eax
    movl %eax, -96(%rbp)
    movq -88(%rbp), %rax
    movl -96(%rbp), %ecx
    movl %ecx, (%rax)
    jmp main_7
main_7:
    leaq -80(%rbp), %rax
    movq %rax, -88(%rbp)
    movq -88(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -96(%rbp)
    movl -96(%rbp), %eax
    addl $1, %eax
    movl %eax, -96(%rbp)
    movq -88(%rbp), %rax
    movl -96(%rbp), %ecx
    movl %ecx, (%rax)
    jmp main_5
main_8:
    leaq .LC2(%rip), %rax
    movq %rax, -88(%rbp)
    leaq -64(%rbp), %rax
    movq %rax, -96(%rbp)
    movq -96(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -96(%rbp)
    subq $176, %rsp
    movq -88(%rbp), %rdi
    movl -96(%rbp), %esi
    xor %eax, %eax
    call printf
    addq $176, %rsp
    movl %eax, -88(%rbp)
    movl $0, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
