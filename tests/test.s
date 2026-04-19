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
    .byte 'N', 'o', ' ', 'o', 'p', 'e', 'r', 'a', 't', 'o', 'r', ' ', 'g', 'i', 'v', 'e', 'n', 0
.align 8
.LC7:
    .quad 0x0000000000000000
.align 8
.LC8:
    .quad 0x0000000000000000
.LC9:
    .byte 'I', 'N', 'V', 'A', 'L', 'I', 'D', ' ', 'S', 'Y', 'N', 'T', 'A', 'X', ',', ' ', 'S', 'I', 'N', 'G', 'L', 'E', ' ', 'E', 'Q', 'U', 'A', 'L', 'S', ' ', 'S', 'I', 'G', 'N', ' ', 'O', 'N', 'L', 'Y', 0
.LC10:
    .byte ' ', 0
.LC11:
    .byte 'N', 'O', ' ', 'E', 'N', 'D', ' ', 'O', 'F', ' ', 'S', 'T', 'R', 'I', 'N', 'G', ' ', 'F', 'O', 'U', 'N', 'D', '!', 0
.LC12:
    .byte ' ', 0
.LC13:
    .byte 'N', 'O', ' ', 'B', 'R', 'A', 'C', 'K', 'E', 'T', 'S', ' ', 'A', 'R', 'E', ' ', 'C', 'O', 'O', 'K', 'E', 'D', 0
.LC14:
    .byte ' ', 0
.LC15:
    .byte 'T', 'R', 'I', 'E', 'D', ' ', 'T', 'O', ' ', 'C', 'R', 'E', 'A', 'T', 'E', ' ', 'V', 'A', 'R', 'I', 'A', 'B', 'L', 'E', ' ', 'W', 'I', 'T', 'H', ' ', 'N', 'O', 'N', '-', 'A', 'L', 'P', 'H', 'A', ' ', 'F', 'I', 'R', 'S', 'T', ' ', 'C', 'H', 'A', 'R', 'A', 'C', 'T', 'E', 'R', ':', ' ', 0
.LC16:
    .byte 'E', 'R', 'R', 'O', 'R', ':', ' ', '%', 's', ' ', '%', 's', 0x0A, 0x0A, 0
.LC17:
    .byte '%', 'g', 0
.LC18:
    .byte 't', 'r', 'u', 'e', 0
.LC19:
    .byte 'f', 'a', 'l', 's', 'e', 0
.LC20:
    .byte 'P', 'I', 0
.LC21:
    .byte 'E', 0
.LC22:
    .byte 'r', 'o', 'o', 't', '2', 0
.align 8
.LC23:
    .quad 0x400921fb54442d18
.align 8
.LC24:
    .quad 0x4005bf0a8b145769
.align 8
.LC25:
    .quad 0x3ff6a09e667f3bcd
.LC26:
    .byte 'S', 't', 'a', 'r', 't', 'i', 'n', 'g', ' ', 'C', 'a', 'l', 'c', 'u', 'l', 'a', 't', 'o', 'r', 0x0A, 0
.align 8
.LC27:
    .quad 0x0000000000000000
.LC28:
    .byte '>', '>', ' ', 0
.LC29:
    .byte 'r', 0
.LC30:
    .byte 'E', 'n', 'd', 'i', 'n', 'g', ' ', 'C', 'a', 'l', 'c', 'u', 'l', 'a', 't', 'o', 'r', '.', 0x0A, 0
.LC31:
    .byte 'e', 'x', 'i', 't', 0x0A, 0
.LC32:
    .byte 'E', 'n', 'd', 'i', 'n', 'g', ' ', 'C', 'a', 'l', 'c', 'u', 'l', 'a', 't', 'o', 'r', '.', 0x0A, 0
.LC33:
    .byte '%', 's', ' ', '=', ' ', 0
.LC34:
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
.global last_result
.bss
.align 8
last_result:
    .zero 40
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
    subq $1104, %rsp
my_pow_0:
    movsd %xmm0, -1064(%rbp)
    movsd %xmm1, -1072(%rbp)
    leaq -1072(%rbp), %rax
    movq %rax, -1080(%rbp)
    movq -1080(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1080(%rbp)
    mov $1, %eax
    movl %eax, -1088(%rbp)
    movl -1080(%rbp), %eax
    cmpl -1088(%rbp), %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1080(%rbp)
    movl -1080(%rbp), %eax
    testl %eax, %eax
    jz my_pow_2
my_pow_1:
    leaq -1064(%rbp), %rax
    movq %rax, -1080(%rbp)
    movq -1080(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1080(%rbp)
    movsd -1080(%rbp), %xmm0
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp my_pow_2
my_pow_2:
    leaq -1064(%rbp), %rax
    movq %rax, -1080(%rbp)
    movq -1080(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1080(%rbp)
    leaq -1064(%rbp), %rax
    movq %rax, -1088(%rbp)
    movq -1088(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1088(%rbp)
    leaq -1072(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1096(%rbp)
    mov $1, %eax
    movl %eax, -1104(%rbp)
    movsd -1096(%rbp), %xmm0
    subsd -1104(%rbp), %xmm0
    movsd %xmm0, -1096(%rbp)
    subq $32, %rsp
    movsd -1088(%rbp), %xmm0
    movsd -1096(%rbp), %xmm1
    call my_pow
    addq $32, %rsp
    movsd %xmm0, -1088(%rbp)
    movsd -1080(%rbp), %xmm0
    mulsd -1088(%rbp), %xmm0
    movsd %xmm0, -1080(%rbp)
    movsd -1080(%rbp), %xmm0
    mov %rbp, %rsp
    pop %rbp
    ret
.global format
format:
    push %rbp
    mov %rsp, %rbp
    subq $1120, %rsp
format_0:
    movq %rcx, -1064(%rbp)
    leaq -1072(%rbp), %rax
    movq %rax, -1088(%rbp)
    movq -1088(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    leaq -1080(%rbp), %rax
    movq %rax, -1088(%rbp)
    movq -1088(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
format_1:
    leaq -1080(%rbp), %rax
    movq %rax, -1088(%rbp)
    movq -1088(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1088(%rbp)
    leaq -1064(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movq %rax, -1096(%rbp)
    subq $32, %rsp
    movq -1096(%rbp), %rcx
    call strlen
    addq $32, %rsp
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movl %eax, -1096(%rbp)
    movl -1088(%rbp), %eax
    cmpl -1096(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -1088(%rbp)
    movl -1088(%rbp), %eax
    testl %eax, %eax
    jz format_7
format_2:
    leaq -1064(%rbp), %rax
    movq %rax, -1088(%rbp)
    movq -1088(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1088(%rbp)
    leaq -1080(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1096(%rbp)
    movslq -1096(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1096(%rbp)
    movq -1088(%rbp), %rax
    addq -1096(%rbp), %rax
    movq %rax, -1088(%rbp)
    movq -1088(%rbp), %rax
    movb (%rax), %al
    movb %al, -1088(%rbp)
    movsbl -1088(%rbp), %eax
    movl %eax, -1088(%rbp)
    movl -1088(%rbp), %eax
    cmpl $32, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1088(%rbp)
    movl -1088(%rbp), %eax
    testl %eax, %eax
    jz format_4
format_3:
    leaq -1064(%rbp), %rax
    movq %rax, -1088(%rbp)
    movq -1088(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1088(%rbp)
    leaq -1072(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1104(%rbp)
    movl -1104(%rbp), %eax
    addl $1, %eax
    movl %eax, -1112(%rbp)
    movq -1096(%rbp), %rax
    movl -1112(%rbp), %ecx
    movl %ecx, (%rax)
    movslq -1104(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1096(%rbp)
    movq -1088(%rbp), %rax
    addq -1096(%rbp), %rax
    movq %rax, -1088(%rbp)
    leaq -1064(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1096(%rbp)
    leaq -1080(%rbp), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1112(%rbp)
    movl -1112(%rbp), %eax
    addl $1, %eax
    movl %eax, -1112(%rbp)
    movq -1104(%rbp), %rax
    movl -1112(%rbp), %ecx
    movl %ecx, (%rax)
    movslq -1112(%rbp), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1104(%rbp)
    movq -1096(%rbp), %rax
    addq -1104(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movb (%rax), %al
    movb %al, -1096(%rbp)
    movq -1088(%rbp), %rax
    movb -1096(%rbp), %cl
    movb %cl, (%rax)
    jmp format_5
format_4:
    leaq -1064(%rbp), %rax
    movq %rax, -1088(%rbp)
    movq -1088(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1088(%rbp)
    leaq -1072(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1104(%rbp)
    movl -1104(%rbp), %eax
    addl $1, %eax
    movl %eax, -1112(%rbp)
    movq -1096(%rbp), %rax
    movl -1112(%rbp), %ecx
    movl %ecx, (%rax)
    movslq -1104(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1096(%rbp)
    movq -1088(%rbp), %rax
    addq -1096(%rbp), %rax
    movq %rax, -1088(%rbp)
    leaq -1064(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1096(%rbp)
    leaq -1080(%rbp), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1104(%rbp)
    movslq -1104(%rbp), %rax
    movq %rax, -1104(%rbp)
    movq -1104(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1104(%rbp)
    movq -1096(%rbp), %rax
    addq -1104(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movb (%rax), %al
    movb %al, -1096(%rbp)
    movq -1088(%rbp), %rax
    movb -1096(%rbp), %cl
    movb %cl, (%rax)
    jmp format_5
format_5:
    jmp format_6
format_6:
    leaq -1080(%rbp), %rax
    movq %rax, -1088(%rbp)
    movq -1088(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1096(%rbp)
    movl -1096(%rbp), %eax
    addl $1, %eax
    movl %eax, -1096(%rbp)
    movq -1088(%rbp), %rax
    movl -1096(%rbp), %ecx
    movl %ecx, (%rax)
    jmp format_1
format_7:
    leaq -1064(%rbp), %rax
    movq %rax, -1088(%rbp)
    movq -1088(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1088(%rbp)
    leaq -1072(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1096(%rbp)
    movslq -1096(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1096(%rbp)
    movq -1088(%rbp), %rax
    addq -1096(%rbp), %rax
    movq %rax, -1088(%rbp)
    movq -1088(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    mov %rbp, %rsp
    pop %rbp
    ret
.global parseToken
parseToken:
    push %rbp
    mov %rsp, %rbp
    subq $1184, %rsp
parseToken_0:
    movq %rcx, -1064(%rbp)
    movq %rdx, -1072(%rbp)
    leaq -1112(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    addq $8, %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    leaq -1112(%rbp), %rax
    movq %rax, -1160(%rbp)
    movsd .LC0(%rip), %xmm0
    movsd %xmm0, -1168(%rbp)
    movq -1160(%rbp), %rax
    movsd -1168(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1128(%rbp), %rax
    movq %rax, -1160(%rbp)
    leaq -1072(%rbp), %rax
    movq %rax, -1168(%rbp)
    movq -1168(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1168(%rbp)
    movq -1168(%rbp), %rax
    movq %rax, -1168(%rbp)
    subq $32, %rsp
    movq -1168(%rbp), %rcx
    call strlen
    addq $32, %rsp
    movq %rax, -1168(%rbp)
    movq -1160(%rbp), %rax
    movq -1168(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -1128(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1160(%rbp)
    movl -1160(%rbp), %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1160(%rbp)
    movl -1160(%rbp), %eax
    movl %eax, -1160(%rbp)
    movl -1160(%rbp), %eax
    testl %eax, %eax
    jz parseToken_2
parseToken_1:
    leaq -1112(%rbp), %rax
    movq %rax, -1160(%rbp)
    movsd .LC1(%rip), %xmm0
    movsd %xmm0, -1168(%rbp)
    movq -1160(%rbp), %rax
    movsd -1168(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1112(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rdx
    movq -1064(%rbp), %rcx
    mov $40, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp parseToken_2
parseToken_2:
    leaq -1072(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1160(%rbp)
    movq $0, %rax
    imulq $1, %rax
    movq %rax, -1168(%rbp)
    movq -1160(%rbp), %rax
    addq -1168(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movb (%rax), %al
    movb %al, -1160(%rbp)
    movsbl -1160(%rbp), %eax
    movl %eax, -1160(%rbp)
    movl -1160(%rbp), %eax
    cmpl $40, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1160(%rbp)
    movl -1160(%rbp), %eax
    testl %eax, %eax
    jz parseToken_4
parseToken_3:
    leaq -1120(%rbp), %rax
    movq %rax, -1160(%rbp)
    leaq -1072(%rbp), %rax
    movq %rax, -1168(%rbp)
    movq -1168(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1168(%rbp)
    movq $1, %rax
    imulq $1, %rax
    movq %rax, -1176(%rbp)
    movq -1168(%rbp), %rax
    addq -1176(%rbp), %rax
    movq %rax, -1168(%rbp)
    movq -1160(%rbp), %rax
    movq -1168(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -1128(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1168(%rbp)
    movq -1168(%rbp), %rax
    subq $1, %rax
    movq %rax, -1168(%rbp)
    movq -1160(%rbp), %rax
    movq -1168(%rbp), %rcx
    movq %rcx, (%rax)
    jmp parseToken_4
parseToken_4:
    leaq -1072(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1160(%rbp)
    movq $0, %rax
    imulq $1, %rax
    movq %rax, -1168(%rbp)
    movq -1160(%rbp), %rax
    addq -1168(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movb (%rax), %al
    movb %al, -1160(%rbp)
    movsbl -1160(%rbp), %eax
    movl %eax, -1160(%rbp)
    movl -1160(%rbp), %eax
    cmpl $35, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1160(%rbp)
    movl -1160(%rbp), %eax
    testl %eax, %eax
    jz parseToken_6
parseToken_5:
    leaq -1112(%rbp), %rax
    movq %rax, -1160(%rbp)
    leaq last_result(%rip), %rax
    movq %rax, -1168(%rbp)
    movq -1168(%rbp), %rdx
    movq -1160(%rbp), %rcx
    mov $40, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    leaq -1112(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rdx
    movq -1064(%rbp), %rcx
    mov $40, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp parseToken_6
parseToken_6:
    leaq -1136(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
parseToken_7:
    leaq -1136(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1160(%rbp)
    movl -1160(%rbp), %eax
    cmpl $3, %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -1160(%rbp)
    movl -1160(%rbp), %eax
    testl %eax, %eax
    jz parseToken_12
parseToken_8:
    leaq -1072(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movq %rax, -1160(%rbp)
    leaq CONSTANTS_NAMES(%rip), %rax
    movq %rax, -1168(%rbp)
    leaq -1136(%rbp), %rax
    movq %rax, -1176(%rbp)
    movq -1176(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1176(%rbp)
    movslq -1176(%rbp), %rax
    movq %rax, -1176(%rbp)
    movq -1176(%rbp), %rax
    imulq $8, %rax
    movq %rax, -1176(%rbp)
    movq -1168(%rbp), %rax
    addq -1176(%rbp), %rax
    movq %rax, -1168(%rbp)
    movq -1168(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1168(%rbp)
    movq -1168(%rbp), %rax
    movq %rax, -1168(%rbp)
    subq $32, %rsp
    movq -1160(%rbp), %rcx
    movq -1168(%rbp), %rdx
    call strcmp
    addq $32, %rsp
    movl %eax, -1160(%rbp)
    movl -1160(%rbp), %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1160(%rbp)
    movl -1160(%rbp), %eax
    testl %eax, %eax
    jz parseToken_10
parseToken_9:
    leaq -1112(%rbp), %rax
    movq %rax, -1160(%rbp)
    leaq CONSTANTS_VALUES(%rip), %rax
    movq %rax, -1168(%rbp)
    leaq -1136(%rbp), %rax
    movq %rax, -1176(%rbp)
    movq -1176(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1176(%rbp)
    movslq -1176(%rbp), %rax
    movq %rax, -1176(%rbp)
    movq -1176(%rbp), %rax
    imulq $8, %rax
    movq %rax, -1176(%rbp)
    movq -1168(%rbp), %rax
    addq -1176(%rbp), %rax
    movq %rax, -1168(%rbp)
    movq -1168(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1168(%rbp)
    movq -1160(%rbp), %rax
    movsd -1168(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1112(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rdx
    movq -1064(%rbp), %rcx
    mov $40, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp parseToken_10
parseToken_10:
    jmp parseToken_11
parseToken_11:
    leaq -1136(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1168(%rbp)
    movl -1168(%rbp), %eax
    addl $1, %eax
    movl %eax, -1168(%rbp)
    movq -1160(%rbp), %rax
    movl -1168(%rbp), %ecx
    movl %ecx, (%rax)
    jmp parseToken_7
parseToken_12:
    leaq -1144(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
parseToken_13:
    leaq -1144(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1160(%rbp)
    leaq variableCount(%rip), %rax
    movq %rax, -1168(%rbp)
    movq -1168(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1168(%rbp)
    movl -1160(%rbp), %eax
    cmpl -1168(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -1160(%rbp)
    movl -1160(%rbp), %eax
    testl %eax, %eax
    jz parseToken_18
parseToken_14:
    leaq -1072(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movq %rax, -1160(%rbp)
    leaq variableNames(%rip), %rax
    movq %rax, -1168(%rbp)
    leaq -1144(%rbp), %rax
    movq %rax, -1176(%rbp)
    movq -1176(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1176(%rbp)
    movslq -1176(%rbp), %rax
    movq %rax, -1176(%rbp)
    movq -1176(%rbp), %rax
    imulq $8, %rax
    movq %rax, -1176(%rbp)
    movq -1168(%rbp), %rax
    addq -1176(%rbp), %rax
    movq %rax, -1168(%rbp)
    movq -1168(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1168(%rbp)
    movq -1168(%rbp), %rax
    movq %rax, -1168(%rbp)
    subq $32, %rsp
    movq -1160(%rbp), %rcx
    movq -1168(%rbp), %rdx
    call strcmp
    addq $32, %rsp
    movl %eax, -1160(%rbp)
    movl -1160(%rbp), %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1160(%rbp)
    movl -1160(%rbp), %eax
    testl %eax, %eax
    jz parseToken_16
parseToken_15:
    leaq -1112(%rbp), %rax
    movq %rax, -1160(%rbp)
    leaq variables(%rip), %rax
    movq %rax, -1168(%rbp)
    leaq -1144(%rbp), %rax
    movq %rax, -1176(%rbp)
    movq -1176(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1176(%rbp)
    movslq -1176(%rbp), %rax
    movq %rax, -1176(%rbp)
    movq -1176(%rbp), %rax
    imulq $40, %rax
    movq %rax, -1176(%rbp)
    movq -1168(%rbp), %rax
    addq -1176(%rbp), %rax
    movq %rax, -1168(%rbp)
    movq -1168(%rbp), %rdx
    movq -1160(%rbp), %rcx
    mov $40, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    leaq -1112(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rdx
    movq -1064(%rbp), %rcx
    mov $40, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp parseToken_16
parseToken_16:
    jmp parseToken_17
parseToken_17:
    leaq -1144(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1168(%rbp)
    movl -1168(%rbp), %eax
    addl $1, %eax
    movl %eax, -1168(%rbp)
    movq -1160(%rbp), %rax
    movl -1168(%rbp), %ecx
    movl %ecx, (%rax)
    jmp parseToken_13
parseToken_18:
    leaq -1152(%rbp), %rax
    movq %rax, -1160(%rbp)
    leaq -1072(%rbp), %rax
    movq %rax, -1168(%rbp)
    movq -1168(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1168(%rbp)
    subq $32, %rsp
    movq -1168(%rbp), %rcx
    call atof
    addq $32, %rsp
    movsd %xmm0, -1168(%rbp)
    movq -1160(%rbp), %rax
    movsd -1168(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1152(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1160(%rbp)
    cvttsd2si -1160(%rbp), %eax
    movl %eax, -1160(%rbp)
    movl -1160(%rbp), %eax
    cmpl $0, %eax
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
    jz parseToken_20
    leaq -1072(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movq %rax, -1160(%rbp)
    leaq .LC2(%rip), %rax
    movq %rax, -1168(%rbp)
    subq $32, %rsp
    movq -1160(%rbp), %rcx
    movq -1168(%rbp), %rdx
    call strcmp
    addq $32, %rsp
    movl %eax, -1160(%rbp)
    movl -1160(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1160(%rbp)
    movl -1160(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1160(%rbp)
    movl -1160(%rbp), %eax
    testl %eax, %eax
    jz parseToken_20
parseToken_19:
    leaq .LC3(%rip), %rax
    movq %rax, -1160(%rbp)
    leaq -1072(%rbp), %rax
    movq %rax, -1168(%rbp)
    movq -1168(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1168(%rbp)
    subq $32, %rsp
    movq -1160(%rbp), %rcx
    movq -1168(%rbp), %rdx
    call printError
    addq $32, %rsp
    leaq -1112(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    addq $8, %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rax
    movl $2, %ecx
    movl %ecx, (%rax)
    leaq -1112(%rbp), %rax
    movq %rax, -1160(%rbp)
    movsd .LC4(%rip), %xmm0
    movsd %xmm0, -1168(%rbp)
    movq -1160(%rbp), %rax
    movsd -1168(%rbp), %xmm2
    movsd %xmm2, (%rax)
    jmp parseToken_21
parseToken_20:
    leaq -1112(%rbp), %rax
    movq %rax, -1160(%rbp)
    leaq -1152(%rbp), %rax
    movq %rax, -1168(%rbp)
    movq -1168(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1168(%rbp)
    movq -1160(%rbp), %rax
    movsd -1168(%rbp), %xmm2
    movsd %xmm2, (%rax)
    jmp parseToken_21
parseToken_21:
    leaq -1112(%rbp), %rax
    movq %rax, -1160(%rbp)
    movq -1160(%rbp), %rdx
    movq -1064(%rbp), %rcx
    mov $40, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    mov %rbp, %rsp
    pop %rbp
    ret
.global solve
solve:
    push %rbp
    mov %rsp, %rbp
    subq $1216, %rsp
solve_0:
    movq %rcx, -1064(%rbp)
    movq %rdx, %rax
    movq %rax, -1200(%rbp)
    movq %r8, %rax
    movq %rax, -1208(%rbp)
    movb %r9b, -1152(%rbp)
    leaq -1104(%rbp), %rax
    movq %rax, -1216(%rbp)
    movq -1200(%rbp), %rdx
    movq -1216(%rbp), %rcx
    mov $40, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    leaq -1144(%rbp), %rax
    movq %rax, -1200(%rbp)
    movq -1208(%rbp), %rdx
    movq -1200(%rbp), %rcx
    mov $40, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    leaq -1104(%rbp), %rax
    movq %rax, -1200(%rbp)
    movq -1200(%rbp), %rax
    addq $8, %rax
    movq %rax, -1200(%rbp)
    movq -1200(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1200(%rbp)
    movl -1200(%rbp), %eax
    cmpl $2, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1200(%rbp)
    movl -1200(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1200(%rbp)
    movl -1200(%rbp), %eax
    testl %eax, %eax
    leaq -1144(%rbp), %rax
    movq %rax, -1200(%rbp)
    movq -1200(%rbp), %rax
    addq $8, %rax
    movq %rax, -1200(%rbp)
    movq -1200(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1200(%rbp)
    movl -1200(%rbp), %eax
    cmpl $2, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1200(%rbp)
    movl -1200(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1200(%rbp)
    movl -1200(%rbp), %eax
    testl %eax, %eax
    jz solve_2
solve_1:
    leaq -1192(%rbp), %rax
    movq %rax, -1200(%rbp)
    movq -1200(%rbp), %rax
    addq $8, %rax
    movq %rax, -1200(%rbp)
    movq -1200(%rbp), %rax
    movl $2, %ecx
    movl %ecx, (%rax)
    leaq -1192(%rbp), %rax
    movq %rax, -1200(%rbp)
    movsd .LC5(%rip), %xmm0
    movsd %xmm0, -1208(%rbp)
    movq -1200(%rbp), %rax
    movsd -1208(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1192(%rbp), %rax
    movq %rax, -1200(%rbp)
    movq -1200(%rbp), %rdx
    movq -1064(%rbp), %rcx
    mov $40, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp solve_2
solve_2:
    leaq -1152(%rbp), %rax
    movq %rax, -1200(%rbp)
    movq -1200(%rbp), %rax
    movb (%rax), %al
    movb %al, -1200(%rbp)
    movsbl -1200(%rbp), %eax
    movl %eax, -1200(%rbp)
    movl -1200(%rbp), %eax
    cmpl $126, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1208(%rbp)
    movl -1208(%rbp), %eax
    testl %eax, %eax
    jz solve_3
    movl -1200(%rbp), %eax
    cmpl $60, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1208(%rbp)
    movl -1208(%rbp), %eax
    testl %eax, %eax
    jz solve_4
    movl -1200(%rbp), %eax
    cmpl $62, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1208(%rbp)
    movl -1208(%rbp), %eax
    testl %eax, %eax
    jz solve_5
    movl -1200(%rbp), %eax
    cmpl $94, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1208(%rbp)
    movl -1208(%rbp), %eax
    testl %eax, %eax
    jz solve_6
    movl -1200(%rbp), %eax
    cmpl $37, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1208(%rbp)
    movl -1208(%rbp), %eax
    testl %eax, %eax
    jz solve_7
    movl -1200(%rbp), %eax
    cmpl $42, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1208(%rbp)
    movl -1208(%rbp), %eax
    testl %eax, %eax
    jz solve_8
    movl -1200(%rbp), %eax
    cmpl $120, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1208(%rbp)
    movl -1208(%rbp), %eax
    testl %eax, %eax
    jz solve_9
    movl -1200(%rbp), %eax
    cmpl $47, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1208(%rbp)
    movl -1208(%rbp), %eax
    testl %eax, %eax
    jz solve_10
    movl -1200(%rbp), %eax
    cmpl $43, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1208(%rbp)
    movl -1208(%rbp), %eax
    testl %eax, %eax
    jz solve_11
    movl -1200(%rbp), %eax
    cmpl $45, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1200(%rbp)
    movl -1200(%rbp), %eax
    testl %eax, %eax
    jz solve_12
    jmp solve_13
solve_3:
    leaq -1192(%rbp), %rax
    movq %rax, -1200(%rbp)
    leaq -1104(%rbp), %rax
    movq %rax, -1208(%rbp)
    movq -1208(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1208(%rbp)
    cvttsd2si -1208(%rbp), %eax
    movl %eax, -1208(%rbp)
    leaq -1144(%rbp), %rax
    movq %rax, -1216(%rbp)
    movq -1216(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1216(%rbp)
    cvttsd2si -1216(%rbp), %eax
    movl %eax, -1216(%rbp)
    movl -1208(%rbp), %eax
    cmpl -1216(%rbp), %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1208(%rbp)
    movslq -1208(%rbp), %rax
    cvtsi2sd %rax, %xmm0
    movsd %xmm0, -1208(%rbp)
    movq -1200(%rbp), %rax
    movsd -1208(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1192(%rbp), %rax
    movq %rax, -1200(%rbp)
    movq -1200(%rbp), %rax
    addq $8, %rax
    movq %rax, -1200(%rbp)
    movq -1200(%rbp), %rax
    movl $1, %ecx
    movl %ecx, (%rax)
    jmp solve_14
solve_4:
    leaq -1192(%rbp), %rax
    movq %rax, -1200(%rbp)
    leaq -1104(%rbp), %rax
    movq %rax, -1208(%rbp)
    movq -1208(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1208(%rbp)
    cvttsd2si -1208(%rbp), %eax
    movl %eax, -1208(%rbp)
    leaq -1144(%rbp), %rax
    movq %rax, -1216(%rbp)
    movq -1216(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1216(%rbp)
    cvttsd2si -1216(%rbp), %eax
    movl %eax, -1216(%rbp)
    movl -1208(%rbp), %eax
    cmpl -1216(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -1208(%rbp)
    movslq -1208(%rbp), %rax
    cvtsi2sd %rax, %xmm0
    movsd %xmm0, -1208(%rbp)
    movq -1200(%rbp), %rax
    movsd -1208(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1192(%rbp), %rax
    movq %rax, -1200(%rbp)
    movq -1200(%rbp), %rax
    addq $8, %rax
    movq %rax, -1200(%rbp)
    movq -1200(%rbp), %rax
    movl $1, %ecx
    movl %ecx, (%rax)
    jmp solve_14
solve_5:
    leaq -1192(%rbp), %rax
    movq %rax, -1200(%rbp)
    leaq -1104(%rbp), %rax
    movq %rax, -1208(%rbp)
    movq -1208(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1208(%rbp)
    cvttsd2si -1208(%rbp), %eax
    movl %eax, -1208(%rbp)
    leaq -1144(%rbp), %rax
    movq %rax, -1216(%rbp)
    movq -1216(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1216(%rbp)
    cvttsd2si -1216(%rbp), %eax
    movl %eax, -1216(%rbp)
    movl -1208(%rbp), %eax
    cmpl -1216(%rbp), %eax
    setg %al
    movzbl %al, %eax
    movl %eax, -1208(%rbp)
    movslq -1208(%rbp), %rax
    cvtsi2sd %rax, %xmm0
    movsd %xmm0, -1208(%rbp)
    movq -1200(%rbp), %rax
    movsd -1208(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1192(%rbp), %rax
    movq %rax, -1200(%rbp)
    movq -1200(%rbp), %rax
    addq $8, %rax
    movq %rax, -1200(%rbp)
    movq -1200(%rbp), %rax
    movl $1, %ecx
    movl %ecx, (%rax)
    jmp solve_14
solve_6:
    leaq -1192(%rbp), %rax
    movq %rax, -1200(%rbp)
    leaq -1104(%rbp), %rax
    movq %rax, -1208(%rbp)
    movq -1208(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1208(%rbp)
    leaq -1144(%rbp), %rax
    movq %rax, -1216(%rbp)
    movq -1216(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1216(%rbp)
    subq $32, %rsp
    movsd -1208(%rbp), %xmm0
    movsd -1216(%rbp), %xmm1
    call my_pow
    addq $32, %rsp
    movsd %xmm0, -1208(%rbp)
    movq -1200(%rbp), %rax
    movsd -1208(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1192(%rbp), %rax
    movq %rax, -1200(%rbp)
    movq -1200(%rbp), %rax
    addq $8, %rax
    movq %rax, -1200(%rbp)
    movq -1200(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    jmp solve_14
solve_7:
    leaq -1192(%rbp), %rax
    movq %rax, -1200(%rbp)
    leaq -1104(%rbp), %rax
    movq %rax, -1208(%rbp)
    movq -1208(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1208(%rbp)
    cvttsd2si -1208(%rbp), %eax
    movl %eax, -1208(%rbp)
    leaq -1144(%rbp), %rax
    movq %rax, -1216(%rbp)
    movq -1216(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1216(%rbp)
    cvttsd2si -1216(%rbp), %eax
    movl %eax, -1216(%rbp)
    movl -1208(%rbp), %eax
    cltd
    idivl -1216(%rbp)
    movl %edx, -1208(%rbp)
    movslq -1208(%rbp), %rax
    cvtsi2ss %rax, %xmm0
    movss %xmm0, -1208(%rbp)
    movq -1200(%rbp), %rax
    movsd -1208(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1192(%rbp), %rax
    movq %rax, -1200(%rbp)
    movq -1200(%rbp), %rax
    addq $8, %rax
    movq %rax, -1200(%rbp)
    movq -1200(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    jmp solve_14
solve_8:
solve_9:
    leaq -1192(%rbp), %rax
    movq %rax, -1200(%rbp)
    leaq -1104(%rbp), %rax
    movq %rax, -1208(%rbp)
    movq -1208(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1208(%rbp)
    leaq -1144(%rbp), %rax
    movq %rax, -1216(%rbp)
    movq -1216(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1216(%rbp)
    movsd -1208(%rbp), %xmm0
    mulsd -1216(%rbp), %xmm0
    movsd %xmm0, -1208(%rbp)
    movq -1200(%rbp), %rax
    movsd -1208(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1192(%rbp), %rax
    movq %rax, -1200(%rbp)
    movq -1200(%rbp), %rax
    addq $8, %rax
    movq %rax, -1200(%rbp)
    movq -1200(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    jmp solve_14
solve_10:
    leaq -1192(%rbp), %rax
    movq %rax, -1200(%rbp)
    leaq -1104(%rbp), %rax
    movq %rax, -1208(%rbp)
    movq -1208(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1208(%rbp)
    leaq -1144(%rbp), %rax
    movq %rax, -1216(%rbp)
    movq -1216(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1216(%rbp)
    movsd -1208(%rbp), %xmm0
    divsd -1216(%rbp), %xmm0
    movsd %xmm0, -1208(%rbp)
    movq -1200(%rbp), %rax
    movsd -1208(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1192(%rbp), %rax
    movq %rax, -1200(%rbp)
    movq -1200(%rbp), %rax
    addq $8, %rax
    movq %rax, -1200(%rbp)
    movq -1200(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    jmp solve_14
solve_11:
    leaq -1192(%rbp), %rax
    movq %rax, -1200(%rbp)
    leaq -1104(%rbp), %rax
    movq %rax, -1208(%rbp)
    movq -1208(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1208(%rbp)
    leaq -1144(%rbp), %rax
    movq %rax, -1216(%rbp)
    movq -1216(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1216(%rbp)
    movsd -1208(%rbp), %xmm0
    addsd -1216(%rbp), %xmm0
    movsd %xmm0, -1208(%rbp)
    movq -1200(%rbp), %rax
    movsd -1208(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1192(%rbp), %rax
    movq %rax, -1200(%rbp)
    movq -1200(%rbp), %rax
    addq $8, %rax
    movq %rax, -1200(%rbp)
    movq -1200(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    jmp solve_14
solve_12:
    leaq -1192(%rbp), %rax
    movq %rax, -1200(%rbp)
    leaq -1104(%rbp), %rax
    movq %rax, -1208(%rbp)
    movq -1208(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1208(%rbp)
    leaq -1144(%rbp), %rax
    movq %rax, -1216(%rbp)
    movq -1216(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1216(%rbp)
    movsd -1208(%rbp), %xmm0
    subsd -1216(%rbp), %xmm0
    movsd %xmm0, -1208(%rbp)
    movq -1200(%rbp), %rax
    movsd -1208(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1192(%rbp), %rax
    movq %rax, -1200(%rbp)
    movq -1200(%rbp), %rax
    addq $8, %rax
    movq %rax, -1200(%rbp)
    movq -1200(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    jmp solve_14
solve_13:
    leaq .LC6(%rip), %rax
    movq %rax, -1200(%rbp)
    subq $32, %rsp
    movq -1200(%rbp), %rcx
    call printf
    addq $32, %rsp
    movl %eax, -1200(%rbp)
    leaq -1192(%rbp), %rax
    movq %rax, -1200(%rbp)
    movq -1200(%rbp), %rax
    addq $8, %rax
    movq %rax, -1200(%rbp)
    movq -1200(%rbp), %rax
    movl $2, %ecx
    movl %ecx, (%rax)
    leaq -1192(%rbp), %rax
    movq %rax, -1200(%rbp)
    movsd .LC7(%rip), %xmm0
    movsd %xmm0, -1208(%rbp)
    movq -1200(%rbp), %rax
    movsd -1208(%rbp), %xmm2
    movsd %xmm2, (%rax)
solve_14:
    leaq -1192(%rbp), %rax
    movq %rax, -1200(%rbp)
    movq -1200(%rbp), %rdx
    movq -1064(%rbp), %rcx
    mov $40, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    mov %rbp, %rsp
    pop %rbp
    ret
.global parse
parse:
    push %rbp
    mov %rsp, %rbp
    subq $1632, %rsp
parse_0:
    movq %rcx, -1064(%rbp)
    movq %rdx, -1072(%rbp)
    movl %r8d, -1080(%rbp)
    movq %r9, -1088(%rbp)
    leaq -1128(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    addq $8, %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    leaq -1128(%rbp), %rax
    movq %rax, -1584(%rbp)
    movsd .LC8(%rip), %xmm0
    movsd %xmm0, -1592(%rbp)
    movq -1584(%rbp), %rax
    movsd -1592(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq -1136(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movl $-1, %ecx
    movl %ecx, (%rax)
    leaq -1144(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    leaq -1152(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movl $-1, %ecx
    movl %ecx, (%rax)
    leaq -1160(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    leaq -1168(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
parse_1:
    leaq -1168(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1584(%rbp)
    leaq -1080(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1592(%rbp)
    movl -1592(%rbp), %eax
    addl $1, %eax
    movl %eax, -1592(%rbp)
    movl -1584(%rbp), %eax
    cmpl -1592(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    testl %eax, %eax
    jz parse_25
parse_2:
    leaq -1072(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1584(%rbp)
    leaq -1168(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1592(%rbp)
    movslq -1592(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1592(%rbp)
    movq -1584(%rbp), %rax
    addq -1592(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movb (%rax), %al
    movb %al, -1584(%rbp)
    movsbl -1584(%rbp), %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $40, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    testl %eax, %eax
    jz parse_4
parse_3:
    leaq -1160(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1592(%rbp)
    movl -1592(%rbp), %eax
    addl $1, %eax
    movl %eax, -1592(%rbp)
    movq -1584(%rbp), %rax
    movl -1592(%rbp), %ecx
    movl %ecx, (%rax)
    jmp parse_7
parse_4:
    leaq -1072(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1584(%rbp)
    leaq -1168(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1592(%rbp)
    movslq -1592(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1592(%rbp)
    movq -1584(%rbp), %rax
    addq -1592(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movb (%rax), %al
    movb %al, -1584(%rbp)
    movsbl -1584(%rbp), %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $41, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    testl %eax, %eax
    jz parse_6
    leaq -1160(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    testl %eax, %eax
    jz parse_6
parse_5:
    leaq -1160(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1592(%rbp)
    movl -1592(%rbp), %eax
    subl $1, %eax
    movl %eax, -1592(%rbp)
    movq -1584(%rbp), %rax
    movl -1592(%rbp), %ecx
    movl %ecx, (%rax)
    jmp parse_6
parse_6:
parse_7:
    leaq -1160(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    testl %eax, %eax
    jz parse_21
parse_8:
    leaq -1072(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1584(%rbp)
    leaq -1168(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1592(%rbp)
    movslq -1592(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1592(%rbp)
    movq -1584(%rbp), %rax
    addq -1592(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movb (%rax), %al
    movb %al, -1584(%rbp)
    subq $32, %rsp
    movb -1584(%rbp), %cl
    call is_operator
    addq $32, %rsp
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    testl %eax, %eax
    jz parse_14
    leaq -1088(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    testl %eax, %eax
    jz parse_14
parse_9:
    leaq -1176(%rbp), %rax
    movq %rax, -1584(%rbp)
    leaq -1072(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1592(%rbp)
    leaq -1168(%rbp), %rax
    movq %rax, -1600(%rbp)
    movq -1600(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1600(%rbp)
    movslq -1600(%rbp), %rax
    movq %rax, -1600(%rbp)
    movq -1600(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1600(%rbp)
    movq -1592(%rbp), %rax
    addq -1600(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movb (%rax), %al
    movb %al, -1592(%rbp)
    subq $32, %rsp
    movb -1592(%rbp), %cl
    call op_priority
    addq $32, %rsp
    movl %eax, -1592(%rbp)
    movq -1584(%rbp), %rax
    movl -1592(%rbp), %ecx
    movl %ecx, (%rax)
    leaq -1184(%rbp), %rax
    movq %rax, -1584(%rbp)
    leaq -1144(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movb (%rax), %al
    movb %al, -1592(%rbp)
    subq $32, %rsp
    movb -1592(%rbp), %cl
    call op_priority
    addq $32, %rsp
    movl %eax, -1592(%rbp)
    movq -1584(%rbp), %rax
    movl -1592(%rbp), %ecx
    movl %ecx, (%rax)
    leaq -1144(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movb (%rax), %al
    movb %al, -1584(%rbp)
    movsbl -1584(%rbp), %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $94, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    testl %eax, %eax
    leaq -1072(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1584(%rbp)
    leaq -1168(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1592(%rbp)
    movslq -1592(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1592(%rbp)
    movq -1584(%rbp), %rax
    addq -1592(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movb (%rax), %al
    movb %al, -1584(%rbp)
    movsbl -1584(%rbp), %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $94, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    testl %eax, %eax
    jz parse_13
parse_10:
    leaq -1136(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $-1, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    testl %eax, %eax
    leaq -1176(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1584(%rbp)
    leaq -1184(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1592(%rbp)
    movl -1584(%rbp), %eax
    cmpl -1592(%rbp), %eax
    setle %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    testl %eax, %eax
    jz parse_12
parse_11:
    leaq -1136(%rbp), %rax
    movq %rax, -1584(%rbp)
    leaq -1168(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1592(%rbp)
    movq -1584(%rbp), %rax
    movl -1592(%rbp), %ecx
    movl %ecx, (%rax)
    leaq -1144(%rbp), %rax
    movq %rax, -1584(%rbp)
    leaq -1072(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1592(%rbp)
    leaq -1168(%rbp), %rax
    movq %rax, -1600(%rbp)
    movq -1600(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1600(%rbp)
    movslq -1600(%rbp), %rax
    movq %rax, -1600(%rbp)
    movq -1600(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1600(%rbp)
    movq -1592(%rbp), %rax
    addq -1600(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movb (%rax), %al
    movb %al, -1592(%rbp)
    movq -1584(%rbp), %rax
    movb -1592(%rbp), %cl
    movb %cl, (%rax)
    jmp parse_12
parse_12:
    jmp parse_13
parse_13:
    jmp parse_20
parse_14:
    leaq -1072(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1584(%rbp)
    leaq -1168(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1592(%rbp)
    movslq -1592(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1592(%rbp)
    movq -1584(%rbp), %rax
    addq -1592(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movb (%rax), %al
    movb %al, -1584(%rbp)
    movsbl -1584(%rbp), %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $61, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    testl %eax, %eax
    jz parse_19
parse_15:
    leaq -1088(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $1, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    testl %eax, %eax
    jz parse_17
parse_16:
    leaq .LC9(%rip), %rax
    movq %rax, -1584(%rbp)
    leaq .LC10(%rip), %rax
    movq %rax, -1592(%rbp)
    subq $32, %rsp
    movq -1584(%rbp), %rcx
    movq -1592(%rbp), %rdx
    call printError
    addq $32, %rsp
    subq $32, %rsp
    movl $0, %ecx
    call exit
    addq $32, %rsp
    jmp parse_18
parse_17:
    leaq -1088(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movl $1, %ecx
    movl %ecx, (%rax)
    leaq -1136(%rbp), %rax
    movq %rax, -1584(%rbp)
    leaq -1168(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1592(%rbp)
    movq -1584(%rbp), %rax
    movl -1592(%rbp), %ecx
    movl %ecx, (%rax)
    leaq -1144(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movb $61, %cl
    movb %cl, (%rax)
    jmp parse_18
parse_18:
    jmp parse_19
parse_19:
parse_20:
    jmp parse_21
parse_21:
    leaq -1072(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1584(%rbp)
    leaq -1168(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1592(%rbp)
    movslq -1592(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1592(%rbp)
    movq -1584(%rbp), %rax
    addq -1592(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movb (%rax), %al
    movb %al, -1584(%rbp)
    movsbl -1584(%rbp), %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    testl %eax, %eax
    jz parse_23
parse_22:
    leaq -1152(%rbp), %rax
    movq %rax, -1584(%rbp)
    leaq -1168(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1592(%rbp)
    movq -1584(%rbp), %rax
    movl -1592(%rbp), %ecx
    movl %ecx, (%rax)
    jmp parse_25
    jmp parse_23
parse_23:
    jmp parse_24
parse_24:
    leaq -1168(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1592(%rbp)
    movl -1592(%rbp), %eax
    addl $1, %eax
    movl %eax, -1592(%rbp)
    movq -1584(%rbp), %rax
    movl -1592(%rbp), %ecx
    movl %ecx, (%rax)
    jmp parse_1
parse_25:
    leaq -1152(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $-1, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    testl %eax, %eax
    jz parse_27
parse_26:
    leaq .LC11(%rip), %rax
    movq %rax, -1584(%rbp)
    leaq .LC12(%rip), %rax
    movq %rax, -1592(%rbp)
    subq $32, %rsp
    movq -1584(%rbp), %rcx
    movq -1592(%rbp), %rdx
    call printError
    addq $32, %rsp
    subq $32, %rsp
    movl $1, %ecx
    call exit
    addq $32, %rsp
    jmp parse_27
parse_27:
    leaq -1160(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $0, %eax
    setg %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    testl %eax, %eax
    jz parse_29
parse_28:
    leaq .LC13(%rip), %rax
    movq %rax, -1584(%rbp)
    leaq .LC14(%rip), %rax
    movq %rax, -1592(%rbp)
    subq $32, %rsp
    movq -1584(%rbp), %rcx
    movq -1592(%rbp), %rdx
    call printError
    addq $32, %rsp
    subq $32, %rsp
    movl $1, %ecx
    call exit
    addq $32, %rsp
    jmp parse_29
parse_29:
    leaq -1136(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $-1, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    testl %eax, %eax
    jz parse_31
    leaq -1072(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1584(%rbp)
    movq $0, %rax
    imulq $1, %rax
    movq %rax, -1592(%rbp)
    movq -1584(%rbp), %rax
    addq -1592(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movb (%rax), %al
    movb %al, -1584(%rbp)
    movsbl -1584(%rbp), %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $40, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    testl %eax, %eax
    jz parse_31
    leaq -1072(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1584(%rbp)
    leaq -1152(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1592(%rbp)
    movl -1592(%rbp), %eax
    subl $1, %eax
    movl %eax, -1592(%rbp)
    movslq -1592(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1592(%rbp)
    movq -1584(%rbp), %rax
    addq -1592(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movb (%rax), %al
    movb %al, -1584(%rbp)
    movsbl -1584(%rbp), %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $41, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    testl %eax, %eax
    jz parse_31
parse_30:
    leaq -1072(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1584(%rbp)
    leaq -1152(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1592(%rbp)
    movl -1592(%rbp), %eax
    subl $1, %eax
    movl %eax, -1592(%rbp)
    movslq -1592(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1592(%rbp)
    movq -1584(%rbp), %rax
    addq -1592(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    leaq -1224(%rbp), %rax
    movq %rax, -1584(%rbp)
    leaq -1072(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1592(%rbp)
    movq $1, %rax
    imulq $1, %rax
    movq %rax, -1600(%rbp)
    movq -1592(%rbp), %rax
    addq -1600(%rbp), %rax
    movq %rax, -1592(%rbp)
    leaq -1152(%rbp), %rax
    movq %rax, -1600(%rbp)
    movq -1600(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1600(%rbp)
    movl -1600(%rbp), %eax
    subl $1, %eax
    movl %eax, -1600(%rbp)
    leaq -1088(%rbp), %rax
    movq %rax, -1608(%rbp)
    movq -1608(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1608(%rbp)
    subq $32, %rsp
    movq -1584(%rbp), %rcx
    movq -1592(%rbp), %rdx
    movl -1600(%rbp), %r8d
    movq -1608(%rbp), %r9
    call parse
    addq $32, %rsp
    movq -1584(%rbp), %rdx
    movq -1064(%rbp), %rcx
    mov $40, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp parse_31
parse_31:
    leaq -1136(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $-1, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    testl %eax, %eax
    jz parse_33
parse_32:
    leaq -1224(%rbp), %rax
    movq %rax, -1584(%rbp)
    leaq -1072(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1592(%rbp)
    subq $32, %rsp
    movq -1584(%rbp), %rcx
    movq -1592(%rbp), %rdx
    call parseToken
    addq $32, %rsp
    movq -1584(%rbp), %rdx
    movq -1064(%rbp), %rcx
    mov $40, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp parse_48
parse_33:
    leaq -1272(%rbp), %rax
    movq %rax, -1584(%rbp)
    leaq -1072(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1592(%rbp)
    leaq -1136(%rbp), %rax
    movq %rax, -1600(%rbp)
    movq -1600(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1600(%rbp)
    movslq -1600(%rbp), %rax
    movq %rax, -1600(%rbp)
    movq -1600(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1600(%rbp)
    movq -1592(%rbp), %rax
    addq -1600(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movb (%rax), %al
    movb %al, -1592(%rbp)
    movq -1584(%rbp), %rax
    movb -1592(%rbp), %cl
    movb %cl, (%rax)
    leaq -1280(%rbp), %rax
    movq %rax, -1584(%rbp)
    leaq -1136(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1592(%rbp)
    movslq -1592(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1584(%rbp), %rax
    movq -1592(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -1288(%rbp), %rax
    movq %rax, -1584(%rbp)
    leaq -1280(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1592(%rbp)
    movq $1, %rax
    mulq -1592(%rbp)
    movq %rax, -1592(%rbp)
    subq $32, %rsp
    movq -1592(%rbp), %rcx
    call malloc
    addq $32, %rsp
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1584(%rbp), %rax
    movq -1592(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -1288(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movq %rax, -1584(%rbp)
    leaq -1072(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movq %rax, -1592(%rbp)
    leaq -1280(%rbp), %rax
    movq %rax, -1600(%rbp)
    movq -1600(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1600(%rbp)
    movq $1, %rax
    mulq -1600(%rbp)
    movq %rax, -1600(%rbp)
    subq $32, %rsp
    movq -1584(%rbp), %rcx
    movq -1592(%rbp), %rdx
    movq -1600(%rbp), %r8
    call memcpy
    addq $32, %rsp
    movq %rax, -1584(%rbp)
    leaq -1288(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1584(%rbp)
    leaq -1280(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1592(%rbp)
    movq -1584(%rbp), %rax
    addq -1592(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    leaq -1296(%rbp), %rax
    movq %rax, -1584(%rbp)
    leaq -1152(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1592(%rbp)
    leaq -1136(%rbp), %rax
    movq %rax, -1600(%rbp)
    movq -1600(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1600(%rbp)
    movl -1592(%rbp), %eax
    subl -1600(%rbp), %eax
    movl %eax, -1592(%rbp)
    movl -1592(%rbp), %eax
    subl $1, %eax
    movl %eax, -1592(%rbp)
    movslq -1592(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1584(%rbp), %rax
    movq -1592(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -1304(%rbp), %rax
    movq %rax, -1584(%rbp)
    leaq -1296(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1592(%rbp)
    movq $1, %rax
    mulq -1592(%rbp)
    movq %rax, -1592(%rbp)
    subq $32, %rsp
    movq -1592(%rbp), %rcx
    call malloc
    addq $32, %rsp
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1584(%rbp), %rax
    movq -1592(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -1304(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1584(%rbp)
    leaq -1296(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1592(%rbp)
    movq -1584(%rbp), %rax
    addq -1592(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    leaq -1304(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movq %rax, -1584(%rbp)
    leaq -1072(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1592(%rbp)
    leaq -1136(%rbp), %rax
    movq %rax, -1600(%rbp)
    movq -1600(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1600(%rbp)
    movslq -1600(%rbp), %rax
    movq %rax, -1600(%rbp)
    movq -1600(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1600(%rbp)
    movq -1592(%rbp), %rax
    addq -1600(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq $1, %rax
    imulq $1, %rax
    movq %rax, -1600(%rbp)
    movq -1592(%rbp), %rax
    addq -1600(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movq %rax, -1592(%rbp)
    leaq -1296(%rbp), %rax
    movq %rax, -1600(%rbp)
    movq -1600(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1600(%rbp)
    movq $1, %rax
    mulq -1600(%rbp)
    movq %rax, -1600(%rbp)
    subq $32, %rsp
    movq -1584(%rbp), %rcx
    movq -1592(%rbp), %rdx
    movq -1600(%rbp), %r8
    call memcpy
    addq $32, %rsp
    movq %rax, -1584(%rbp)
    leaq -1088(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $1, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    testl %eax, %eax
    jz parse_47
parse_34:
    leaq -1312(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    leaq -1320(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
parse_35:
    leaq -1320(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1584(%rbp)
    leaq variableCount(%rip), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1592(%rbp)
    movl -1584(%rbp), %eax
    cmpl -1592(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    testl %eax, %eax
    jz parse_40
parse_36:
    leaq variableNames(%rip), %rax
    movq %rax, -1584(%rbp)
    leaq -1320(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1592(%rbp)
    movslq -1592(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    imulq $8, %rax
    movq %rax, -1592(%rbp)
    movq -1584(%rbp), %rax
    addq -1592(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movq %rax, -1584(%rbp)
    leaq -1288(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movq %rax, -1592(%rbp)
    subq $32, %rsp
    movq -1584(%rbp), %rcx
    movq -1592(%rbp), %rdx
    call strcmp
    addq $32, %rsp
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    testl %eax, %eax
    jz parse_38
parse_37:
    leaq -1312(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movl $1, %ecx
    movl %ecx, (%rax)
    leaq variables(%rip), %rax
    movq %rax, -1584(%rbp)
    leaq -1320(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1592(%rbp)
    movslq -1592(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    imulq $40, %rax
    movq %rax, -1592(%rbp)
    movq -1584(%rbp), %rax
    addq -1592(%rbp), %rax
    movq %rax, -1584(%rbp)
    leaq -1224(%rbp), %rax
    movq %rax, -1592(%rbp)
    leaq -1304(%rbp), %rax
    movq %rax, -1600(%rbp)
    movq -1600(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1600(%rbp)
    leaq -1296(%rbp), %rax
    movq %rax, -1608(%rbp)
    movq -1608(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1608(%rbp)
    movq -1608(%rbp), %rax
    movl %eax, -1608(%rbp)
    leaq -1088(%rbp), %rax
    movq %rax, -1616(%rbp)
    movq -1616(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1616(%rbp)
    subq $32, %rsp
    movq -1592(%rbp), %rcx
    movq -1600(%rbp), %rdx
    movl -1608(%rbp), %r8d
    movq -1616(%rbp), %r9
    call parse
    addq $32, %rsp
    movq -1592(%rbp), %rdx
    movq -1584(%rbp), %rcx
    mov $40, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    leaq -1128(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rdx
    movq -1064(%rbp), %rcx
    mov $40, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp parse_38
parse_38:
    jmp parse_39
parse_39:
    leaq -1320(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1592(%rbp)
    movl -1592(%rbp), %eax
    addl $1, %eax
    movl %eax, -1592(%rbp)
    movq -1584(%rbp), %rax
    movl -1592(%rbp), %ecx
    movl %ecx, (%rax)
    jmp parse_35
parse_40:
    leaq -1312(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    testl %eax, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    testl %eax, %eax
    jz parse_46
parse_41:
    leaq -1288(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1584(%rbp)
    movq $0, %rax
    imulq $1, %rax
    movq %rax, -1592(%rbp)
    movq -1584(%rbp), %rax
    addq -1592(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movb (%rax), %al
    movb %al, -1584(%rbp)
    subq $32, %rsp
    movb -1584(%rbp), %cl
    call isAlpha
    addq $32, %rsp
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    testl %eax, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    testl %eax, %eax
    jz parse_43
parse_42:
    leaq .LC15(%rip), %rax
    movq %rax, -1584(%rbp)
    leaq -1288(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1592(%rbp)
    subq $32, %rsp
    movq -1584(%rbp), %rcx
    movq -1592(%rbp), %rdx
    call printError
    addq $32, %rsp
    subq $32, %rsp
    movl $1, %ecx
    call exit
    addq $32, %rsp
    jmp parse_43
parse_43:
    leaq -1368(%rbp), %rax
    movq %rax, -1584(%rbp)
    leaq -1280(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1592(%rbp)
    movq $1, %rax
    mulq -1592(%rbp)
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    addq $1, %rax
    movq %rax, -1592(%rbp)
    movq -1584(%rbp), %rax
    movq -1592(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -1376(%rbp), %rax
    movq %rax, -1584(%rbp)
    leaq -1368(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1592(%rbp)
    subq $32, %rsp
    movq -1592(%rbp), %rcx
    call malloc
    addq $32, %rsp
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1584(%rbp), %rax
    movq -1592(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -1376(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1584(%rbp)
    movl -1584(%rbp), %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1584(%rbp)
    movl -1584(%rbp), %eax
    testl %eax, %eax
    jz parse_45
parse_44:
    subq $32, %rsp
    movl $1, %ecx
    call exit
    addq $32, %rsp
    jmp parse_45
parse_45:
    leaq -1376(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movq %rax, -1584(%rbp)
    leaq -1288(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movq %rax, -1592(%rbp)
    leaq -1368(%rbp), %rax
    movq %rax, -1600(%rbp)
    movq -1600(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1600(%rbp)
    subq $32, %rsp
    movq -1584(%rbp), %rcx
    movq -1592(%rbp), %rdx
    movq -1600(%rbp), %r8
    call memcpy
    addq $32, %rsp
    movq %rax, -1584(%rbp)
    leaq variableNames(%rip), %rax
    movq %rax, -1584(%rbp)
    leaq variableCount(%rip), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1592(%rbp)
    movslq -1592(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    imulq $8, %rax
    movq %rax, -1592(%rbp)
    movq -1584(%rbp), %rax
    addq -1592(%rbp), %rax
    movq %rax, -1584(%rbp)
    leaq -1376(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1592(%rbp)
    movq -1584(%rbp), %rax
    movq -1592(%rbp), %rcx
    movq %rcx, (%rax)
    leaq variables(%rip), %rax
    movq %rax, -1584(%rbp)
    leaq variableCount(%rip), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1592(%rbp)
    movslq -1592(%rbp), %rax
    movq %rax, -1592(%rbp)
    movq -1592(%rbp), %rax
    imulq $40, %rax
    movq %rax, -1592(%rbp)
    movq -1584(%rbp), %rax
    addq -1592(%rbp), %rax
    movq %rax, -1584(%rbp)
    leaq -1224(%rbp), %rax
    movq %rax, -1592(%rbp)
    leaq -1304(%rbp), %rax
    movq %rax, -1600(%rbp)
    movq -1600(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1600(%rbp)
    leaq -1296(%rbp), %rax
    movq %rax, -1608(%rbp)
    movq -1608(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1608(%rbp)
    movq -1608(%rbp), %rax
    movl %eax, -1608(%rbp)
    leaq -1088(%rbp), %rax
    movq %rax, -1616(%rbp)
    movq -1616(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1616(%rbp)
    subq $32, %rsp
    movq -1592(%rbp), %rcx
    movq -1600(%rbp), %rdx
    movl -1608(%rbp), %r8d
    movq -1616(%rbp), %r9
    call parse
    addq $32, %rsp
    movq -1592(%rbp), %rdx
    movq -1584(%rbp), %rcx
    mov $40, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    leaq variableCount(%rip), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1592(%rbp)
    movl -1592(%rbp), %eax
    addl $1, %eax
    movl %eax, -1592(%rbp)
    movq -1584(%rbp), %rax
    movl -1592(%rbp), %ecx
    movl %ecx, (%rax)
    leaq -1128(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rdx
    movq -1064(%rbp), %rcx
    mov $40, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp parse_46
parse_46:
    jmp parse_47
parse_47:
    leaq -1456(%rbp), %rax
    movq %rax, -1584(%rbp)
    leaq -1224(%rbp), %rax
    movq %rax, -1592(%rbp)
    leaq -1224(%rbp), %rax
    movq %rax, -1600(%rbp)
    leaq -1288(%rbp), %rax
    movq %rax, -1608(%rbp)
    movq -1608(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1608(%rbp)
    leaq -1280(%rbp), %rax
    movq %rax, -1616(%rbp)
    movq -1616(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1616(%rbp)
    movq -1616(%rbp), %rax
    movl %eax, -1616(%rbp)
    leaq -1088(%rbp), %rax
    movq %rax, -1624(%rbp)
    movq -1624(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1624(%rbp)
    subq $32, %rsp
    movq -1600(%rbp), %rcx
    movq -1608(%rbp), %rdx
    movl -1616(%rbp), %r8d
    movq -1624(%rbp), %r9
    call parse
    addq $32, %rsp
    leaq -1224(%rbp), %rax
    movq %rax, -1608(%rbp)
    leaq -1304(%rbp), %rax
    movq %rax, -1616(%rbp)
    movq -1616(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1616(%rbp)
    leaq -1296(%rbp), %rax
    movq %rax, -1624(%rbp)
    movq -1624(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1624(%rbp)
    movq -1624(%rbp), %rax
    movl %eax, -1624(%rbp)
    leaq -1088(%rbp), %rax
    movq %rax, -1632(%rbp)
    movq -1632(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1632(%rbp)
    subq $32, %rsp
    movq -1608(%rbp), %rcx
    movq -1616(%rbp), %rdx
    movl -1624(%rbp), %r8d
    movq -1632(%rbp), %r9
    call parse
    addq $32, %rsp
    leaq -1272(%rbp), %rax
    movq %rax, -1616(%rbp)
    movq -1616(%rbp), %rax
    movb (%rax), %al
    movb %al, -1616(%rbp)
    subq $32, %rsp
    movq -1592(%rbp), %rcx
    movq -1600(%rbp), %rdx
    movq -1608(%rbp), %r8
    movb -1616(%rbp), %r9b
    call solve
    addq $32, %rsp
    movq -1592(%rbp), %rdx
    movq -1584(%rbp), %rcx
    mov $40, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    leaq -1288(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movq %rax, -1584(%rbp)
    subq $32, %rsp
    movq -1584(%rbp), %rcx
    call free
    addq $32, %rsp
    leaq -1304(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rax
    movq %rax, -1584(%rbp)
    subq $32, %rsp
    movq -1584(%rbp), %rcx
    call free
    addq $32, %rsp
    leaq -1456(%rbp), %rax
    movq %rax, -1584(%rbp)
    movq -1584(%rbp), %rdx
    movq -1064(%rbp), %rcx
    mov $40, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp parse_48
parse_48:
.global op_priority
op_priority:
    push %rbp
    mov %rsp, %rbp
    subq $1088, %rsp
op_priority_0:
    movb %cl, -1064(%rbp)
    leaq -1064(%rbp), %rax
    movq %rax, -1072(%rbp)
    movq -1072(%rbp), %rax
    movb (%rax), %al
    movb %al, -1072(%rbp)
    movsbl -1072(%rbp), %eax
    movl %eax, -1072(%rbp)
    movl -1072(%rbp), %eax
    cmpl $62, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1080(%rbp)
    movl -1080(%rbp), %eax
    testl %eax, %eax
    jz op_priority_1
    movl -1072(%rbp), %eax
    cmpl $60, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1080(%rbp)
    movl -1080(%rbp), %eax
    testl %eax, %eax
    jz op_priority_2
    movl -1072(%rbp), %eax
    cmpl $126, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1080(%rbp)
    movl -1080(%rbp), %eax
    testl %eax, %eax
    jz op_priority_3
    movl -1072(%rbp), %eax
    cmpl $43, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1080(%rbp)
    movl -1080(%rbp), %eax
    testl %eax, %eax
    jz op_priority_4
    movl -1072(%rbp), %eax
    cmpl $45, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1080(%rbp)
    movl -1080(%rbp), %eax
    testl %eax, %eax
    jz op_priority_5
    movl -1072(%rbp), %eax
    cmpl $42, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1080(%rbp)
    movl -1080(%rbp), %eax
    testl %eax, %eax
    jz op_priority_6
    movl -1072(%rbp), %eax
    cmpl $120, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1080(%rbp)
    movl -1080(%rbp), %eax
    testl %eax, %eax
    jz op_priority_7
    movl -1072(%rbp), %eax
    cmpl $47, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1080(%rbp)
    movl -1080(%rbp), %eax
    testl %eax, %eax
    jz op_priority_8
    movl -1072(%rbp), %eax
    cmpl $94, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1080(%rbp)
    movl -1080(%rbp), %eax
    testl %eax, %eax
    jz op_priority_9
    movl -1072(%rbp), %eax
    cmpl $37, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1072(%rbp)
    movl -1072(%rbp), %eax
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
    subq $1088, %rsp
is_operator_0:
    movb %cl, -1064(%rbp)
    leaq -1064(%rbp), %rax
    movq %rax, -1072(%rbp)
    movq -1072(%rbp), %rax
    movb (%rax), %al
    movb %al, -1072(%rbp)
    movsbl -1072(%rbp), %eax
    movl %eax, -1072(%rbp)
    movl -1072(%rbp), %eax
    cmpl $43, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1080(%rbp)
    movl -1080(%rbp), %eax
    testl %eax, %eax
    jz is_operator_1
    movl -1072(%rbp), %eax
    cmpl $45, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1080(%rbp)
    movl -1080(%rbp), %eax
    testl %eax, %eax
    jz is_operator_2
    movl -1072(%rbp), %eax
    cmpl $120, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1080(%rbp)
    movl -1080(%rbp), %eax
    testl %eax, %eax
    jz is_operator_3
    movl -1072(%rbp), %eax
    cmpl $42, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1080(%rbp)
    movl -1080(%rbp), %eax
    testl %eax, %eax
    jz is_operator_4
    movl -1072(%rbp), %eax
    cmpl $47, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1080(%rbp)
    movl -1080(%rbp), %eax
    testl %eax, %eax
    jz is_operator_5
    movl -1072(%rbp), %eax
    cmpl $37, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1080(%rbp)
    movl -1080(%rbp), %eax
    testl %eax, %eax
    jz is_operator_6
    movl -1072(%rbp), %eax
    cmpl $94, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1080(%rbp)
    movl -1080(%rbp), %eax
    testl %eax, %eax
    jz is_operator_7
    movl -1072(%rbp), %eax
    cmpl $62, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1080(%rbp)
    movl -1080(%rbp), %eax
    testl %eax, %eax
    jz is_operator_8
    movl -1072(%rbp), %eax
    cmpl $60, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1080(%rbp)
    movl -1080(%rbp), %eax
    testl %eax, %eax
    jz is_operator_9
    movl -1072(%rbp), %eax
    cmpl $126, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1072(%rbp)
    movl -1072(%rbp), %eax
    testl %eax, %eax
    jz is_operator_10
    jmp is_operator_11
is_operator_1:
is_operator_2:
is_operator_3:
is_operator_4:
is_operator_5:
is_operator_6:
is_operator_7:
is_operator_8:
is_operator_9:
is_operator_10:
    movl $1, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
is_operator_11:
    movl $0, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
is_operator_12:
.global isAlpha
isAlpha:
    push %rbp
    mov %rsp, %rbp
    subq $1088, %rsp
isAlpha_0:
    movb %cl, -1064(%rbp)
    leaq -1064(%rbp), %rax
    movq %rax, -1072(%rbp)
    movq -1072(%rbp), %rax
    movb (%rax), %al
    movb %al, -1072(%rbp)
    movsbl -1072(%rbp), %eax
    movl %eax, -1072(%rbp)
    movl -1072(%rbp), %eax
    cmpl $97, %eax
    setge %al
    movzbl %al, %eax
    movl %eax, -1072(%rbp)
    movl -1072(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1072(%rbp)
    leaq -1064(%rbp), %rax
    movq %rax, -1080(%rbp)
    movq -1080(%rbp), %rax
    movb (%rax), %al
    movb %al, -1080(%rbp)
    movsbl -1080(%rbp), %eax
    movl %eax, -1080(%rbp)
    movl -1080(%rbp), %eax
    cmpl $122, %eax
    setle %al
    movzbl %al, %eax
    movl %eax, -1080(%rbp)
    movl -1080(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1080(%rbp)
    movl -1072(%rbp), %eax
    andl -1080(%rbp), %eax
    movl %eax, -1072(%rbp)
    movl -1072(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1072(%rbp)
    leaq -1064(%rbp), %rax
    movq %rax, -1080(%rbp)
    movq -1080(%rbp), %rax
    movb (%rax), %al
    movb %al, -1080(%rbp)
    movsbl -1080(%rbp), %eax
    movl %eax, -1080(%rbp)
    movl -1080(%rbp), %eax
    cmpl $65, %eax
    setge %al
    movzbl %al, %eax
    movl %eax, -1080(%rbp)
    movl -1080(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1080(%rbp)
    leaq -1064(%rbp), %rax
    movq %rax, -1088(%rbp)
    movq -1088(%rbp), %rax
    movb (%rax), %al
    movb %al, -1088(%rbp)
    movsbl -1088(%rbp), %eax
    movl %eax, -1088(%rbp)
    movl -1088(%rbp), %eax
    cmpl $90, %eax
    setle %al
    movzbl %al, %eax
    movl %eax, -1088(%rbp)
    movl -1088(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1088(%rbp)
    movl -1080(%rbp), %eax
    andl -1088(%rbp), %eax
    movl %eax, -1080(%rbp)
    movl -1080(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1080(%rbp)
    movl -1072(%rbp), %eax
    orl -1080(%rbp), %eax
    movl %eax, -1072(%rbp)
    movl -1072(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
.global printError
printError:
    push %rbp
    mov %rsp, %rbp
    subq $1104, %rsp
printError_0:
    movq %rcx, -1064(%rbp)
    movq %rdx, -1072(%rbp)
    leaq .LC16(%rip), %rax
    movq %rax, -1080(%rbp)
    leaq -1064(%rbp), %rax
    movq %rax, -1088(%rbp)
    movq -1088(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1088(%rbp)
    leaq -1072(%rbp), %rax
    movq %rax, -1096(%rbp)
    movq -1096(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1096(%rbp)
    subq $32, %rsp
    movq -1080(%rbp), %rcx
    movq -1088(%rbp), %rdx
    movq -1096(%rbp), %r8
    call printf
    addq $32, %rsp
    movl %eax, -1080(%rbp)
    mov %rbp, %rsp
    pop %rbp
    ret
.global printVar
printVar:
    push %rbp
    mov %rsp, %rbp
    subq $1120, %rsp
printVar_0:
    movq %rcx, %rax
    movq %rax, -1112(%rbp)
    leaq -1096(%rbp), %rax
    movq %rax, -1120(%rbp)
    movq -1112(%rbp), %rdx
    movq -1120(%rbp), %rcx
    mov $40, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    leaq -1104(%rbp), %rax
    movq %rax, -1112(%rbp)
    leaq -1096(%rbp), %rax
    movq %rax, -1120(%rbp)
    movq -1120(%rbp), %rax
    addq $8, %rax
    movq %rax, -1120(%rbp)
    movq -1120(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1120(%rbp)
    movq -1112(%rbp), %rax
    movl -1120(%rbp), %ecx
    movl %ecx, (%rax)
    leaq -1104(%rbp), %rax
    movq %rax, -1112(%rbp)
    movq -1112(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1112(%rbp)
    movl -1112(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1120(%rbp)
    movl -1120(%rbp), %eax
    testl %eax, %eax
    jz printVar_1
    movl -1112(%rbp), %eax
    cmpl $1, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1112(%rbp)
    movl -1112(%rbp), %eax
    testl %eax, %eax
    jz printVar_2
    jmp printVar_6
printVar_1:
    leaq .LC17(%rip), %rax
    movq %rax, -1112(%rbp)
    leaq -1096(%rbp), %rax
    movq %rax, -1120(%rbp)
    movq -1120(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1120(%rbp)
    subq $32, %rsp
    movq -1112(%rbp), %rcx
    movq -1120(%rbp), %rdx
    movsd -1120(%rbp), %xmm1
    call printf
    addq $32, %rsp
    movl %eax, -1112(%rbp)
    jmp printVar_7
printVar_2:
    leaq -1096(%rbp), %rax
    movq %rax, -1112(%rbp)
    movq -1112(%rbp), %rax
    movsd (%rax), %xmm0
    movsd %xmm0, -1112(%rbp)
    cvttsd2si -1112(%rbp), %eax
    movl %eax, -1112(%rbp)
    movl -1112(%rbp), %eax
    testl %eax, %eax
    jz printVar_4
printVar_3:
    leaq .LC18(%rip), %rax
    movq %rax, -1112(%rbp)
    subq $32, %rsp
    movq -1112(%rbp), %rcx
    call printf
    addq $32, %rsp
    movl %eax, -1112(%rbp)
    jmp printVar_5
printVar_4:
    leaq .LC19(%rip), %rax
    movq %rax, -1112(%rbp)
    subq $32, %rsp
    movq -1112(%rbp), %rcx
    call printf
    addq $32, %rsp
    movl %eax, -1112(%rbp)
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
    subq $1280, %rsp
main_0:
    leaq CONSTANTS_NAMES(%rip), %rax
    movq %rax, -1248(%rbp)
    movq $0, %rax
    imulq $8, %rax
    movq %rax, -1256(%rbp)
    movq -1248(%rbp), %rax
    addq -1256(%rbp), %rax
    movq %rax, -1248(%rbp)
    leaq .LC20(%rip), %rax
    movq %rax, -1256(%rbp)
    movq -1248(%rbp), %rax
    movq -1256(%rbp), %rcx
    movq %rcx, (%rax)
    leaq CONSTANTS_NAMES(%rip), %rax
    movq %rax, -1248(%rbp)
    movq $1, %rax
    imulq $8, %rax
    movq %rax, -1256(%rbp)
    movq -1248(%rbp), %rax
    addq -1256(%rbp), %rax
    movq %rax, -1248(%rbp)
    leaq .LC21(%rip), %rax
    movq %rax, -1256(%rbp)
    movq -1248(%rbp), %rax
    movq -1256(%rbp), %rcx
    movq %rcx, (%rax)
    leaq CONSTANTS_NAMES(%rip), %rax
    movq %rax, -1248(%rbp)
    movq $2, %rax
    imulq $8, %rax
    movq %rax, -1256(%rbp)
    movq -1248(%rbp), %rax
    addq -1256(%rbp), %rax
    movq %rax, -1248(%rbp)
    leaq .LC22(%rip), %rax
    movq %rax, -1256(%rbp)
    movq -1248(%rbp), %rax
    movq -1256(%rbp), %rcx
    movq %rcx, (%rax)
    leaq CONSTANTS_VALUES(%rip), %rax
    movq %rax, -1248(%rbp)
    movq $0, %rax
    imulq $8, %rax
    movq %rax, -1256(%rbp)
    movq -1248(%rbp), %rax
    addq -1256(%rbp), %rax
    movq %rax, -1248(%rbp)
    movsd .LC23(%rip), %xmm0
    movsd %xmm0, -1256(%rbp)
    movq -1248(%rbp), %rax
    movsd -1256(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq CONSTANTS_VALUES(%rip), %rax
    movq %rax, -1248(%rbp)
    movq $1, %rax
    imulq $8, %rax
    movq %rax, -1256(%rbp)
    movq -1248(%rbp), %rax
    addq -1256(%rbp), %rax
    movq %rax, -1248(%rbp)
    movsd .LC24(%rip), %xmm0
    movsd %xmm0, -1256(%rbp)
    movq -1248(%rbp), %rax
    movsd -1256(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq CONSTANTS_VALUES(%rip), %rax
    movq %rax, -1248(%rbp)
    movq $2, %rax
    imulq $8, %rax
    movq %rax, -1256(%rbp)
    movq -1248(%rbp), %rax
    addq -1256(%rbp), %rax
    movq %rax, -1248(%rbp)
    movsd .LC25(%rip), %xmm0
    movsd %xmm0, -1256(%rbp)
    movq -1248(%rbp), %rax
    movsd -1256(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq .LC26(%rip), %rax
    movq %rax, -1248(%rbp)
    subq $32, %rsp
    movq -1248(%rbp), %rcx
    call printf
    addq $32, %rsp
    movl %eax, -1248(%rbp)
    leaq last_result(%rip), %rax
    movq %rax, -1248(%rbp)
    movsd .LC27(%rip), %xmm0
    movsd %xmm0, -1256(%rbp)
    movq -1248(%rbp), %rax
    movsd -1256(%rbp), %xmm2
    movsd %xmm2, (%rax)
    leaq last_result(%rip), %rax
    movq %rax, -1248(%rbp)
    movq -1248(%rbp), %rax
    addq $8, %rax
    movq %rax, -1248(%rbp)
    movq -1248(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    leaq -1064(%rbp), %rax
    movq %rax, -1248(%rbp)
    movl $1, %eax
    imull $100, %eax
    movl %eax, -1256(%rbp)
    movslq -1256(%rbp), %rax
    movq %rax, -1256(%rbp)
    subq $32, %rsp
    movq -1256(%rbp), %rcx
    call malloc
    addq $32, %rsp
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movq %rax, -1256(%rbp)
    movq -1248(%rbp), %rax
    movq -1256(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -1064(%rbp), %rax
    movq %rax, -1248(%rbp)
    movq -1248(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1248(%rbp)
    movl -1248(%rbp), %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1248(%rbp)
    movl -1248(%rbp), %eax
    testl %eax, %eax
    jz main_2
main_1:
    subq $32, %rsp
    movl $1, %ecx
    call exit
    addq $32, %rsp
    jmp main_2
main_2:
main_3:
    movl $1, %eax
    testl %eax, %eax
    jz main_18
main_4:
    leaq -1168(%rbp), %rax
    movq %rax, -1248(%rbp)
    movq -1248(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $1, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $2, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $3, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $4, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $5, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $6, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $7, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $8, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $9, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $10, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $11, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $12, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $13, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $14, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $15, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $16, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $17, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $18, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $19, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $20, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $21, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $22, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $23, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $24, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $25, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $26, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $27, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $28, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $29, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $30, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $31, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $32, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $33, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $34, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $35, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $36, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $37, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $38, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $39, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $40, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $41, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $42, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $43, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $44, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $45, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $46, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $47, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $48, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $49, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $50, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $51, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $52, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $53, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $54, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $55, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $56, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $57, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $58, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $59, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $60, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $61, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $62, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $63, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $64, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $65, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $66, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $67, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $68, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $69, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $70, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $71, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $72, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $73, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $74, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $75, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $76, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $77, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $78, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $79, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $80, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $81, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $82, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $83, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $84, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $85, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $86, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $87, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $88, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $89, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $90, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $91, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $92, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $93, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $94, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $95, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $96, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $97, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $98, %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    movq -1248(%rbp), %rax
    addq $99, %rax
    movq %rax, -1248(%rbp)
    movq -1248(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    leaq .LC28(%rip), %rax
    movq %rax, -1248(%rbp)
    subq $32, %rsp
    movq -1248(%rbp), %rcx
    call printf
    addq $32, %rsp
    movl %eax, -1248(%rbp)
    leaq -1176(%rbp), %rax
    movq %rax, -1248(%rbp)
    leaq .LC29(%rip), %rax
    movq %rax, -1256(%rbp)
    subq $32, %rsp
    movl $0, %ecx
    movq -1256(%rbp), %rdx
    call fdopen
    addq $32, %rsp
    movq %rax, -1256(%rbp)
    movq -1248(%rbp), %rax
    movq -1256(%rbp), %rcx
    movq %rcx, (%rax)
    leaq -1168(%rbp), %rax
    movq %rax, -1248(%rbp)
    leaq -1176(%rbp), %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1256(%rbp)
    subq $32, %rsp
    movq -1248(%rbp), %rcx
    movl $100, %edx
    movq -1256(%rbp), %r8
    call fgets
    addq $32, %rsp
    movq %rax, -1248(%rbp)
    leaq -1168(%rbp), %rax
    movq %rax, -1248(%rbp)
    movq $0, %rax
    imulq $1, %rax
    movq %rax, -1256(%rbp)
    movq -1248(%rbp), %rax
    addq -1256(%rbp), %rax
    movq %rax, -1248(%rbp)
    movq -1248(%rbp), %rax
    movb (%rax), %al
    movb %al, -1248(%rbp)
    movsbl -1248(%rbp), %eax
    movl %eax, -1248(%rbp)
    movl -1248(%rbp), %eax
    cmpl $10, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1248(%rbp)
    movl -1248(%rbp), %eax
    testl %eax, %eax
    jz main_6
main_5:
    leaq .LC30(%rip), %rax
    movq %rax, -1248(%rbp)
    subq $32, %rsp
    movq -1248(%rbp), %rcx
    call printf
    addq $32, %rsp
    movl %eax, -1248(%rbp)
    movl $0, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp main_9
main_6:
    leaq -1168(%rbp), %rax
    movq %rax, -1248(%rbp)
    leaq .LC31(%rip), %rax
    movq %rax, -1256(%rbp)
    subq $32, %rsp
    movq -1248(%rbp), %rcx
    movq -1256(%rbp), %rdx
    call strcmp
    addq $32, %rsp
    movl %eax, -1248(%rbp)
    movl -1248(%rbp), %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1248(%rbp)
    movl -1248(%rbp), %eax
    testl %eax, %eax
    jz main_8
main_7:
    leaq .LC32(%rip), %rax
    movq %rax, -1248(%rbp)
    subq $32, %rsp
    movq -1248(%rbp), %rcx
    call printf
    addq $32, %rsp
    movl %eax, -1248(%rbp)
    movl $0, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp main_8
main_8:
main_9:
    leaq -1184(%rbp), %rax
    movq %rax, -1248(%rbp)
    movq -1248(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
main_10:
    leaq -1184(%rbp), %rax
    movq %rax, -1248(%rbp)
    movq -1248(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1248(%rbp)
    movl -1248(%rbp), %eax
    cmpl $100, %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -1248(%rbp)
    movl -1248(%rbp), %eax
    testl %eax, %eax
    jz main_15
main_11:
    leaq -1168(%rbp), %rax
    movq %rax, -1248(%rbp)
    leaq -1184(%rbp), %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1256(%rbp)
    movslq -1256(%rbp), %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1256(%rbp)
    movq -1248(%rbp), %rax
    addq -1256(%rbp), %rax
    movq %rax, -1248(%rbp)
    movq -1248(%rbp), %rax
    movb (%rax), %al
    movb %al, -1248(%rbp)
    movsbl -1248(%rbp), %eax
    movl %eax, -1248(%rbp)
    movl -1248(%rbp), %eax
    cmpl $10, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1248(%rbp)
    movl -1248(%rbp), %eax
    testl %eax, %eax
    jz main_13
main_12:
    leaq -1168(%rbp), %rax
    movq %rax, -1248(%rbp)
    leaq -1184(%rbp), %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1256(%rbp)
    movslq -1256(%rbp), %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    imulq $1, %rax
    movq %rax, -1256(%rbp)
    movq -1248(%rbp), %rax
    addq -1256(%rbp), %rax
    movq %rax, -1248(%rbp)
    movq -1248(%rbp), %rax
    movb $0, %cl
    movb %cl, (%rax)
    jmp main_15
    jmp main_13
main_13:
    jmp main_14
main_14:
    leaq -1184(%rbp), %rax
    movq %rax, -1248(%rbp)
    movq -1248(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1256(%rbp)
    movl -1256(%rbp), %eax
    addl $1, %eax
    movl %eax, -1256(%rbp)
    movq -1248(%rbp), %rax
    movl -1256(%rbp), %ecx
    movl %ecx, (%rax)
    jmp main_10
main_15:
    leaq -1064(%rbp), %rax
    movq %rax, -1248(%rbp)
    movq -1248(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1248(%rbp)
    movq -1248(%rbp), %rax
    movq %rax, -1248(%rbp)
    leaq -1168(%rbp), %rax
    movq %rax, -1256(%rbp)
    movl $1, %eax
    imull $100, %eax
    movl %eax, -1264(%rbp)
    movslq -1264(%rbp), %rax
    movq %rax, -1264(%rbp)
    subq $32, %rsp
    movq -1248(%rbp), %rcx
    movq -1256(%rbp), %rdx
    movq -1264(%rbp), %r8
    call memcpy
    addq $32, %rsp
    movq %rax, -1248(%rbp)
    leaq -1168(%rbp), %rax
    movq %rax, -1248(%rbp)
    subq $32, %rsp
    movq -1248(%rbp), %rcx
    call format
    addq $32, %rsp
    leaq -1192(%rbp), %rax
    movq %rax, -1248(%rbp)
    movq -1248(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    leaq last_result(%rip), %rax
    movq %rax, -1248(%rbp)
    leaq -1232(%rbp), %rax
    movq %rax, -1256(%rbp)
    leaq -1168(%rbp), %rax
    movq %rax, -1264(%rbp)
    leaq -1192(%rbp), %rax
    movq %rax, -1272(%rbp)
    subq $32, %rsp
    movq -1256(%rbp), %rcx
    movq -1264(%rbp), %rdx
    movl $100, %r8d
    movq -1272(%rbp), %r9
    call parse
    addq $32, %rsp
    movq -1256(%rbp), %rdx
    movq -1248(%rbp), %rcx
    mov $40, %r8
    sub $32, %rsp
    call memcpy
    add $32, %rsp
    leaq -1192(%rbp), %rax
    movq %rax, -1248(%rbp)
    movq -1248(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1248(%rbp)
    movl -1248(%rbp), %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -1248(%rbp)
    movl -1248(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1248(%rbp)
    movl -1248(%rbp), %eax
    testl %eax, %eax
    jz main_17
    leaq last_result(%rip), %rax
    movq %rax, -1248(%rbp)
    movq -1248(%rbp), %rax
    addq $8, %rax
    movq %rax, -1248(%rbp)
    movq -1248(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1248(%rbp)
    movl -1248(%rbp), %eax
    cmpl $2, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1248(%rbp)
    movl -1248(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1248(%rbp)
    movl -1248(%rbp), %eax
    testl %eax, %eax
    jz main_17
main_16:
    leaq .LC33(%rip), %rax
    movq %rax, -1248(%rbp)
    leaq -1064(%rbp), %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1256(%rbp)
    subq $32, %rsp
    movq -1248(%rbp), %rcx
    movq -1256(%rbp), %rdx
    call printf
    addq $32, %rsp
    movl %eax, -1248(%rbp)
    leaq last_result(%rip), %rax
    movq %rax, -1248(%rbp)
    subq $32, %rsp
    movq -1248(%rbp), %rcx
    call printVar
    addq $32, %rsp
    leaq .LC34(%rip), %rax
    movq %rax, -1248(%rbp)
    subq $32, %rsp
    movq -1248(%rbp), %rcx
    call printf
    addq $32, %rsp
    movl %eax, -1248(%rbp)
    jmp main_17
main_17:
    jmp main_3
main_18:
    leaq -1240(%rbp), %rax
    movq %rax, -1248(%rbp)
    movq -1248(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
main_19:
    leaq -1240(%rbp), %rax
    movq %rax, -1248(%rbp)
    movq -1248(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1248(%rbp)
    movl -1248(%rbp), %eax
    cmpl $20, %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -1248(%rbp)
    movl -1248(%rbp), %eax
    testl %eax, %eax
    jz main_24
main_20:
    leaq variableNames(%rip), %rax
    movq %rax, -1248(%rbp)
    leaq -1240(%rbp), %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1256(%rbp)
    movslq -1256(%rbp), %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    imulq $8, %rax
    movq %rax, -1256(%rbp)
    movq -1248(%rbp), %rax
    addq -1256(%rbp), %rax
    movq %rax, -1248(%rbp)
    movq -1248(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1248(%rbp)
    movl -1248(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1248(%rbp)
    movl -1248(%rbp), %eax
    testl %eax, %eax
    jz main_22
main_21:
    leaq variableNames(%rip), %rax
    movq %rax, -1248(%rbp)
    leaq -1240(%rbp), %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1256(%rbp)
    movslq -1256(%rbp), %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    imulq $8, %rax
    movq %rax, -1256(%rbp)
    movq -1248(%rbp), %rax
    addq -1256(%rbp), %rax
    movq %rax, -1248(%rbp)
    movq -1248(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1248(%rbp)
    movq -1248(%rbp), %rax
    movq %rax, -1248(%rbp)
    subq $32, %rsp
    movq -1248(%rbp), %rcx
    call free
    addq $32, %rsp
    leaq variableNames(%rip), %rax
    movq %rax, -1248(%rbp)
    leaq -1240(%rbp), %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1256(%rbp)
    movslq -1256(%rbp), %rax
    movq %rax, -1256(%rbp)
    movq -1256(%rbp), %rax
    imulq $8, %rax
    movq %rax, -1256(%rbp)
    movq -1248(%rbp), %rax
    addq -1256(%rbp), %rax
    movq %rax, -1248(%rbp)
    movq -1248(%rbp), %rax
    movq $0, %rcx
    movq %rcx, (%rax)
    jmp main_22
main_22:
    jmp main_23
main_23:
    leaq -1240(%rbp), %rax
    movq %rax, -1248(%rbp)
    movq -1248(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -1256(%rbp)
    movl -1256(%rbp), %eax
    addl $1, %eax
    movl %eax, -1256(%rbp)
    movq -1248(%rbp), %rax
    movl -1256(%rbp), %ecx
    movl %ecx, (%rax)
    jmp main_19
main_24:
    leaq -1064(%rbp), %rax
    movq %rax, -1248(%rbp)
    movq -1248(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -1248(%rbp)
    movq -1248(%rbp), %rax
    movq %rax, -1248(%rbp)
    subq $32, %rsp
    movq -1248(%rbp), %rcx
    call free
    addq $32, %rsp
    movl $0, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
