.section .rodata
.align 8
.LC0:
    .quad 0x40091eb851eb851f
.global global_int
.data
.align 4
global_int:
    .long 42
.global global_double
.bss
.align 8
global_double:
    .zero 8
.global global_array
.bss
.align 4
global_array:
    .zero 20
.global global_ptr
.bss
.align 8
global_ptr:
    .zero 8

.text
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $96, %rsp
main_0:
    leaq global_double(%rip), %rax
    movq %rax, -64(%rbp)
    movsd .LC0(%rip), %xmm0
    movsd %xmm0, -72(%rbp)
    movq -64(%rbp), %rax
    movsd -72(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq global_array(%rip), %rax
    movq %rax, -64(%rbp)
    movq $0, %rax
    imulq $4, %rax
    movq %rax, -72(%rbp)
    movq -64(%rbp), %rax
    addq -72(%rbp), %rax
    movq %rax, -64(%rbp)
    movq -64(%rbp), %rax
    movl $10, %ecx
    movl %ecx, (%rax)
    leaq global_array(%rip), %rax
    movq %rax, -64(%rbp)
    movq $1, %rax
    imulq $4, %rax
    movq %rax, -72(%rbp)
    movq -64(%rbp), %rax
    addq -72(%rbp), %rax
    movq %rax, -64(%rbp)
    movq -64(%rbp), %rax
    movl $20, %ecx
    movl %ecx, (%rax)
    leaq global_array(%rip), %rax
    movq %rax, -64(%rbp)
    movq $2, %rax
    imulq $4, %rax
    movq %rax, -72(%rbp)
    movq -64(%rbp), %rax
    addq -72(%rbp), %rax
    movq %rax, -64(%rbp)
    movq -64(%rbp), %rax
    movl $30, %ecx
    movl %ecx, (%rax)
    leaq global_ptr(%rip), %rax
    movq %rax, -64(%rbp)
    leaq global_int(%rip), %rax
    movq %rax, -72(%rbp)
    movq -64(%rbp), %rax
    movq -72(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -56(%rbp), %rax
    movq %rax, -64(%rbp)
    leaq global_int(%rip), %rax
    movq %rax, -72(%rbp)
    movq -72(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -72(%rbp)
    leaq global_array(%rip), %rax
    movq %rax, -80(%rbp)
    movq $0, %rax
    imulq $4, %rax
    movq %rax, -88(%rbp)
    movq -80(%rbp), %rax
    addq -88(%rbp), %rax
    movq %rax, -80(%rbp)
    movq -80(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -80(%rbp)
    movl -72(%rbp), %eax
    addl -80(%rbp), %eax
    movl %eax, -72(%rbp)
    leaq global_array(%rip), %rax
    movq %rax, -80(%rbp)
    movq $1, %rax
    imulq $4, %rax
    movq %rax, -88(%rbp)
    movq -80(%rbp), %rax
    addq -88(%rbp), %rax
    movq %rax, -80(%rbp)
    movq -80(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -80(%rbp)
    movl -72(%rbp), %eax
    addl -80(%rbp), %eax
    movl %eax, -72(%rbp)
    leaq global_array(%rip), %rax
    movq %rax, -80(%rbp)
    movq $2, %rax
    imulq $4, %rax
    movq %rax, -88(%rbp)
    movq -80(%rbp), %rax
    addq -88(%rbp), %rax
    movq %rax, -80(%rbp)
    movq -80(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -80(%rbp)
    movl -72(%rbp), %eax
    addl -80(%rbp), %eax
    movl %eax, -72(%rbp)
    movq -64(%rbp), %rax
    movl -72(%rbp), %ecx
    movl %ecx, (%rax)
    leaq -56(%rbp), %rax
    movq %rax, -64(%rbp)
    movq -64(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -64(%rbp)
    movl -64(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
