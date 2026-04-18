.section .note.GNU-stack,"",@progbits
.section .rodata
.align 8
.LC0:
    .quad 0xc03d000000000000
.align 8
.LC1:
    .quad 0x0000000000000000
.LC2:
    .byte '0', 0
.LC3:
    .byte 'F', 'A', 'I', 'L', 'E', 'D', ' ', 'T', 'O', ' ', 'P', 'A', 'R', 'S', 'E', ' ', 'T', 'O', 'K', 'E', 'N', 0
.align 8
.LC4:
    .quad 0x0000000000000000
.align 8
.LC5:
    .quad 0x0000000000000000
.LC6:
    .byte 'a', '=', ' ', '%', 'l', 'f', 0x0A, 0
.LC7:
    .byte 'N', 'o', ' ', 'o', 'p', 'e', 'r', 'a', 't', 'o', 'r', ' ', 'g', 'i', 'v', 'e', 'n', 0x0A, 0
.align 8
.LC8:
    .quad 0x0000000000000000
.LC9:
    .byte 's', '1', 0x0A, 0x0A, 0
.align 8
.LC10:
    .quad 0x0000000000000000
.LC11:
    .byte 's', '2', 0x0A, 0
.LC12:
    .byte 's', '3', 0x0A, 0
.LC13:
    .byte 'N', 'o', ' ', 'o', 'p', 'e', 'r', 'a', 't', 'o', 'r', ' ', 'g', 'i', 'v', 'e', 'n', 0
.align 8
.LC14:
    .quad 0x0000000000000000
.LC15:
    .byte 's', '4', 0x0A, 0
.align 8
.LC16:
    .quad 0x0000000000000000
.LC17:
    .byte 'I', 'N', 'V', 'A', 'L', 'I', 'D', ' ', 'S', 'Y', 'N', 'T', 'A', 'X', ',', ' ', 'S', 'I', 'N', 'G', 'L', 'E', ' ', 'E', 'Q', 'U', 'A', 'L', 'S', ' ', 'S', 'I', 'G', 'N', ' ', 'O', 'N', 'L', 'Y', 0
.LC18:
    .byte ' ', 0
.LC19:
    .byte 'N', 'O', ' ', 'E', 'N', 'D', ' ', 'O', 'F', ' ', 'S', 'T', 'R', 'I', 'N', 'G', ' ', 'F', 'O', 'U', 'N', 'D', '!', 0
.LC20:
    .byte ' ', 0
.LC21:
    .byte 'N', 'O', ' ', 'B', 'R', 'A', 'C', 'K', 'E', 'T', 'S', ' ', 'A', 'R', 'E', ' ', 'C', 'O', 'O', 'K', 'E', 'D', 0
.LC22:
    .byte ' ', 0
.LC23:
    .byte 'T', 'R', 'I', 'E', 'D', ' ', 'T', 'O', ' ', 'C', 'R', 'E', 'A', 'T', 'E', ' ', 'V', 'A', 'R', 'I', 'A', 'B', 'L', 'E', ' ', 'W', 'I', 'T', 'H', ' ', 'N', 'O', 'N', '-', 'A', 'L', 'P', 'H', 'A', ' ', 'F', 'I', 'R', 'S', 'T', ' ', 'C', 'H', 'A', 'R', 'A', 'C', 'T', 'E', 'R', ':', ' ', 0
.LC24:
    .byte 'E', 'R', 'R', 'O', 'R', ':', ' ', '%', 's', ' ', '%', 's', 0x0A, 0x0A, 0
.LC25:
    .byte '%', 'g', 0
.LC26:
    .byte 't', 'r', 'u', 'e', 0
.LC27:
    .byte 'f', 'a', 'l', 's', 'e', 0
.LC28:
    .byte 'S', 't', 'a', 'r', 't', 'i', 'n', 'g', ' ', 'C', 'a', 'l', 'c', 'u', 'l', 'a', 't', 'o', 'r', 0x0A, 0
.align 8
.LC29:
    .quad 0x0000000000000000
.LC30:
    .byte '>', '>', ' ', 0
.LC31:
    .byte 'r', 0
.LC32:
    .byte 'E', 'n', 'd', 'i', 'n', 'g', ' ', 'C', 'a', 'l', 'c', 'u', 'l', 'a', 't', 'o', 'r', '.', 0x0A, 0
.LC33:
    .byte 'e', 'x', 'i', 't', 0x0A, 0
.LC34:
    .byte 'E', 'n', 'd', 'i', 'n', 'g', ' ', 'C', 'a', 'l', 'c', 'u', 'l', 'a', 't', 'o', 'r', '.', 0x0A, 0
.LC35:
    .byte '%', 's', ' ', '=', ' ', 0
.LC36:
    .byte 0x0A, 0
.global CONSTANTS_NAMES
.bss
.align 8
CONSTANTS_NAMES:
    .zero 24
.global CONSTANTS_VALUES
.bss
.align 8
CONSTANTS_VALUES:
    .zero 24
.global variableNames
.bss
.align 8
variableNames:
    .zero 160
.global variables
.bss
.align 8
variables:
    .zero 800
.global variableCount
.data
.align 4
variableCount:
    .long 0

.text
.global my_pow
my_pow:
    push %rbp
    mov %rsp, %rbp
    subq $1072, %rsp
my_pow_0:
    movsd %xmm0, -1024(%rbp)
    movsd %xmm1, -1032(%rbp)
    leaq -1032(%rbp), %rax
    movq %rax, -1040(%rbp)
    movq -1040(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1040(%rbp)
    mov $1, %eax
    movl %eax, -1048(%rbp)
    movl -1040(%rbp), %eax
    cmpl -1048(%rbp), %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1040(%rbp)
    movl -1040(%rbp), %eax
    testl %eax, %eax
    jz my_pow_2
my_pow_1:
    leaq -1024(%rbp), %rax
    movq %rax, -1040(%rbp)
    movq -1040(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1040(%rbp)
    movsd -1040(%rbp), %xmm0
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp my_pow_2
my_pow_2:
    leaq -1024(%rbp), %rax
    movq %rax, -1040(%rbp)
    movq -1040(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1040(%rbp)
    leaq -1024(%rbp), %rax
    movq %rax, -1048(%rbp)
    movq -1048(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1048(%rbp)
    leaq -1032(%rbp), %rax
    movq %rax, -1056(%rbp)
    movq -1056(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1056(%rbp)
    mov $1, %eax
    movl %eax, -1064(%rbp)
    movsd -1056(%rbp), %xmm0
    subsd -1064(%rbp), %xmm0
    movsd %xmm0, -1056(%rbp)
    movsd -1048(%rbp), %xmm0
    movsd -1056(%rbp), %xmm1
    call my_pow
    movsd %xmm0, -1048(%rbp)
    movsd -1040(%rbp), %xmm0
    mulsd -1048(%rbp), %xmm0
    movsd %xmm0, -1040(%rbp)
    movsd -1040(%rbp), %xmm0
    mov %rbp, %rsp
    pop %rbp
    ret
.global format
format:
    push %rbp
    mov %rsp, %rbp
    subq $1072, %rsp
format_0:
    movq %rdi, -1024(%rbp)
    leaq -1032(%rbp), %rax
    movq %rax, -1048(%rbp)
    movq -1048(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    leaq -1040(%rbp), %rax
    movq %rax, -1048(%rbp)
    movq -1048(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
format_1:
    leaq -1040(%rbp), %rax
    movq %rax, -1048(%rbp)
    movq -1048(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1048(%rbp)
    leaq -1024(%rbp), %rax
    movq %rax, -1056(%rbp)
    movq -1056(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1056(%rbp)
    movq -1056(%rbp), %rax
    movq %rax, -1056(%rbp)
    movq -1056(%rbp), %rdi
    call strlen
    movq %rax, -1056(%rbp)
    movq -1056(%rbp), %rax
    movl %eax, -1056(%rbp)
    movl -1048(%rbp), %eax
    cmpl -1056(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -1048(%rbp)
    movl -1048(%rbp), %eax
    testl %eax, %eax
    jz format_7
format_2:
    leaq -1024(%rbp), %rax
    movq %rax, -1048(%rbp)
    movq -1048(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1048(%rbp)
    leaq -1040(%rbp), %rax
    movq %rax, -1056(%rbp)
    movq -1056(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1056(%rbp)
    movslq -1056(%rbp), %rax
    movq %rax, -1056(%rbp)
    movq -1056(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1056(%rbp)
    movq -1048(%rbp), %rax
    addq -1056(%rbp), %rax
    movq %rax, -1048(%rbp)
    movq -1048(%rbp), %rax
    movb (%rax), %al
    movb %al, -1048(%rbp)
    movsbl -1048(%rbp), %eax
    movl %eax, -1048(%rbp)
    movl -1048(%rbp), %eax
    cmpl $32, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1048(%rbp)
    movl -1048(%rbp), %eax
    testl %eax, %eax
    jz format_4
format_3:
    leaq -1024(%rbp), %rax
    movq %rax, -1048(%rbp)
    movq -1048(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1048(%rbp)
    leaq -1032(%rbp), %rax
    movq %rax, -1056(%rbp)
    movq -1056(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1064(%rbp)
    movl -1064(%rbp), %eax
    addl $1, %eax
    movl %eax, -1072(%rbp)
    movq -1056(%rbp), %rax
    movl -1072(%rbp), %ecx
    movl %ecx, (%rax)
    movslq -1064(%rbp), %rax
    movq %rax, -1056(%rbp)
    movq -1056(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1056(%rbp)
    movq -1048(%rbp), %rax
    addq -1056(%rbp), %rax
    movq %rax, -1048(%rbp)
    leaq -1024(%rbp), %rax
    movq %rax, -1056(%rbp)
    movq -1056(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1056(%rbp)
    leaq -1040(%rbp), %rax
    movq %rax, -1064(%rbp)
    movq -1064(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1072(%rbp)
    movl -1072(%rbp), %eax
    addl $1, %eax
    movl %eax, -1072(%rbp)
    movq -1064(%rbp), %rax
    movl -1072(%rbp), %ecx
    movl %ecx, (%rax)
    movslq -1072(%rbp), %rax
    movq %rax, -1064(%rbp)
    movq -1064(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1064(%rbp)
    movq -1056(%rbp), %rax
    addq -1064(%rbp), %rax
    movq %rax, -1056(%rbp)
    movq -1056(%rbp), %rax
    movb (%rax), %al
    movb %al, -1056(%rbp)
    movq -1048(%rbp), %rax
    movb -1056(%rbp), %cl
    movb %cl, (%rax)
    jmp format_5
format_4:
    leaq -1024(%rbp), %rax
    movq %rax, -1048(%rbp)
    movq -1048(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1048(%rbp)
    leaq -1032(%rbp), %rax
    movq %rax, -1056(%rbp)
    movq -1056(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1064(%rbp)
    movl -1064(%rbp), %eax
    addl $1, %eax
    movl %eax, -1072(%rbp)
    movq -1056(%rbp), %rax
    movl -1072(%rbp), %ecx
    movl %ecx, (%rax)
    movslq -1064(%rbp), %rax
    movq %rax, -1056(%rbp)
    movq -1056(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1056(%rbp)
    movq -1048(%rbp), %rax
    addq -1056(%rbp), %rax
    movq %rax, -1048(%rbp)
    leaq -1024(%rbp), %rax
    movq %rax, -1056(%rbp)
    movq -1056(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1056(%rbp)
    leaq -1040(%rbp), %rax
    movq %rax, -1064(%rbp)
    movq -1064(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1064(%rbp)
    movslq -1064(%rbp), %rax
    movq %rax, -1064(%rbp)
    movq -1064(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1064(%rbp)
    movq -1056(%rbp), %rax
    addq -1064(%rbp), %rax
    movq %rax, -1056(%rbp)
    movq -1056(%rbp), %rax
    movb (%rax), %al
    movb %al, -1056(%rbp)
    movq -1048(%rbp), %rax
    movb -1056(%rbp), %cl
    movb %cl, (%rax)
    jmp format_5
format_5:
    jmp format_6
format_6:
    leaq -1040(%rbp), %rax
    movq %rax, -1048(%rbp)
    movq -1048(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1056(%rbp)
    movl -1056(%rbp), %eax
    addl $1, %eax
    movl %eax, -1056(%rbp)
    movq -1048(%rbp), %rax
    movl -1056(%rbp), %ecx
    movl %ecx, (%rax)
    jmp format_1
format_7:
    leaq -1024(%rbp), %rax
    movq %rax, -1048(%rbp)
    movq -1048(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1048(%rbp)
    leaq -1032(%rbp), %rax
    movq %rax, -1056(%rbp)
    movq -1056(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1056(%rbp)
    movslq -1056(%rbp), %rax
    movq %rax, -1056(%rbp)
    movq -1056(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1056(%rbp)
    movq -1048(%rbp), %rax
    addq -1056(%rbp), %rax
    movq %rax, -1048(%rbp)
    movq -1048(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    mov %rbp, %rsp
    pop %rbp
    ret
.global parseToken
parseToken:
    push %rbp
    mov %rsp, %rbp
    subq $1120, %rsp
parseToken_0:
    movq %rdi, -1024(%rbp)
    movq %rsi, -1032(%rbp)
    leaq -1072(%rbp), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    addq $8, %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    leaq -1072(%rbp), %rax
    movq %rax, -1104(%rbp)
    movsd .LC0(%rip), %xmm0
    movsd %xmm0, -1112(%rbp)
    movq -1104(%rbp), %rax
    movsd -1112(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1088(%rbp), %rax
    movq %rax, -1104(%rbp)
    leaq -1032(%rbp), %rax
    movq %rax, -1112(%rbp)
    movq -1112(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1112(%rbp)
    movq -1112(%rbp), %rax
    movq %rax, -1112(%rbp)
    movq -1112(%rbp), %rdi
    call strlen
    movq %rax, -1112(%rbp)
    movq -1104(%rbp), %rax
    movq -1112(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -1088(%rbp), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1104(%rbp)
    movl -1104(%rbp), %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1104(%rbp)
    movl -1104(%rbp), %eax
    movl %eax, -1104(%rbp)
    movl -1104(%rbp), %eax
    testl %eax, %eax
    jz parseToken_2
parseToken_1:
    leaq -1072(%rbp), %rax
    movq %rax, -1104(%rbp)
    movsd .LC1(%rip), %xmm0
    movsd %xmm0, -1112(%rbp)
    movq -1104(%rbp), %rax
    movsd -1112(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1072(%rbp), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rsi
    movq -1024(%rbp), %rdi
    mov $40, %rdx
    call memcpy
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp parseToken_2
parseToken_2:
    leaq -1032(%rbp), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1104(%rbp)
    movq $0, %rax
    imulq $1, %rax
    movq %rax, -1112(%rbp)
    movq -1104(%rbp), %rax
    addq -1112(%rbp), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movb (%rax), %al
    movb %al, -1104(%rbp)
    movsbl -1104(%rbp), %eax
    movl %eax, -1104(%rbp)
    movl -1104(%rbp), %eax
    cmpl $40, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1104(%rbp)
    movl -1104(%rbp), %eax
    testl %eax, %eax
    jz parseToken_4
parseToken_3:
    leaq -1080(%rbp), %rax
    movq %rax, -1104(%rbp)
    leaq -1032(%rbp), %rax
    movq %rax, -1112(%rbp)
    movq -1112(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1112(%rbp)
    movq $1, %rax
    imulq $1, %rax
    movq %rax, -1120(%rbp)
    movq -1112(%rbp), %rax
    addq -1120(%rbp), %rax
    movq %rax, -1112(%rbp)
    movq -1104(%rbp), %rax
    movq -1112(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -1088(%rbp), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1112(%rbp)
    movq -1112(%rbp), %rax
    subq $1, %rax
    movq %rax, -1112(%rbp)
    movq -1104(%rbp), %rax
    movq -1112(%rbp), %rcx
    movq %rcx, (%rax)
    jmp parseToken_4
parseToken_4:
    leaq -1032(%rbp), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1104(%rbp)
    movq $0, %rax
    imulq $1, %rax
    movq %rax, -1112(%rbp)
    movq -1104(%rbp), %rax
    addq -1112(%rbp), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movb (%rax), %al
    movb %al, -1104(%rbp)
    movsbl -1104(%rbp), %eax
    movl %eax, -1104(%rbp)
    movl -1104(%rbp), %eax
    cmpl $35, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1104(%rbp)
    movl -1104(%rbp), %eax
    testl %eax, %eax
    jz parseToken_6
parseToken_5:
    leaq -1072(%rbp), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rsi
    movq -1024(%rbp), %rdi
    mov $40, %rdx
    call memcpy
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp parseToken_6
parseToken_6:
    leaq -1096(%rbp), %rax
    movq %rax, -1104(%rbp)
    leaq -1032(%rbp), %rax
    movq %rax, -1112(%rbp)
    movq -1112(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1112(%rbp)
    movq -1112(%rbp), %rdi
    call atof
    movsd %xmm0, -1112(%rbp)
    movq -1104(%rbp), %rax
    movsd -1112(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1096(%rbp), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1104(%rbp)
    cvttsd2si -1104(%rbp), %eax
    movl %eax, -1104(%rbp)
    movl -1104(%rbp), %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1104(%rbp)
    movl -1104(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1104(%rbp)
    movl -1104(%rbp), %eax
    testl %eax, %eax
    jz parseToken_8
    leaq -1032(%rbp), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movq %rax, -1104(%rbp)
    leaq .LC2(%rip), %rax
    movq %rax, -1112(%rbp)
    movq -1104(%rbp), %rdi
    movq -1112(%rbp), %rsi
    call strcmp
    movl %eax, -1104(%rbp)
    movl -1104(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1104(%rbp)
    movl -1104(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1104(%rbp)
    movl -1104(%rbp), %eax
    testl %eax, %eax
    jz parseToken_8
parseToken_7:
    leaq .LC3(%rip), %rax
    movq %rax, -1104(%rbp)
    leaq -1032(%rbp), %rax
    movq %rax, -1112(%rbp)
    movq -1112(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1112(%rbp)
    movq -1104(%rbp), %rdi
    movq -1112(%rbp), %rsi
    call printError
    leaq -1072(%rbp), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    addq $8, %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movl $2, %ecx
    movl %ecx, (%rax)
    leaq -1072(%rbp), %rax
    movq %rax, -1104(%rbp)
    movsd .LC4(%rip), %xmm0
    movsd %xmm0, -1112(%rbp)
    movq -1104(%rbp), %rax
    movsd -1112(%rbp), %xmm2
    movsd %xmm2, (%rax)
    jmp parseToken_9
parseToken_8:
    leaq -1072(%rbp), %rax
    movq %rax, -1104(%rbp)
    leaq -1096(%rbp), %rax
    movq %rax, -1112(%rbp)
    movq -1112(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1112(%rbp)
    movq -1104(%rbp), %rax
    movsd -1112(%rbp), %xmm2
    movsd %xmm2, (%rax)
    jmp parseToken_9
parseToken_9:
    leaq -1072(%rbp), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rsi
    movq -1024(%rbp), %rdi
    mov $40, %rdx
    call memcpy
    mov %rbp, %rsp
    pop %rbp
    ret
.global solve_x
solve_x:
    push %rbp
    mov %rsp, %rbp
    subq $1120, %rsp
solve_x_0:
    movq %rdi, -1024(%rbp)
    movq %rsi, -1032(%rbp)
    movq %rdx, -1040(%rbp)
    movb %cl, -1048(%rbp)
    leaq -1032(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    addq $8, %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1096(%rbp)
    movl -1096(%rbp), %eax
    cmpl $2, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1096(%rbp)
    movl -1096(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1096(%rbp)
    movl -1096(%rbp), %eax
    testl %eax, %eax
    leaq -1040(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    addq $8, %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1096(%rbp)
    movl -1096(%rbp), %eax
    cmpl $2, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1096(%rbp)
    movl -1096(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1096(%rbp)
    movl -1096(%rbp), %eax
    testl %eax, %eax
    jz solve_x_2
solve_x_1:
    leaq -1088(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    addq $8, %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movl $2, %ecx
    movl %ecx, (%rax)
    leaq -1088(%rbp), %rax
    movq %rax, -1096(%rbp)
    movsd .LC5(%rip), %xmm0
    movsd %xmm0, -1104(%rbp)
    movq -1096(%rbp), %rax
    movsd -1104(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1088(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rsi
    movq -1024(%rbp), %rdi
    mov $40, %rdx
    call memcpy
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp solve_x_2
solve_x_2:
    leaq -1048(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movb (%rax), %al
    movb %al, -1096(%rbp)
    movsbl -1096(%rbp), %eax
    movl %eax, -1096(%rbp)
    movl -1096(%rbp), %eax
    cmpl $126, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1096(%rbp)
    movl -1096(%rbp), %eax
    testl %eax, %eax
    jz solve_x_4
solve_x_3:
    leaq -1088(%rbp), %rax
    movq %rax, -1096(%rbp)
    leaq -1032(%rbp), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1104(%rbp)
    leaq -1040(%rbp), %rax
    movq %rax, -1112(%rbp)
    movq -1112(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1112(%rbp)
    movq -1112(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1112(%rbp)
    movl -1104(%rbp), %eax
    cmpl -1112(%rbp), %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1104(%rbp)
    movslq -1104(%rbp), %rax
    cvtsi2sd %rax, %xmm0
    movsd %xmm0, -1104(%rbp)
    movq -1096(%rbp), %rax
    movsd -1104(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1088(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    addq $8, %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movl $1, %ecx
    movl %ecx, (%rax)
    jmp solve_x_29
solve_x_4:
    leaq -1048(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movb (%rax), %al
    movb %al, -1096(%rbp)
    movsbl -1096(%rbp), %eax
    movl %eax, -1096(%rbp)
    movl -1096(%rbp), %eax
    cmpl $60, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1096(%rbp)
    movl -1096(%rbp), %eax
    testl %eax, %eax
    jz solve_x_6
solve_x_5:
    leaq -1088(%rbp), %rax
    movq %rax, -1096(%rbp)
    leaq -1032(%rbp), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1104(%rbp)
    leaq -1040(%rbp), %rax
    movq %rax, -1112(%rbp)
    movq -1112(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1112(%rbp)
    movq -1112(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1112(%rbp)
    movl -1104(%rbp), %eax
    cmpl -1112(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -1104(%rbp)
    movslq -1104(%rbp), %rax
    cvtsi2sd %rax, %xmm0
    movsd %xmm0, -1104(%rbp)
    movq -1096(%rbp), %rax
    movsd -1104(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1088(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    addq $8, %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movl $1, %ecx
    movl %ecx, (%rax)
    jmp solve_x_28
solve_x_6:
    leaq -1048(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movb (%rax), %al
    movb %al, -1096(%rbp)
    movsbl -1096(%rbp), %eax
    movl %eax, -1096(%rbp)
    movl -1096(%rbp), %eax
    cmpl $62, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1096(%rbp)
    movl -1096(%rbp), %eax
    testl %eax, %eax
    jz solve_x_8
solve_x_7:
    leaq -1088(%rbp), %rax
    movq %rax, -1096(%rbp)
    leaq -1032(%rbp), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1104(%rbp)
    leaq -1040(%rbp), %rax
    movq %rax, -1112(%rbp)
    movq -1112(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1112(%rbp)
    movq -1112(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1112(%rbp)
    movl -1104(%rbp), %eax
    cmpl -1112(%rbp), %eax
    setg %al
    movzbl %al, %eax
    movl %eax, -1104(%rbp)
    movslq -1104(%rbp), %rax
    cvtsi2sd %rax, %xmm0
    movsd %xmm0, -1104(%rbp)
    movq -1096(%rbp), %rax
    movsd -1104(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1088(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    addq $8, %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movl $1, %ecx
    movl %ecx, (%rax)
    jmp solve_x_27
solve_x_8:
    leaq -1048(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movb (%rax), %al
    movb %al, -1096(%rbp)
    movsbl -1096(%rbp), %eax
    movl %eax, -1096(%rbp)
    movl -1096(%rbp), %eax
    cmpl $94, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1096(%rbp)
    movl -1096(%rbp), %eax
    testl %eax, %eax
    jz solve_x_10
solve_x_9:
    leaq -1088(%rbp), %rax
    movq %rax, -1096(%rbp)
    leaq -1032(%rbp), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1104(%rbp)
    leaq -1040(%rbp), %rax
    movq %rax, -1112(%rbp)
    movq -1112(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1112(%rbp)
    movq -1112(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1112(%rbp)
    movsd -1104(%rbp), %xmm0
    movsd -1112(%rbp), %xmm1
    call my_pow
    movsd %xmm0, -1104(%rbp)
    movq -1096(%rbp), %rax
    movsd -1104(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq .LC6(%rip), %rax
    movq %rax, -1096(%rbp)
    leaq -1088(%rbp), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1104(%rbp)
    subq $176, %rsp
    movq -1096(%rbp), %rdi
    movsd -1104(%rbp), %xmm0
    movl $1, %eax
    call printf
    addq $176, %rsp
    movl %eax, -1096(%rbp)
    leaq -1088(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    addq $8, %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    jmp solve_x_26
solve_x_10:
    leaq -1048(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movb (%rax), %al
    movb %al, -1096(%rbp)
    movsbl -1096(%rbp), %eax
    movl %eax, -1096(%rbp)
    movl -1096(%rbp), %eax
    cmpl $37, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1096(%rbp)
    movl -1096(%rbp), %eax
    testl %eax, %eax
    jz solve_x_12
solve_x_11:
    leaq -1088(%rbp), %rax
    movq %rax, -1096(%rbp)
    leaq -1032(%rbp), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1104(%rbp)
    cvttsd2si -1104(%rbp), %eax
    movl %eax, -1104(%rbp)
    leaq -1040(%rbp), %rax
    movq %rax, -1112(%rbp)
    movq -1112(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1112(%rbp)
    movq -1112(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1112(%rbp)
    cvttsd2si -1112(%rbp), %eax
    movl %eax, -1112(%rbp)
    movl -1104(%rbp), %eax
    cltd
    idivl -1112(%rbp)
    movl %edx, -1104(%rbp)
    movslq -1104(%rbp), %rax
    cvtsi2ss %rax, %xmm0
    movss %xmm0, -1104(%rbp)
    movq -1096(%rbp), %rax
    movsd -1104(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1088(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    addq $8, %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    jmp solve_x_25
solve_x_12:
    leaq -1048(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movb (%rax), %al
    movb %al, -1096(%rbp)
    movsbl -1096(%rbp), %eax
    movl %eax, -1096(%rbp)
    movl -1096(%rbp), %eax
    cmpl $120, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1096(%rbp)
    movl -1096(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1096(%rbp)
    movl -1096(%rbp), %eax
    testl %eax, %eax
    leaq -1048(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movb (%rax), %al
    movb %al, -1096(%rbp)
    movsbl -1096(%rbp), %eax
    movl %eax, -1096(%rbp)
    movl -1096(%rbp), %eax
    cmpl $42, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1096(%rbp)
    movl -1096(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1096(%rbp)
    movl -1096(%rbp), %eax
    testl %eax, %eax
    jz solve_x_14
solve_x_13:
    leaq -1088(%rbp), %rax
    movq %rax, -1096(%rbp)
    leaq -1032(%rbp), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1104(%rbp)
    leaq -1040(%rbp), %rax
    movq %rax, -1112(%rbp)
    movq -1112(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1112(%rbp)
    movq -1112(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1112(%rbp)
    movsd -1104(%rbp), %xmm0
    mulsd -1112(%rbp), %xmm0
    movsd %xmm0, -1104(%rbp)
    movq -1096(%rbp), %rax
    movsd -1104(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1088(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    addq $8, %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    jmp solve_x_24
solve_x_14:
    leaq -1048(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movb (%rax), %al
    movb %al, -1096(%rbp)
    movsbl -1096(%rbp), %eax
    movl %eax, -1096(%rbp)
    movl -1096(%rbp), %eax
    cmpl $47, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1096(%rbp)
    movl -1096(%rbp), %eax
    testl %eax, %eax
    jz solve_x_16
solve_x_15:
    leaq -1088(%rbp), %rax
    movq %rax, -1096(%rbp)
    leaq -1032(%rbp), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1104(%rbp)
    leaq -1040(%rbp), %rax
    movq %rax, -1112(%rbp)
    movq -1112(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1112(%rbp)
    movq -1112(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1112(%rbp)
    movsd -1104(%rbp), %xmm0
    divsd -1112(%rbp), %xmm0
    movsd %xmm0, -1104(%rbp)
    movq -1096(%rbp), %rax
    movsd -1104(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1088(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    addq $8, %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    jmp solve_x_23
solve_x_16:
    leaq -1048(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movb (%rax), %al
    movb %al, -1096(%rbp)
    movsbl -1096(%rbp), %eax
    movl %eax, -1096(%rbp)
    movl -1096(%rbp), %eax
    cmpl $43, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1096(%rbp)
    movl -1096(%rbp), %eax
    testl %eax, %eax
    jz solve_x_18
solve_x_17:
    leaq -1088(%rbp), %rax
    movq %rax, -1096(%rbp)
    leaq -1032(%rbp), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1104(%rbp)
    leaq -1040(%rbp), %rax
    movq %rax, -1112(%rbp)
    movq -1112(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1112(%rbp)
    movq -1112(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1112(%rbp)
    movsd -1104(%rbp), %xmm0
    addsd -1112(%rbp), %xmm0
    movsd %xmm0, -1104(%rbp)
    movq -1096(%rbp), %rax
    movsd -1104(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1088(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    addq $8, %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    jmp solve_x_22
solve_x_18:
    leaq -1048(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movb (%rax), %al
    movb %al, -1096(%rbp)
    movsbl -1096(%rbp), %eax
    movl %eax, -1096(%rbp)
    movl -1096(%rbp), %eax
    cmpl $45, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1096(%rbp)
    movl -1096(%rbp), %eax
    testl %eax, %eax
    jz solve_x_20
solve_x_19:
    leaq -1088(%rbp), %rax
    movq %rax, -1096(%rbp)
    leaq -1032(%rbp), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1104(%rbp)
    leaq -1040(%rbp), %rax
    movq %rax, -1112(%rbp)
    movq -1112(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1112(%rbp)
    movq -1112(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1112(%rbp)
    movsd -1104(%rbp), %xmm0
    subsd -1112(%rbp), %xmm0
    movsd %xmm0, -1104(%rbp)
    movq -1096(%rbp), %rax
    movsd -1104(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1088(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    addq $8, %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    jmp solve_x_21
solve_x_20:
    leaq .LC7(%rip), %rax
    movq %rax, -1096(%rbp)
    subq $176, %rsp
    movq -1096(%rbp), %rdi
    xor %eax, %eax
    call printf
    addq $176, %rsp
    movl %eax, -1096(%rbp)
    leaq -1088(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    addq $8, %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movl $2, %ecx
    movl %ecx, (%rax)
    leaq -1088(%rbp), %rax
    movq %rax, -1096(%rbp)
    movsd .LC8(%rip), %xmm0
    movsd %xmm0, -1104(%rbp)
    movq -1096(%rbp), %rax
    movsd -1104(%rbp), %xmm2
    movsd %xmm2, (%rax)
    jmp solve_x_21
solve_x_21:
solve_x_22:
solve_x_23:
solve_x_24:
solve_x_25:
solve_x_26:
solve_x_27:
solve_x_28:
solve_x_29:
    leaq -1088(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rsi
    movq -1024(%rbp), %rdi
    mov $40, %rdx
    call memcpy
    mov %rbp, %rsp
    pop %rbp
    ret
.global solve
solve:
    push %rbp
    mov %rsp, %rbp
    subq $1184, %rsp
solve_0:
    movq %rdi, -1024(%rbp)
    movq %rsi, %rax
    movq %rax, -1160(%rbp)
    leaq -1064(%rbp), %rax
    movq %rax, -1168(%rbp)
    movq -1160(%rbp), %rsi
    movq -1168(%rbp), %rdi
    mov $40, %rdx
    call memcpy
    movq %rdx, %rax
    movq %rax, -1160(%rbp)
    leaq -1104(%rbp), %rax
    movq %rax, -1168(%rbp)
    movq -1160(%rbp), %rsi
    movq -1168(%rbp), %rdi
    mov $40, %rdx
    call memcpy
    movb %cl, -1112(%rbp)
    leaq .LC9(%rip), %rax
    movq %rax, -1160(%rbp)
    subq $176, %rsp
    movq -1160(%rbp), %rdi
    xor %eax, %eax
    call printf
    addq $176, %rsp
    movl %eax, -1160(%rbp)
    leaq -1064(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    addq $8, %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1160(%rbp)
    movl -1160(%rbp), %eax
    cmpl $2, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1160(%rbp)
    movl -1160(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1160(%rbp)
    movl -1160(%rbp), %eax
    testl %eax, %eax
    leaq -1104(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    addq $8, %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1160(%rbp)
    movl -1160(%rbp), %eax
    cmpl $2, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1160(%rbp)
    movl -1160(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1160(%rbp)
    movl -1160(%rbp), %eax
    testl %eax, %eax
    jz solve_2
solve_1:
    leaq -1152(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    addq $8, %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movl $2, %ecx
    movl %ecx, (%rax)
    leaq -1152(%rbp), %rax
    movq %rax, -1160(%rbp)
    movsd .LC10(%rip), %xmm0
    movsd %xmm0, -1168(%rbp)
    movq -1160(%rbp), %rax
    movsd -1168(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1152(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rsi
    movq -1024(%rbp), %rdi
    mov $40, %rdx
    call memcpy
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp solve_2
solve_2:
    leaq .LC11(%rip), %rax
    movq %rax, -1160(%rbp)
    subq $176, %rsp
    movq -1160(%rbp), %rdi
    xor %eax, %eax
    call printf
    addq $176, %rsp
    movl %eax, -1160(%rbp)
    leaq -1112(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movb (%rax), %al
    movb %al, -1160(%rbp)
    movl -1160(%rbp), %eax
    cmpl $126, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1168(%rbp)
    movl -1168(%rbp), %eax
    testl %eax, %eax
    jz solve_3
    movl -1160(%rbp), %eax
    cmpl $60, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1168(%rbp)
    movl -1168(%rbp), %eax
    testl %eax, %eax
    jz solve_4
    movl -1160(%rbp), %eax
    cmpl $62, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1168(%rbp)
    movl -1168(%rbp), %eax
    testl %eax, %eax
    jz solve_5
    movl -1160(%rbp), %eax
    cmpl $94, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1168(%rbp)
    movl -1168(%rbp), %eax
    testl %eax, %eax
    jz solve_6
    movl -1160(%rbp), %eax
    cmpl $37, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1168(%rbp)
    movl -1168(%rbp), %eax
    testl %eax, %eax
    jz solve_7
    movl -1160(%rbp), %eax
    cmpl $42, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1168(%rbp)
    movl -1168(%rbp), %eax
    testl %eax, %eax
    jz solve_8
    movl -1160(%rbp), %eax
    cmpl $47, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1168(%rbp)
    movl -1168(%rbp), %eax
    testl %eax, %eax
    jz solve_9
    movl -1160(%rbp), %eax
    cmpl $43, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1168(%rbp)
    movl -1168(%rbp), %eax
    testl %eax, %eax
    jz solve_10
    movl -1160(%rbp), %eax
    cmpl $45, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1168(%rbp)
    movl -1168(%rbp), %eax
    testl %eax, %eax
    jz solve_11
    movl -1160(%rbp), %eax
    cmpl $120, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1160(%rbp)
    movl -1160(%rbp), %eax
    testl %eax, %eax
    jz solve_12
    jmp solve_13
solve_3:
    leaq -1152(%rbp), %rax
    movq %rax, -1160(%rbp)
    leaq -1064(%rbp), %rax
    movq %rax, -1168(%rbp)
    movq -1168(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1168(%rbp)
    leaq -1104(%rbp), %rax
    movq %rax, -1176(%rbp)
    movq -1176(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1176(%rbp)
    movl -1168(%rbp), %eax
    cmpl -1176(%rbp), %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1168(%rbp)
    movslq -1168(%rbp), %rax
    cvtsi2sd %rax, %xmm0
    movsd %xmm0, -1168(%rbp)
    movq -1160(%rbp), %rax
    movsd -1168(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1152(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    addq $8, %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movl $1, %ecx
    movl %ecx, (%rax)
    jmp solve_14
solve_4:
    leaq -1152(%rbp), %rax
    movq %rax, -1160(%rbp)
    leaq -1064(%rbp), %rax
    movq %rax, -1168(%rbp)
    movq -1168(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1168(%rbp)
    leaq -1104(%rbp), %rax
    movq %rax, -1176(%rbp)
    movq -1176(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1176(%rbp)
    movl -1168(%rbp), %eax
    cmpl -1176(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -1168(%rbp)
    movslq -1168(%rbp), %rax
    cvtsi2sd %rax, %xmm0
    movsd %xmm0, -1168(%rbp)
    movq -1160(%rbp), %rax
    movsd -1168(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1152(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    addq $8, %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movl $1, %ecx
    movl %ecx, (%rax)
    jmp solve_14
solve_5:
    leaq -1152(%rbp), %rax
    movq %rax, -1160(%rbp)
    leaq -1064(%rbp), %rax
    movq %rax, -1168(%rbp)
    movq -1168(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1168(%rbp)
    leaq -1104(%rbp), %rax
    movq %rax, -1176(%rbp)
    movq -1176(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1176(%rbp)
    movl -1168(%rbp), %eax
    cmpl -1176(%rbp), %eax
    setg %al
    movzbl %al, %eax
    movl %eax, -1168(%rbp)
    movslq -1168(%rbp), %rax
    cvtsi2sd %rax, %xmm0
    movsd %xmm0, -1168(%rbp)
    movq -1160(%rbp), %rax
    movsd -1168(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1152(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    addq $8, %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movl $1, %ecx
    movl %ecx, (%rax)
    jmp solve_14
solve_6:
    leaq -1152(%rbp), %rax
    movq %rax, -1160(%rbp)
    leaq -1064(%rbp), %rax
    movq %rax, -1168(%rbp)
    movq -1168(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1168(%rbp)
    leaq -1104(%rbp), %rax
    movq %rax, -1176(%rbp)
    movq -1176(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1176(%rbp)
    movsd -1168(%rbp), %xmm0
    movsd -1176(%rbp), %xmm1
    call my_pow
    movsd %xmm0, -1168(%rbp)
    movq -1160(%rbp), %rax
    movsd -1168(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1152(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    addq $8, %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    jmp solve_14
solve_7:
    leaq -1152(%rbp), %rax
    movq %rax, -1160(%rbp)
    leaq -1064(%rbp), %rax
    movq %rax, -1168(%rbp)
    movq -1168(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1168(%rbp)
    cvttsd2si -1168(%rbp), %eax
    movl %eax, -1168(%rbp)
    leaq -1104(%rbp), %rax
    movq %rax, -1176(%rbp)
    movq -1176(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1176(%rbp)
    cvttsd2si -1176(%rbp), %eax
    movl %eax, -1176(%rbp)
    movl -1168(%rbp), %eax
    cltd
    idivl -1176(%rbp)
    movl %edx, -1168(%rbp)
    movslq -1168(%rbp), %rax
    cvtsi2ss %rax, %xmm0
    movss %xmm0, -1168(%rbp)
    movq -1160(%rbp), %rax
    movsd -1168(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1152(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    addq $8, %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    jmp solve_14
solve_8:
    leaq -1152(%rbp), %rax
    movq %rax, -1160(%rbp)
    leaq -1064(%rbp), %rax
    movq %rax, -1168(%rbp)
    movq -1168(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1168(%rbp)
    leaq -1104(%rbp), %rax
    movq %rax, -1176(%rbp)
    movq -1176(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1176(%rbp)
    movsd -1168(%rbp), %xmm0
    mulsd -1176(%rbp), %xmm0
    movsd %xmm0, -1168(%rbp)
    movq -1160(%rbp), %rax
    movsd -1168(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1152(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    addq $8, %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    jmp solve_14
solve_9:
    leaq -1152(%rbp), %rax
    movq %rax, -1160(%rbp)
    leaq -1064(%rbp), %rax
    movq %rax, -1168(%rbp)
    movq -1168(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1168(%rbp)
    leaq -1104(%rbp), %rax
    movq %rax, -1176(%rbp)
    movq -1176(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1176(%rbp)
    movsd -1168(%rbp), %xmm0
    divsd -1176(%rbp), %xmm0
    movsd %xmm0, -1168(%rbp)
    movq -1160(%rbp), %rax
    movsd -1168(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1152(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    addq $8, %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    jmp solve_14
solve_10:
    leaq .LC12(%rip), %rax
    movq %rax, -1160(%rbp)
    subq $176, %rsp
    movq -1160(%rbp), %rdi
    xor %eax, %eax
    call printf
    addq $176, %rsp
    movl %eax, -1160(%rbp)
    leaq -1152(%rbp), %rax
    movq %rax, -1160(%rbp)
    leaq -1064(%rbp), %rax
    movq %rax, -1168(%rbp)
    movq -1168(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1168(%rbp)
    leaq -1104(%rbp), %rax
    movq %rax, -1176(%rbp)
    movq -1176(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1176(%rbp)
    movsd -1168(%rbp), %xmm0
    addsd -1176(%rbp), %xmm0
    movsd %xmm0, -1168(%rbp)
    movq -1160(%rbp), %rax
    movsd -1168(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1152(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    addq $8, %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    jmp solve_14
solve_11:
    leaq -1152(%rbp), %rax
    movq %rax, -1160(%rbp)
    leaq -1064(%rbp), %rax
    movq %rax, -1168(%rbp)
    movq -1168(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1168(%rbp)
    leaq -1104(%rbp), %rax
    movq %rax, -1176(%rbp)
    movq -1176(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1176(%rbp)
    movsd -1168(%rbp), %xmm0
    subsd -1176(%rbp), %xmm0
    movsd %xmm0, -1168(%rbp)
    movq -1160(%rbp), %rax
    movsd -1168(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1152(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    addq $8, %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    jmp solve_14
solve_12:
solve_13:
    leaq .LC13(%rip), %rax
    movq %rax, -1160(%rbp)
    subq $176, %rsp
    movq -1160(%rbp), %rdi
    xor %eax, %eax
    call printf
    addq $176, %rsp
    movl %eax, -1160(%rbp)
    leaq -1152(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    addq $8, %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movl $2, %ecx
    movl %ecx, (%rax)
    leaq -1152(%rbp), %rax
    movq %rax, -1160(%rbp)
    movsd .LC14(%rip), %xmm0
    movsd %xmm0, -1168(%rbp)
    movq -1160(%rbp), %rax
    movsd -1168(%rbp), %xmm2
    movsd %xmm2, (%rax)
solve_14:
    leaq .LC15(%rip), %rax
    movq %rax, -1160(%rbp)
    subq $176, %rsp
    movq -1160(%rbp), %rdi
    xor %eax, %eax
    call printf
    addq $176, %rsp
    movl %eax, -1160(%rbp)
    leaq -1152(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rsi
    movq -1024(%rbp), %rdi
    mov $40, %rdx
    call memcpy
    mov %rbp, %rsp
    pop %rbp
    ret
.global parse
parse:
    push %rbp
    mov %rsp, %rbp
    subq $1696, %rsp
parse_0:
    movq %rdi, -1024(%rbp)
    movq %rsi, -1032(%rbp)
    movl %edx, -1040(%rbp)
    movq %rcx, -1048(%rbp)
    leaq -1088(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    addq $8, %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    leaq -1088(%rbp), %rax
    movq %rax, -1664(%rbp)
    movsd .LC16(%rip), %xmm0
    movsd %xmm0, -1672(%rbp)
    movq -1664(%rbp), %rax
    movsd -1672(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1096(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movl $-1, %ecx
    movl %ecx, (%rax)
    leaq -1104(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    leaq -1112(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movl $-1, %ecx
    movl %ecx, (%rax)
    leaq -1120(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    leaq -1128(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
parse_1:
    leaq -1128(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1664(%rbp)
    leaq -1040(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1672(%rbp)
    movl -1672(%rbp), %eax
    addl $1, %eax
    movl %eax, -1672(%rbp)
    movl -1664(%rbp), %eax
    cmpl -1672(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    testl %eax, %eax
    jz parse_25
parse_2:
    leaq -1032(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1664(%rbp)
    leaq -1128(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1672(%rbp)
    movslq -1672(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1672(%rbp)
    movq -1664(%rbp), %rax
    addq -1672(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movb (%rax), %al
    movb %al, -1664(%rbp)
    movsbl -1664(%rbp), %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $40, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    testl %eax, %eax
    jz parse_4
parse_3:
    leaq -1120(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1672(%rbp)
    movl -1672(%rbp), %eax
    addl $1, %eax
    movl %eax, -1672(%rbp)
    movq -1664(%rbp), %rax
    movl -1672(%rbp), %ecx
    movl %ecx, (%rax)
    jmp parse_7
parse_4:
    leaq -1032(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1664(%rbp)
    leaq -1128(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1672(%rbp)
    movslq -1672(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1672(%rbp)
    movq -1664(%rbp), %rax
    addq -1672(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movb (%rax), %al
    movb %al, -1664(%rbp)
    movsbl -1664(%rbp), %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $41, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    testl %eax, %eax
    jz parse_6
    leaq -1120(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    testl %eax, %eax
    jz parse_6
parse_5:
    leaq -1120(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1672(%rbp)
    movl -1672(%rbp), %eax
    subl $1, %eax
    movl %eax, -1672(%rbp)
    movq -1664(%rbp), %rax
    movl -1672(%rbp), %ecx
    movl %ecx, (%rax)
    jmp parse_6
parse_6:
parse_7:
    leaq -1120(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    testl %eax, %eax
    jz parse_21
parse_8:
    leaq -1032(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1664(%rbp)
    leaq -1128(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1672(%rbp)
    movslq -1672(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1672(%rbp)
    movq -1664(%rbp), %rax
    addq -1672(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movb (%rax), %al
    movb %al, -1664(%rbp)
    movb -1664(%rbp), %dil
    call is_operator
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    testl %eax, %eax
    jz parse_14
    leaq -1048(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    testl %eax, %eax
    jz parse_14
parse_9:
    leaq -1136(%rbp), %rax
    movq %rax, -1664(%rbp)
    leaq -1032(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1672(%rbp)
    leaq -1128(%rbp), %rax
    movq %rax, -1680(%rbp)
    movq -1680(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1680(%rbp)
    movslq -1680(%rbp), %rax
    movq %rax, -1680(%rbp)
    movq -1680(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1680(%rbp)
    movq -1672(%rbp), %rax
    addq -1680(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movb (%rax), %al
    movb %al, -1672(%rbp)
    movb -1672(%rbp), %dil
    call op_priority
    movl %eax, -1672(%rbp)
    movq -1664(%rbp), %rax
    movl -1672(%rbp), %ecx
    movl %ecx, (%rax)
    leaq -1144(%rbp), %rax
    movq %rax, -1664(%rbp)
    leaq -1104(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movb (%rax), %al
    movb %al, -1672(%rbp)
    movb -1672(%rbp), %dil
    call op_priority
    movl %eax, -1672(%rbp)
    movq -1664(%rbp), %rax
    movl -1672(%rbp), %ecx
    movl %ecx, (%rax)
    leaq -1104(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movb (%rax), %al
    movb %al, -1664(%rbp)
    movsbl -1664(%rbp), %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $94, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    testl %eax, %eax
    leaq -1032(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1664(%rbp)
    leaq -1128(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1672(%rbp)
    movslq -1672(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1672(%rbp)
    movq -1664(%rbp), %rax
    addq -1672(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movb (%rax), %al
    movb %al, -1664(%rbp)
    movsbl -1664(%rbp), %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $94, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    testl %eax, %eax
    jz parse_13
parse_10:
    leaq -1096(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $-1, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    testl %eax, %eax
    leaq -1136(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1664(%rbp)
    leaq -1144(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1672(%rbp)
    movl -1664(%rbp), %eax
    cmpl -1672(%rbp), %eax
    setle %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    testl %eax, %eax
    jz parse_12
parse_11:
    leaq -1096(%rbp), %rax
    movq %rax, -1664(%rbp)
    leaq -1128(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1672(%rbp)
    movq -1664(%rbp), %rax
    movl -1672(%rbp), %ecx
    movl %ecx, (%rax)
    leaq -1104(%rbp), %rax
    movq %rax, -1664(%rbp)
    leaq -1032(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1672(%rbp)
    leaq -1128(%rbp), %rax
    movq %rax, -1680(%rbp)
    movq -1680(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1680(%rbp)
    movslq -1680(%rbp), %rax
    movq %rax, -1680(%rbp)
    movq -1680(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1680(%rbp)
    movq -1672(%rbp), %rax
    addq -1680(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movb (%rax), %al
    movb %al, -1672(%rbp)
    movq -1664(%rbp), %rax
    movb -1672(%rbp), %cl
    movb %cl, (%rax)
    jmp parse_12
parse_12:
    jmp parse_13
parse_13:
    jmp parse_20
parse_14:
    leaq -1032(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1664(%rbp)
    leaq -1128(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1672(%rbp)
    movslq -1672(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1672(%rbp)
    movq -1664(%rbp), %rax
    addq -1672(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movb (%rax), %al
    movb %al, -1664(%rbp)
    movsbl -1664(%rbp), %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $61, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    testl %eax, %eax
    jz parse_19
parse_15:
    leaq -1048(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $1, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    testl %eax, %eax
    jz parse_17
parse_16:
    leaq .LC17(%rip), %rax
    movq %rax, -1664(%rbp)
    leaq .LC18(%rip), %rax
    movq %rax, -1672(%rbp)
    movq -1664(%rbp), %rdi
    movq -1672(%rbp), %rsi
    call printError
    movl $0, %edi
    call exit
    jmp parse_18
parse_17:
    leaq -1048(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movl $1, %ecx
    movl %ecx, (%rax)
    leaq -1096(%rbp), %rax
    movq %rax, -1664(%rbp)
    leaq -1128(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1672(%rbp)
    movq -1664(%rbp), %rax
    movl -1672(%rbp), %ecx
    movl %ecx, (%rax)
    leaq -1104(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movb $61, %cl
    movb %cl, (%rax)
    jmp parse_18
parse_18:
    jmp parse_19
parse_19:
parse_20:
    jmp parse_21
parse_21:
    leaq -1032(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1664(%rbp)
    leaq -1128(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1672(%rbp)
    movslq -1672(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1672(%rbp)
    movq -1664(%rbp), %rax
    addq -1672(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movb (%rax), %al
    movb %al, -1664(%rbp)
    movsbl -1664(%rbp), %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    testl %eax, %eax
    jz parse_23
parse_22:
    leaq -1112(%rbp), %rax
    movq %rax, -1664(%rbp)
    leaq -1128(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1672(%rbp)
    movq -1664(%rbp), %rax
    movl -1672(%rbp), %ecx
    movl %ecx, (%rax)
    jmp parse_25
    jmp parse_23
parse_23:
    jmp parse_24
parse_24:
    leaq -1128(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1672(%rbp)
    movl -1672(%rbp), %eax
    addl $1, %eax
    movl %eax, -1672(%rbp)
    movq -1664(%rbp), %rax
    movl -1672(%rbp), %ecx
    movl %ecx, (%rax)
    jmp parse_1
parse_25:
    leaq -1112(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $-1, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    testl %eax, %eax
    jz parse_27
parse_26:
    leaq .LC19(%rip), %rax
    movq %rax, -1664(%rbp)
    leaq .LC20(%rip), %rax
    movq %rax, -1672(%rbp)
    movq -1664(%rbp), %rdi
    movq -1672(%rbp), %rsi
    call printError
    movl $1, %edi
    call exit
    jmp parse_27
parse_27:
    leaq -1120(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $0, %eax
    setg %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    testl %eax, %eax
    jz parse_29
parse_28:
    leaq .LC21(%rip), %rax
    movq %rax, -1664(%rbp)
    leaq .LC22(%rip), %rax
    movq %rax, -1672(%rbp)
    movq -1664(%rbp), %rdi
    movq -1672(%rbp), %rsi
    call printError
    movl $1, %edi
    call exit
    jmp parse_29
parse_29:
    leaq -1096(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $-1, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    testl %eax, %eax
    jz parse_31
    leaq -1032(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1664(%rbp)
    movq $0, %rax
    imulq $1, %rax
    movq %rax, -1672(%rbp)
    movq -1664(%rbp), %rax
    addq -1672(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movb (%rax), %al
    movb %al, -1664(%rbp)
    movsbl -1664(%rbp), %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $40, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    testl %eax, %eax
    jz parse_31
    leaq -1032(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1664(%rbp)
    leaq -1112(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1672(%rbp)
    movl -1672(%rbp), %eax
    subl $1, %eax
    movl %eax, -1672(%rbp)
    movslq -1672(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1672(%rbp)
    movq -1664(%rbp), %rax
    addq -1672(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movb (%rax), %al
    movb %al, -1664(%rbp)
    movsbl -1664(%rbp), %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $41, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    testl %eax, %eax
    jz parse_31
parse_30:
    leaq -1032(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1664(%rbp)
    leaq -1112(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1672(%rbp)
    movl -1672(%rbp), %eax
    subl $1, %eax
    movl %eax, -1672(%rbp)
    movslq -1672(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1672(%rbp)
    movq -1664(%rbp), %rax
    addq -1672(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    leaq -1184(%rbp), %rax
    movq %rax, -1664(%rbp)
    leaq -1032(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1672(%rbp)
    movq $1, %rax
    imulq $1, %rax
    movq %rax, -1680(%rbp)
    movq -1672(%rbp), %rax
    addq -1680(%rbp), %rax
    movq %rax, -1672(%rbp)
    leaq -1112(%rbp), %rax
    movq %rax, -1680(%rbp)
    movq -1680(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1680(%rbp)
    movl -1680(%rbp), %eax
    subl $1, %eax
    movl %eax, -1680(%rbp)
    leaq -1048(%rbp), %rax
    movq %rax, -1688(%rbp)
    movq -1688(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1688(%rbp)
    movq -1664(%rbp), %rdi
    movq -1672(%rbp), %rsi
    movl -1680(%rbp), %edx
    movq -1688(%rbp), %rcx
    call parse
    movq -1664(%rbp), %rsi
    movq -1024(%rbp), %rdi
    mov $40, %rdx
    call memcpy
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp parse_31
parse_31:
    leaq -1096(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $-1, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    testl %eax, %eax
    jz parse_33
parse_32:
    leaq -1224(%rbp), %rax
    movq %rax, -1664(%rbp)
    leaq -1184(%rbp), %rax
    movq %rax, -1672(%rbp)
    leaq -1032(%rbp), %rax
    movq %rax, -1680(%rbp)
    movq -1680(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1680(%rbp)
    movq -1672(%rbp), %rdi
    movq -1680(%rbp), %rsi
    call parseToken
    movq -1672(%rbp), %rsi
    movq -1664(%rbp), %rdi
    mov $40, %rdx
    call memcpy
    leaq -1224(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rsi
    movq -1024(%rbp), %rdi
    mov $40, %rdx
    call memcpy
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp parse_48
parse_33:
    leaq -1272(%rbp), %rax
    movq %rax, -1664(%rbp)
    leaq -1032(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1672(%rbp)
    leaq -1096(%rbp), %rax
    movq %rax, -1680(%rbp)
    movq -1680(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1680(%rbp)
    movslq -1680(%rbp), %rax
    movq %rax, -1680(%rbp)
    movq -1680(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1680(%rbp)
    movq -1672(%rbp), %rax
    addq -1680(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movb (%rax), %al
    movb %al, -1672(%rbp)
    movq -1664(%rbp), %rax
    movb -1672(%rbp), %cl
    movb %cl, (%rax)
    leaq -1280(%rbp), %rax
    movq %rax, -1664(%rbp)
    leaq -1096(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1672(%rbp)
    movslq -1672(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1664(%rbp), %rax
    movq -1672(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -1288(%rbp), %rax
    movq %rax, -1664(%rbp)
    leaq -1280(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1672(%rbp)
    movq $1, %rax
    mulq -1672(%rbp)
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rdi
    call malloc
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1664(%rbp), %rax
    movq -1672(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -1288(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movq %rax, -1664(%rbp)
    leaq -1032(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movq %rax, -1672(%rbp)
    leaq -1280(%rbp), %rax
    movq %rax, -1680(%rbp)
    movq -1680(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1680(%rbp)
    movq $1, %rax
    mulq -1680(%rbp)
    movq %rax, -1680(%rbp)
    movq -1664(%rbp), %rdi
    movq -1672(%rbp), %rsi
    movq -1680(%rbp), %rdx
    call memcpy
    movq %rax, -1664(%rbp)
    leaq -1288(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1664(%rbp)
    leaq -1280(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1672(%rbp)
    movq -1664(%rbp), %rax
    addq -1672(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    leaq -1296(%rbp), %rax
    movq %rax, -1664(%rbp)
    leaq -1112(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1672(%rbp)
    leaq -1096(%rbp), %rax
    movq %rax, -1680(%rbp)
    movq -1680(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1680(%rbp)
    movl -1672(%rbp), %eax
    subl -1680(%rbp), %eax
    movl %eax, -1672(%rbp)
    movl -1672(%rbp), %eax
    subl $1, %eax
    movl %eax, -1672(%rbp)
    movslq -1672(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1664(%rbp), %rax
    movq -1672(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -1304(%rbp), %rax
    movq %rax, -1664(%rbp)
    leaq -1296(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1672(%rbp)
    movq $1, %rax
    mulq -1672(%rbp)
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rdi
    call malloc
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1664(%rbp), %rax
    movq -1672(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -1304(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1664(%rbp)
    leaq -1296(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1672(%rbp)
    movq -1664(%rbp), %rax
    addq -1672(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    leaq -1304(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movq %rax, -1664(%rbp)
    leaq -1032(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1672(%rbp)
    leaq -1096(%rbp), %rax
    movq %rax, -1680(%rbp)
    movq -1680(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1680(%rbp)
    movslq -1680(%rbp), %rax
    movq %rax, -1680(%rbp)
    movq -1680(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1680(%rbp)
    movq -1672(%rbp), %rax
    addq -1680(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq $1, %rax
    imulq $1, %rax
    movq %rax, -1680(%rbp)
    movq -1672(%rbp), %rax
    addq -1680(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movq %rax, -1672(%rbp)
    leaq -1296(%rbp), %rax
    movq %rax, -1680(%rbp)
    movq -1680(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1680(%rbp)
    movq $1, %rax
    mulq -1680(%rbp)
    movq %rax, -1680(%rbp)
    movq -1664(%rbp), %rdi
    movq -1672(%rbp), %rsi
    movq -1680(%rbp), %rdx
    call memcpy
    movq %rax, -1664(%rbp)
    leaq -1048(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $1, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    testl %eax, %eax
    jz parse_47
parse_34:
    leaq -1312(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    leaq -1320(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
parse_35:
    leaq -1320(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1664(%rbp)
    leaq -1016(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1672(%rbp)
    movl -1664(%rbp), %eax
    cmpl -1672(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    testl %eax, %eax
    jz parse_40
parse_36:
    leaq -208(%rbp), %rax
    movq %rax, -1664(%rbp)
    leaq -1320(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1672(%rbp)
    movslq -1672(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    imulq $8, %rax
    movq %rax, -1672(%rbp)
    movq -1664(%rbp), %rax
    addq -1672(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movq %rax, -1664(%rbp)
    leaq -1288(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1664(%rbp), %rdi
    movq -1672(%rbp), %rsi
    call strcmp
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    testl %eax, %eax
    jz parse_38
parse_37:
    leaq -1312(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movl $1, %ecx
    movl %ecx, (%rax)
    leaq -1008(%rbp), %rax
    movq %rax, -1664(%rbp)
    leaq -1320(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1672(%rbp)
    movslq -1672(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    imulq $40, %rax
    movq %rax, -1672(%rbp)
    movq -1664(%rbp), %rax
    addq -1672(%rbp), %rax
    movq %rax, -1664(%rbp)
    leaq -1184(%rbp), %rax
    movq %rax, -1672(%rbp)
    leaq -1304(%rbp), %rax
    movq %rax, -1680(%rbp)
    movq -1680(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1680(%rbp)
    leaq -1296(%rbp), %rax
    movq %rax, -1688(%rbp)
    movq -1688(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1688(%rbp)
    movq -1688(%rbp), %rax
    movl %eax, -1688(%rbp)
    leaq -1048(%rbp), %rax
    movq %rax, -1696(%rbp)
    movq -1696(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1696(%rbp)
    movq -1672(%rbp), %rdi
    movq -1680(%rbp), %rsi
    movl -1688(%rbp), %edx
    movq -1696(%rbp), %rcx
    call parse
    movq -1672(%rbp), %rsi
    movq -1664(%rbp), %rdi
    mov $40, %rdx
    call memcpy
    leaq -1088(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rsi
    movq -1024(%rbp), %rdi
    mov $40, %rdx
    call memcpy
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp parse_38
parse_38:
    jmp parse_39
parse_39:
    leaq -1320(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1672(%rbp)
    movl -1672(%rbp), %eax
    addl $1, %eax
    movl %eax, -1672(%rbp)
    movq -1664(%rbp), %rax
    movl -1672(%rbp), %ecx
    movl %ecx, (%rax)
    jmp parse_35
parse_40:
    leaq -1312(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    testl %eax, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    testl %eax, %eax
    jz parse_46
parse_41:
    leaq -1288(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1664(%rbp)
    movq $0, %rax
    imulq $1, %rax
    movq %rax, -1672(%rbp)
    movq -1664(%rbp), %rax
    addq -1672(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movb (%rax), %al
    movb %al, -1664(%rbp)
    movb -1664(%rbp), %dil
    call isAlpha
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    testl %eax, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    testl %eax, %eax
    jz parse_43
parse_42:
    leaq .LC23(%rip), %rax
    movq %rax, -1664(%rbp)
    leaq -1288(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1672(%rbp)
    movq -1664(%rbp), %rdi
    movq -1672(%rbp), %rsi
    call printError
    movl $1, %edi
    call exit
    jmp parse_43
parse_43:
    leaq -1368(%rbp), %rax
    movq %rax, -1664(%rbp)
    leaq -1280(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1672(%rbp)
    movq $1, %rax
    mulq -1672(%rbp)
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    addq $1, %rax
    movq %rax, -1672(%rbp)
    movq -1664(%rbp), %rax
    movq -1672(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -1376(%rbp), %rax
    movq %rax, -1664(%rbp)
    leaq -1368(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rdi
    call malloc
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1664(%rbp), %rax
    movq -1672(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -1376(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1664(%rbp)
    movl -1664(%rbp), %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1664(%rbp)
    movl -1664(%rbp), %eax
    testl %eax, %eax
    jz parse_45
parse_44:
    movl $1, %edi
    call exit
    jmp parse_45
parse_45:
    leaq -1376(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movq %rax, -1664(%rbp)
    leaq -1288(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movq %rax, -1672(%rbp)
    leaq -1368(%rbp), %rax
    movq %rax, -1680(%rbp)
    movq -1680(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1680(%rbp)
    movq -1664(%rbp), %rdi
    movq -1672(%rbp), %rsi
    movq -1680(%rbp), %rdx
    call memcpy
    movq %rax, -1664(%rbp)
    leaq -208(%rbp), %rax
    movq %rax, -1664(%rbp)
    leaq -1016(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1672(%rbp)
    movslq -1672(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    imulq $8, %rax
    movq %rax, -1672(%rbp)
    movq -1664(%rbp), %rax
    addq -1672(%rbp), %rax
    movq %rax, -1664(%rbp)
    leaq -1376(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1672(%rbp)
    movq -1664(%rbp), %rax
    movq -1672(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -1008(%rbp), %rax
    movq %rax, -1664(%rbp)
    leaq -1016(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1672(%rbp)
    movslq -1672(%rbp), %rax
    movq %rax, -1672(%rbp)
    movq -1672(%rbp), %rax
    imulq $40, %rax
    movq %rax, -1672(%rbp)
    movq -1664(%rbp), %rax
    addq -1672(%rbp), %rax
    movq %rax, -1664(%rbp)
    leaq -1184(%rbp), %rax
    movq %rax, -1672(%rbp)
    leaq -1304(%rbp), %rax
    movq %rax, -1680(%rbp)
    movq -1680(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1680(%rbp)
    leaq -1296(%rbp), %rax
    movq %rax, -1688(%rbp)
    movq -1688(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1688(%rbp)
    movq -1688(%rbp), %rax
    movl %eax, -1688(%rbp)
    leaq -1048(%rbp), %rax
    movq %rax, -1696(%rbp)
    movq -1696(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1696(%rbp)
    movq -1672(%rbp), %rdi
    movq -1680(%rbp), %rsi
    movl -1688(%rbp), %edx
    movq -1696(%rbp), %rcx
    call parse
    movq -1672(%rbp), %rsi
    movq -1664(%rbp), %rdi
    mov $40, %rdx
    call memcpy
    leaq -1016(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1672(%rbp)
    movl -1672(%rbp), %eax
    addl $1, %eax
    movl %eax, -1672(%rbp)
    movq -1664(%rbp), %rax
    movl -1672(%rbp), %ecx
    movl %ecx, (%rax)
    leaq -1088(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rsi
    movq -1024(%rbp), %rdi
    mov $40, %rdx
    call memcpy
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp parse_46
parse_46:
    jmp parse_47
parse_47:
    leaq -1456(%rbp), %rax
    movq %rax, -1664(%rbp)
    leaq -1184(%rbp), %rax
    movq %rax, -1672(%rbp)
    leaq -1288(%rbp), %rax
    movq %rax, -1680(%rbp)
    movq -1680(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1680(%rbp)
    leaq -1280(%rbp), %rax
    movq %rax, -1688(%rbp)
    movq -1688(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1688(%rbp)
    movq -1688(%rbp), %rax
    movl %eax, -1688(%rbp)
    leaq -1048(%rbp), %rax
    movq %rax, -1696(%rbp)
    movq -1696(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1696(%rbp)
    movq -1672(%rbp), %rdi
    movq -1680(%rbp), %rsi
    movl -1688(%rbp), %edx
    movq -1696(%rbp), %rcx
    call parse
    movq -1672(%rbp), %rsi
    movq -1664(%rbp), %rdi
    mov $40, %rdx
    call memcpy
    leaq -1536(%rbp), %rax
    movq %rax, -1664(%rbp)
    leaq -1184(%rbp), %rax
    movq %rax, -1672(%rbp)
    leaq -1304(%rbp), %rax
    movq %rax, -1680(%rbp)
    movq -1680(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1680(%rbp)
    leaq -1296(%rbp), %rax
    movq %rax, -1688(%rbp)
    movq -1688(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1688(%rbp)
    movq -1688(%rbp), %rax
    movl %eax, -1688(%rbp)
    leaq -1048(%rbp), %rax
    movq %rax, -1696(%rbp)
    movq -1696(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1696(%rbp)
    movq -1672(%rbp), %rdi
    movq -1680(%rbp), %rsi
    movl -1688(%rbp), %edx
    movq -1696(%rbp), %rcx
    call parse
    movq -1672(%rbp), %rsi
    movq -1664(%rbp), %rdi
    mov $40, %rdx
    call memcpy
    leaq -1616(%rbp), %rax
    movq %rax, -1664(%rbp)
    leaq -1184(%rbp), %rax
    movq %rax, -1672(%rbp)
    leaq -1456(%rbp), %rax
    movq %rax, -1680(%rbp)
    leaq -1536(%rbp), %rax
    movq %rax, -1688(%rbp)
    leaq -1272(%rbp), %rax
    movq %rax, -1696(%rbp)
    movq -1696(%rbp), %rax
    movb (%rax), %al
    movb %al, -1696(%rbp)
    movq -1672(%rbp), %rdi
    movq -1680(%rbp), %rsi
    movq -1688(%rbp), %rdx
    movb -1696(%rbp), %cl
    call solve_x
    movq -1672(%rbp), %rsi
    movq -1664(%rbp), %rdi
    mov $40, %rdx
    call memcpy
    leaq -1288(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rdi
    call free
    leaq -1304(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rdi
    call free
    leaq -1616(%rbp), %rax
    movq %rax, -1664(%rbp)
    movq -1664(%rbp), %rsi
    movq -1024(%rbp), %rdi
    mov $40, %rdx
    call memcpy
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp parse_48
parse_48:
.global op_priority
op_priority:
    push %rbp
    mov %rsp, %rbp
    subq $1040, %rsp
op_priority_0:
    movb %dil, -1024(%rbp)
    leaq -1024(%rbp), %rax
    movq %rax, -1032(%rbp)
    movq -1032(%rbp), %rax
    movb (%rax), %al
    movb %al, -1032(%rbp)
    movl -1032(%rbp), %eax
    cmpl $62, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1040(%rbp)
    movl -1040(%rbp), %eax
    testl %eax, %eax
    jz op_priority_1
    movl -1032(%rbp), %eax
    cmpl $60, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1040(%rbp)
    movl -1040(%rbp), %eax
    testl %eax, %eax
    jz op_priority_2
    movl -1032(%rbp), %eax
    cmpl $126, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1040(%rbp)
    movl -1040(%rbp), %eax
    testl %eax, %eax
    jz op_priority_3
    movl -1032(%rbp), %eax
    cmpl $43, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1040(%rbp)
    movl -1040(%rbp), %eax
    testl %eax, %eax
    jz op_priority_4
    movl -1032(%rbp), %eax
    cmpl $45, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1040(%rbp)
    movl -1040(%rbp), %eax
    testl %eax, %eax
    jz op_priority_5
    movl -1032(%rbp), %eax
    cmpl $42, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1040(%rbp)
    movl -1040(%rbp), %eax
    testl %eax, %eax
    jz op_priority_6
    movl -1032(%rbp), %eax
    cmpl $120, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1040(%rbp)
    movl -1040(%rbp), %eax
    testl %eax, %eax
    jz op_priority_7
    movl -1032(%rbp), %eax
    cmpl $47, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1040(%rbp)
    movl -1040(%rbp), %eax
    testl %eax, %eax
    jz op_priority_8
    movl -1032(%rbp), %eax
    cmpl $94, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1040(%rbp)
    movl -1040(%rbp), %eax
    testl %eax, %eax
    jz op_priority_9
    movl -1032(%rbp), %eax
    cmpl $37, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1032(%rbp)
    movl -1032(%rbp), %eax
    testl %eax, %eax
    jz op_priority_10
    jmp op_priority_11
op_priority_1:
op_priority_2:
op_priority_3:
    movl $0, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
op_priority_4:
op_priority_5:
    movl $1, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
op_priority_6:
op_priority_7:
op_priority_8:
    movl $2, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
op_priority_9:
op_priority_10:
op_priority_11:
    movl $3, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
op_priority_12:
.global is_operator
is_operator:
    push %rbp
    mov %rsp, %rbp
    subq $1040, %rsp
is_operator_0:
    movb %dil, -1024(%rbp)
    leaq -1024(%rbp), %rax
    movq %rax, -1032(%rbp)
    movq -1032(%rbp), %rax
    movb (%rax), %al
    movb %al, -1032(%rbp)
    movsbl -1032(%rbp), %eax
    movl %eax, -1032(%rbp)
    movl -1032(%rbp), %eax
    cmpl $43, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1032(%rbp)
    movl -1032(%rbp), %eax
    testl %eax, %eax
    jz is_operator_2
is_operator_1:
    movl $1, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp is_operator_2
is_operator_2:
    leaq -1024(%rbp), %rax
    movq %rax, -1032(%rbp)
    movq -1032(%rbp), %rax
    movb (%rax), %al
    movb %al, -1032(%rbp)
    movsbl -1032(%rbp), %eax
    movl %eax, -1032(%rbp)
    movl -1032(%rbp), %eax
    cmpl $45, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1032(%rbp)
    movl -1032(%rbp), %eax
    testl %eax, %eax
    jz is_operator_4
is_operator_3:
    movl $1, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp is_operator_4
is_operator_4:
    leaq -1024(%rbp), %rax
    movq %rax, -1032(%rbp)
    movq -1032(%rbp), %rax
    movb (%rax), %al
    movb %al, -1032(%rbp)
    movsbl -1032(%rbp), %eax
    movl %eax, -1032(%rbp)
    movl -1032(%rbp), %eax
    cmpl $120, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1032(%rbp)
    movl -1032(%rbp), %eax
    testl %eax, %eax
    jz is_operator_6
is_operator_5:
    movl $1, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp is_operator_6
is_operator_6:
    leaq -1024(%rbp), %rax
    movq %rax, -1032(%rbp)
    movq -1032(%rbp), %rax
    movb (%rax), %al
    movb %al, -1032(%rbp)
    movsbl -1032(%rbp), %eax
    movl %eax, -1032(%rbp)
    movl -1032(%rbp), %eax
    cmpl $42, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1032(%rbp)
    movl -1032(%rbp), %eax
    testl %eax, %eax
    jz is_operator_8
is_operator_7:
    movl $1, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp is_operator_8
is_operator_8:
    leaq -1024(%rbp), %rax
    movq %rax, -1032(%rbp)
    movq -1032(%rbp), %rax
    movb (%rax), %al
    movb %al, -1032(%rbp)
    movsbl -1032(%rbp), %eax
    movl %eax, -1032(%rbp)
    movl -1032(%rbp), %eax
    cmpl $47, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1032(%rbp)
    movl -1032(%rbp), %eax
    testl %eax, %eax
    jz is_operator_10
is_operator_9:
    movl $1, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp is_operator_10
is_operator_10:
    leaq -1024(%rbp), %rax
    movq %rax, -1032(%rbp)
    movq -1032(%rbp), %rax
    movb (%rax), %al
    movb %al, -1032(%rbp)
    movsbl -1032(%rbp), %eax
    movl %eax, -1032(%rbp)
    movl -1032(%rbp), %eax
    cmpl $37, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1032(%rbp)
    movl -1032(%rbp), %eax
    testl %eax, %eax
    jz is_operator_12
is_operator_11:
    movl $1, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp is_operator_12
is_operator_12:
    leaq -1024(%rbp), %rax
    movq %rax, -1032(%rbp)
    movq -1032(%rbp), %rax
    movb (%rax), %al
    movb %al, -1032(%rbp)
    movsbl -1032(%rbp), %eax
    movl %eax, -1032(%rbp)
    movl -1032(%rbp), %eax
    cmpl $94, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1032(%rbp)
    movl -1032(%rbp), %eax
    testl %eax, %eax
    jz is_operator_14
is_operator_13:
    movl $1, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp is_operator_14
is_operator_14:
    leaq -1024(%rbp), %rax
    movq %rax, -1032(%rbp)
    movq -1032(%rbp), %rax
    movb (%rax), %al
    movb %al, -1032(%rbp)
    movsbl -1032(%rbp), %eax
    movl %eax, -1032(%rbp)
    movl -1032(%rbp), %eax
    cmpl $62, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1032(%rbp)
    movl -1032(%rbp), %eax
    testl %eax, %eax
    jz is_operator_16
is_operator_15:
    movl $1, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp is_operator_16
is_operator_16:
    leaq -1024(%rbp), %rax
    movq %rax, -1032(%rbp)
    movq -1032(%rbp), %rax
    movb (%rax), %al
    movb %al, -1032(%rbp)
    movsbl -1032(%rbp), %eax
    movl %eax, -1032(%rbp)
    movl -1032(%rbp), %eax
    cmpl $60, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1032(%rbp)
    movl -1032(%rbp), %eax
    testl %eax, %eax
    jz is_operator_18
is_operator_17:
    movl $1, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp is_operator_18
is_operator_18:
    leaq -1024(%rbp), %rax
    movq %rax, -1032(%rbp)
    movq -1032(%rbp), %rax
    movb (%rax), %al
    movb %al, -1032(%rbp)
    movsbl -1032(%rbp), %eax
    movl %eax, -1032(%rbp)
    movl -1032(%rbp), %eax
    cmpl $126, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1032(%rbp)
    movl -1032(%rbp), %eax
    testl %eax, %eax
    jz is_operator_20
is_operator_19:
    movl $1, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp is_operator_20
is_operator_20:
    movl $0, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
.global isAlpha
isAlpha:
    push %rbp
    mov %rsp, %rbp
    subq $1056, %rsp
isAlpha_0:
    movb %dil, -1024(%rbp)
    leaq -1024(%rbp), %rax
    movq %rax, -1032(%rbp)
    movq -1032(%rbp), %rax
    movb (%rax), %al
    movb %al, -1032(%rbp)
    movsbl -1032(%rbp), %eax
    movl %eax, -1032(%rbp)
    movl -1032(%rbp), %eax
    cmpl $97, %eax
    setge %al
    movzbl %al, %eax
    movl %eax, -1032(%rbp)
    movl -1032(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1032(%rbp)
    leaq -1024(%rbp), %rax
    movq %rax, -1040(%rbp)
    movq -1040(%rbp), %rax
    movb (%rax), %al
    movb %al, -1040(%rbp)
    movsbl -1040(%rbp), %eax
    movl %eax, -1040(%rbp)
    movl -1040(%rbp), %eax
    cmpl $122, %eax
    setle %al
    movzbl %al, %eax
    movl %eax, -1040(%rbp)
    movl -1040(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1040(%rbp)
    movl -1032(%rbp), %eax
    andl -1040(%rbp), %eax
    movl %eax, -1032(%rbp)
    movl -1032(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1032(%rbp)
    leaq -1024(%rbp), %rax
    movq %rax, -1040(%rbp)
    movq -1040(%rbp), %rax
    movb (%rax), %al
    movb %al, -1040(%rbp)
    movsbl -1040(%rbp), %eax
    movl %eax, -1040(%rbp)
    movl -1040(%rbp), %eax
    cmpl $65, %eax
    setge %al
    movzbl %al, %eax
    movl %eax, -1040(%rbp)
    movl -1040(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1040(%rbp)
    leaq -1024(%rbp), %rax
    movq %rax, -1048(%rbp)
    movq -1048(%rbp), %rax
    movb (%rax), %al
    movb %al, -1048(%rbp)
    movsbl -1048(%rbp), %eax
    movl %eax, -1048(%rbp)
    movl -1048(%rbp), %eax
    cmpl $90, %eax
    setle %al
    movzbl %al, %eax
    movl %eax, -1048(%rbp)
    movl -1048(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1048(%rbp)
    movl -1040(%rbp), %eax
    andl -1048(%rbp), %eax
    movl %eax, -1040(%rbp)
    movl -1040(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1040(%rbp)
    movl -1032(%rbp), %eax
    orl -1040(%rbp), %eax
    movl %eax, -1032(%rbp)
    movl -1032(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
.global printError
printError:
    push %rbp
    mov %rsp, %rbp
    subq $1056, %rsp
printError_0:
    movq %rdi, -1024(%rbp)
    movq %rsi, -1032(%rbp)
    leaq .LC24(%rip), %rax
    movq %rax, -1040(%rbp)
    leaq -1024(%rbp), %rax
    movq %rax, -1048(%rbp)
    movq -1048(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1048(%rbp)
    leaq -1032(%rbp), %rax
    movq %rax, -1056(%rbp)
    movq -1056(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1056(%rbp)
    subq $176, %rsp
    movq -1040(%rbp), %rdi
    movq -1048(%rbp), %rsi
    movq -1056(%rbp), %rdx
    xor %eax, %eax
    call printf
    addq $176, %rsp
    movl %eax, -1040(%rbp)
    mov %rbp, %rsp
    pop %rbp
    ret
.global printVar
printVar:
    push %rbp
    mov %rsp, %rbp
    subq $1088, %rsp
printVar_0:
    movq %rdi, %rax
    movq %rax, -1072(%rbp)
    leaq -1056(%rbp), %rax
    movq %rax, -1080(%rbp)
    movq -1072(%rbp), %rsi
    movq -1080(%rbp), %rdi
    mov $40, %rdx
    call memcpy
    leaq -1064(%rbp), %rax
    movq %rax, -1072(%rbp)
    leaq -1056(%rbp), %rax
    movq %rax, -1080(%rbp)
    movq -1080(%rbp), %rax
    addq $8, %rax
    movq %rax, -1080(%rbp)
    movq -1080(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1080(%rbp)
    movq -1072(%rbp), %rax
    movl -1080(%rbp), %ecx
    movl %ecx, (%rax)
    leaq -1064(%rbp), %rax
    movq %rax, -1072(%rbp)
    movq -1072(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1072(%rbp)
    movl -1072(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1080(%rbp)
    movl -1080(%rbp), %eax
    testl %eax, %eax
    jz printVar_1
    movl -1072(%rbp), %eax
    cmpl $1, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1072(%rbp)
    movl -1072(%rbp), %eax
    testl %eax, %eax
    jz printVar_2
    jmp printVar_6
printVar_1:
    leaq .LC25(%rip), %rax
    movq %rax, -1072(%rbp)
    leaq -1056(%rbp), %rax
    movq %rax, -1080(%rbp)
    movq -1080(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1080(%rbp)
    subq $176, %rsp
    movq -1072(%rbp), %rdi
    movsd -1080(%rbp), %xmm0
    movl $1, %eax
    call printf
    addq $176, %rsp
    movl %eax, -1072(%rbp)
    jmp printVar_7
printVar_2:
    leaq -1056(%rbp), %rax
    movq %rax, -1072(%rbp)
    movq -1072(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1072(%rbp)
    cvttsd2si -1072(%rbp), %eax
    movl %eax, -1072(%rbp)
    movl -1072(%rbp), %eax
    testl %eax, %eax
    jz printVar_4
printVar_3:
    leaq .LC26(%rip), %rax
    movq %rax, -1072(%rbp)
    subq $176, %rsp
    movq -1072(%rbp), %rdi
    xor %eax, %eax
    call printf
    addq $176, %rsp
    movl %eax, -1072(%rbp)
    jmp printVar_5
printVar_4:
    leaq .LC27(%rip), %rax
    movq %rax, -1072(%rbp)
    subq $176, %rsp
    movq -1072(%rbp), %rdi
    xor %eax, %eax
    call printf
    addq $176, %rsp
    movl %eax, -1072(%rbp)
    jmp printVar_5
printVar_5:
    jmp printVar_7
printVar_6:
    mov %rbp, %rsp
    pop %rbp
    ret
printVar_7:
    mov %rbp, %rsp
    pop %rbp
    ret
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $1312, %rsp
main_0:
    leaq .LC28(%rip), %rax
    movq %rax, -1288(%rbp)
    subq $176, %rsp
    movq -1288(%rbp), %rdi
    xor %eax, %eax
    call printf
    addq $176, %rsp
    movl %eax, -1288(%rbp)
    leaq -1056(%rbp), %rax
    movq %rax, -1288(%rbp)
    movsd .LC29(%rip), %xmm0
    movsd %xmm0, -1296(%rbp)
    movq -1288(%rbp), %rax
    movsd -1296(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1056(%rbp), %rax
    movq %rax, -1288(%rbp)
    movq -1288(%rbp), %rax
    addq $8, %rax
    movq %rax, -1288(%rbp)
    movq -1288(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    leaq -1064(%rbp), %rax
    movq %rax, -1288(%rbp)
    movl $1, %eax
    imull $100, %eax
    movl %eax, -1296(%rbp)
    movslq -1296(%rbp), %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rdi
    call malloc
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movq %rax, -1296(%rbp)
    movq -1288(%rbp), %rax
    movq -1296(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -1064(%rbp), %rax
    movq %rax, -1288(%rbp)
    movq -1288(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1288(%rbp)
    movl -1288(%rbp), %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1288(%rbp)
    movl -1288(%rbp), %eax
    testl %eax, %eax
    jz main_2
main_1:
    movl $1, %edi
    call exit
    jmp main_2
main_2:
main_3:
    movl $1, %eax
    testl %eax, %eax
    jz main_18
main_4:
    leaq -1168(%rbp), %rax
    movq %rax, -1288(%rbp)
    movq -1288(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $1, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $2, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $3, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $4, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $5, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $6, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $7, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $8, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $9, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $10, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $11, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $12, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $13, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $14, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $15, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $16, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $17, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $18, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $19, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $20, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $21, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $22, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $23, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $24, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $25, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $26, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $27, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $28, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $29, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $30, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $31, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $32, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $33, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $34, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $35, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $36, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $37, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $38, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $39, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $40, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $41, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $42, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $43, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $44, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $45, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $46, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $47, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $48, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $49, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $50, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $51, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $52, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $53, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $54, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $55, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $56, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $57, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $58, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $59, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $60, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $61, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $62, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $63, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $64, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $65, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $66, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $67, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $68, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $69, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $70, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $71, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $72, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $73, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $74, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $75, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $76, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $77, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $78, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $79, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $80, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $81, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $82, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $83, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $84, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $85, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $86, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $87, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $88, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $89, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $90, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $91, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $92, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $93, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $94, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $95, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $96, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $97, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $98, %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1288(%rbp), %rax
    addq $99, %rax
    movq %rax, -1288(%rbp)
    movq -1288(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    leaq .LC30(%rip), %rax
    movq %rax, -1288(%rbp)
    subq $176, %rsp
    movq -1288(%rbp), %rdi
    xor %eax, %eax
    call printf
    addq $176, %rsp
    movl %eax, -1288(%rbp)
    leaq -1176(%rbp), %rax
    movq %rax, -1288(%rbp)
    leaq .LC31(%rip), %rax
    movq %rax, -1296(%rbp)
    movl $0, %edi
    movq -1296(%rbp), %rsi
    call fdopen
    movq %rax, -1296(%rbp)
    movq -1288(%rbp), %rax
    movq -1296(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -1168(%rbp), %rax
    movq %rax, -1288(%rbp)
    leaq -1176(%rbp), %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1296(%rbp)
    movq -1288(%rbp), %rdi
    movl $100, %esi
    movq -1296(%rbp), %rdx
    call fgets
    movq %rax, -1288(%rbp)
    leaq -1168(%rbp), %rax
    movq %rax, -1288(%rbp)
    movq $0, %rax
    imulq $1, %rax
    movq %rax, -1296(%rbp)
    movq -1288(%rbp), %rax
    addq -1296(%rbp), %rax
    movq %rax, -1288(%rbp)
    movq -1288(%rbp), %rax
    movb (%rax), %al
    movb %al, -1288(%rbp)
    movsbl -1288(%rbp), %eax
    movl %eax, -1288(%rbp)
    movl -1288(%rbp), %eax
    subl $10, %eax
    movl %eax, -1288(%rbp)
    movl -1288(%rbp), %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1288(%rbp)
    movl -1288(%rbp), %eax
    testl %eax, %eax
    jz main_6
main_5:
    leaq .LC32(%rip), %rax
    movq %rax, -1288(%rbp)
    subq $176, %rsp
    movq -1288(%rbp), %rdi
    xor %eax, %eax
    call printf
    addq $176, %rsp
    movl %eax, -1288(%rbp)
    movl $0, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp main_9
main_6:
    leaq -1168(%rbp), %rax
    movq %rax, -1288(%rbp)
    leaq .LC33(%rip), %rax
    movq %rax, -1296(%rbp)
    movq -1288(%rbp), %rdi
    movq -1296(%rbp), %rsi
    call strcmp
    movl %eax, -1288(%rbp)
    movl -1288(%rbp), %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1288(%rbp)
    movl -1288(%rbp), %eax
    testl %eax, %eax
    jz main_8
main_7:
    leaq .LC34(%rip), %rax
    movq %rax, -1288(%rbp)
    subq $176, %rsp
    movq -1288(%rbp), %rdi
    xor %eax, %eax
    call printf
    addq $176, %rsp
    movl %eax, -1288(%rbp)
    movl $0, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp main_8
main_8:
main_9:
    leaq -1184(%rbp), %rax
    movq %rax, -1288(%rbp)
    movq -1288(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
main_10:
    leaq -1184(%rbp), %rax
    movq %rax, -1288(%rbp)
    movq -1288(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1288(%rbp)
    movl -1288(%rbp), %eax
    cmpl $100, %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -1288(%rbp)
    movl -1288(%rbp), %eax
    testl %eax, %eax
    jz main_15
main_11:
    leaq -1168(%rbp), %rax
    movq %rax, -1288(%rbp)
    leaq -1184(%rbp), %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1296(%rbp)
    movslq -1296(%rbp), %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1296(%rbp)
    movq -1288(%rbp), %rax
    addq -1296(%rbp), %rax
    movq %rax, -1288(%rbp)
    movq -1288(%rbp), %rax
    movb (%rax), %al
    movb %al, -1288(%rbp)
    movsbl -1288(%rbp), %eax
    movl %eax, -1288(%rbp)
    movl -1288(%rbp), %eax
    cmpl $10, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1288(%rbp)
    movl -1288(%rbp), %eax
    testl %eax, %eax
    jz main_13
main_12:
    leaq -1168(%rbp), %rax
    movq %rax, -1288(%rbp)
    leaq -1184(%rbp), %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1296(%rbp)
    movslq -1296(%rbp), %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1296(%rbp)
    movq -1288(%rbp), %rax
    addq -1296(%rbp), %rax
    movq %rax, -1288(%rbp)
    movq -1288(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    jmp main_15
    jmp main_13
main_13:
    jmp main_14
main_14:
    leaq -1184(%rbp), %rax
    movq %rax, -1288(%rbp)
    movq -1288(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1296(%rbp)
    movl -1296(%rbp), %eax
    addl $1, %eax
    movl %eax, -1296(%rbp)
    movq -1288(%rbp), %rax
    movl -1296(%rbp), %ecx
    movl %ecx, (%rax)
    jmp main_10
main_15:
    leaq -1064(%rbp), %rax
    movq %rax, -1288(%rbp)
    movq -1288(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1288(%rbp)
    movq -1288(%rbp), %rax
    movq %rax, -1288(%rbp)
    leaq -1168(%rbp), %rax
    movq %rax, -1296(%rbp)
    movl $1, %eax
    imull $100, %eax
    movl %eax, -1304(%rbp)
    movslq -1304(%rbp), %rax
    movq %rax, -1304(%rbp)
    movq -1288(%rbp), %rdi
    movq -1296(%rbp), %rsi
    movq -1304(%rbp), %rdx
    call memcpy
    movq %rax, -1288(%rbp)
    leaq -1168(%rbp), %rax
    movq %rax, -1288(%rbp)
    movq -1288(%rbp), %rdi
    call format
    leaq -1192(%rbp), %rax
    movq %rax, -1288(%rbp)
    movq -1288(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    leaq -1200(%rbp), %rax
    movq %rax, -1288(%rbp)
    leaq -1192(%rbp), %rax
    movq %rax, -1296(%rbp)
    movq -1288(%rbp), %rax
    movq -1296(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -1240(%rbp), %rax
    movq %rax, -1288(%rbp)
    leaq -1280(%rbp), %rax
    movq %rax, -1296(%rbp)
    leaq -1168(%rbp), %rax
    movq %rax, -1304(%rbp)
    leaq -1192(%rbp), %rax
    movq %rax, -1312(%rbp)
    movq -1296(%rbp), %rdi
    movq -1304(%rbp), %rsi
    movl $100, %edx
    movq -1312(%rbp), %rcx
    call parse
    movq -1296(%rbp), %rsi
    movq -1288(%rbp), %rdi
    mov $40, %rdx
    call memcpy
    leaq -1056(%rbp), %rax
    movq %rax, -1288(%rbp)
    leaq -1240(%rbp), %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rsi
    movq -1288(%rbp), %rdi
    mov $40, %rdx
    call memcpy
    leaq -1192(%rbp), %rax
    movq %rax, -1288(%rbp)
    movq -1288(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1288(%rbp)
    movl -1288(%rbp), %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1288(%rbp)
    movl -1288(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1288(%rbp)
    movl -1288(%rbp), %eax
    testl %eax, %eax
    jz main_17
    leaq -1056(%rbp), %rax
    movq %rax, -1288(%rbp)
    movq -1288(%rbp), %rax
    addq $8, %rax
    movq %rax, -1288(%rbp)
    movq -1288(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1288(%rbp)
    movl -1288(%rbp), %eax
    cmpl $2, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1288(%rbp)
    movl -1288(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1288(%rbp)
    movl -1288(%rbp), %eax
    testl %eax, %eax
    jz main_17
main_16:
    leaq .LC35(%rip), %rax
    movq %rax, -1288(%rbp)
    leaq -1064(%rbp), %rax
    movq %rax, -1296(%rbp)
    movq -1296(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1296(%rbp)
    subq $176, %rsp
    movq -1288(%rbp), %rdi
    movq -1296(%rbp), %rsi
    xor %eax, %eax
    call printf
    addq $176, %rsp
    movl %eax, -1288(%rbp)
    leaq -1056(%rbp), %rax
    movq %rax, -1288(%rbp)
    movq -1288(%rbp), %rdi
    call printVar
    leaq .LC36(%rip), %rax
    movq %rax, -1288(%rbp)
    subq $176, %rsp
    movq -1288(%rbp), %rdi
    xor %eax, %eax
    call printf
    addq $176, %rsp
    movl %eax, -1288(%rbp)
    jmp main_17
main_17:
    jmp main_3
main_18:
    leaq -1064(%rbp), %rax
    movq %rax, -1288(%rbp)
    movq -1288(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1288(%rbp)
    movq -1288(%rbp), %rax
    movq %rax, -1288(%rbp)
    movq -1288(%rbp), %rdi
    call free
    movl $0, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
