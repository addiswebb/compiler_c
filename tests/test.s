.section .note.GNU-stack,"",@progbits
.section .rodata
.LC0:
    .byte 0x1B, '[', '3', '4', 'm', 'D', 'E', 'B', 'U', 'G', 0x1B, '[', '0', 'm', 0
.LC1:
    .byte 0x1B, '[', '3', '2', 'm', 'I', 'N', 'F', 'O', 0x1B, '[', '0', 'm', 0
.LC2:
    .byte 0x1B, '[', '3', '3', 'm', 'W', 'A', 'R', 'N', 0x1B, '[', '0', 'm', 0
.LC3:
    .byte 0x1B, '[', '3', '1', 'm', 'E', 'R', 'R', 'O', 'R', 0x1B, '[', '0', 'm', 0
.LC4:
    .byte 0x1B, '[', '1', ';', '3', '1', 'm', 'P', 'A', 'N', 'I', 'C', 0x1B, '[', '0', 'm', 0
.LC5:
    .byte 'L', 'O', 'G', 0
.LC6:
    .byte 'C', 'o', 'm', 'p', 'i', 'l', 'e', 'r', 0
.LC7:
    .byte 'T', 'o', 'k', 'e', 'n', 'i', 'z', 'e', 'r', 0
.LC8:
    .byte 'P', 'a', 'r', 's', 'e', 'r', 0
.LC9:
    .byte 'I', 'R', 0
.LC10:
    .byte 'x', '8', '6', ' ', 'G', 'e', 'n', 0
.LC11:
    .byte 'S', 'e', 'm', 'a', 'n', 't', 'i', 'c', ' ', 'A', 'n', 'a', 'l', 'y', 's', 'i', 's', 0
.LC12:
    .byte 'A', 's', 's', 'e', 'm', 'b', 'l', 'e', 'r', 0
.LC13:
    .byte 'L', 'i', 'n', 'k', 'e', 'r', 0
.LC14:
    .byte '[', '%', 's', ']', ' ', '%', 's', ':', ' ', 0
.LC15:
    .byte 'C', 'O', 'M', 'P', 'I', 'L', 'E', 'R', '_', 'C', ' ', 'c', 'o', 'm', 'p', 'i', 'l', 'e', 'd', ' ', 't', 'h', 'i', 's', ' ', 'o', 'M', 'G', 0x0A, 0

.text
init_logger:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
init_logger_0:
    movq %rdi, -8(%rbp)
    movl %esi, -16(%rbp)
    leaq logger(%rip), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    addq $8, %rax
    movq %rax, -24(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    cmpq $0, %rax
    sete %al
    movzbl %al, %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    testl %eax, %eax
    jnz init_logger_1
    leaq -8(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movq %rax, -40(%rbp)
    jmp init_logger_2
init_logger_1:
    leaq stderr(%rip), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movq %rax, -40(%rbp)
init_logger_2:
    movq -24(%rbp), %rax
    movq -40(%rbp), %rcx
    movq %rcx, (%rax)
    leaq logger(%rip), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    addq $4, %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    leaq logger(%rip), %rax
    movq %rax, -24(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -32(%rbp)
    movq -24(%rbp), %rax
    movl -32(%rbp), %ecx
    movl %ecx, (%rax)
    mov %rbp, %rsp
    pop %rbp
    ret
set_log_stage:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
set_log_stage_0:
    movl %edi, -8(%rbp)
    leaq logger(%rip), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    addq $4, %rax
    movq %rax, -16(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -24(%rbp)
    movq -16(%rbp), %rax
    movl -24(%rbp), %ecx
    movl %ecx, (%rax)
    mov %rbp, %rsp
    pop %rbp
    ret
log_start:
    push %rbp
    mov %rsp, %rbp
    subq $64, %rsp
log_start_0:
    movl %edi, -8(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -32(%rbp)
    leaq logger(%rip), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    movl -32(%rbp), %eax
    cmpl -40(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    testl %eax, %eax
    jz log_start_2
log_start_1:
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp log_start_2
log_start_2:
    leaq -8(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz log_start_3
    movl -32(%rbp), %eax
    cmpl $1, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz log_start_4
    movl -32(%rbp), %eax
    cmpl $2, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz log_start_5
    movl -32(%rbp), %eax
    cmpl $3, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz log_start_6
    movl -32(%rbp), %eax
    cmpl $4, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    testl %eax, %eax
    jz log_start_7
    jmp log_start_8
log_start_3:
    leaq -16(%rbp), %rax
    movq %rax, -32(%rbp)
    leaq .LC0(%rip), %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    movq -40(%rbp), %rcx
    movq %rcx, (%rax)
    jmp log_start_9
log_start_4:
    leaq -16(%rbp), %rax
    movq %rax, -32(%rbp)
    leaq .LC1(%rip), %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    movq -40(%rbp), %rcx
    movq %rcx, (%rax)
    jmp log_start_9
log_start_5:
    leaq -16(%rbp), %rax
    movq %rax, -32(%rbp)
    leaq .LC2(%rip), %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    movq -40(%rbp), %rcx
    movq %rcx, (%rax)
    jmp log_start_9
log_start_6:
    leaq -16(%rbp), %rax
    movq %rax, -32(%rbp)
    leaq .LC3(%rip), %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    movq -40(%rbp), %rcx
    movq %rcx, (%rax)
    jmp log_start_9
log_start_7:
    leaq -16(%rbp), %rax
    movq %rax, -32(%rbp)
    leaq .LC4(%rip), %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    movq -40(%rbp), %rcx
    movq %rcx, (%rax)
    jmp log_start_9
log_start_8:
    leaq -16(%rbp), %rax
    movq %rax, -32(%rbp)
    leaq .LC5(%rip), %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    movq -40(%rbp), %rcx
    movq %rcx, (%rax)
    jmp log_start_9
log_start_9:
    leaq logger(%rip), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    addq $4, %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz log_start_10
    movl -32(%rbp), %eax
    cmpl $1, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz log_start_11
    movl -32(%rbp), %eax
    cmpl $2, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz log_start_12
    movl -32(%rbp), %eax
    cmpl $4, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz log_start_13
    movl -32(%rbp), %eax
    cmpl $5, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz log_start_14
    movl -32(%rbp), %eax
    cmpl $3, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz log_start_15
    movl -32(%rbp), %eax
    cmpl $6, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz log_start_16
    movl -32(%rbp), %eax
    cmpl $7, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    testl %eax, %eax
    jz log_start_17
    jmp log_start_18
log_start_10:
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    leaq .LC6(%rip), %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    movq -40(%rbp), %rcx
    movq %rcx, (%rax)
    jmp log_start_19
log_start_11:
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    leaq .LC7(%rip), %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    movq -40(%rbp), %rcx
    movq %rcx, (%rax)
    jmp log_start_19
log_start_12:
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    leaq .LC8(%rip), %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    movq -40(%rbp), %rcx
    movq %rcx, (%rax)
    jmp log_start_19
log_start_13:
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    leaq .LC9(%rip), %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    movq -40(%rbp), %rcx
    movq %rcx, (%rax)
    jmp log_start_19
log_start_14:
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    leaq .LC10(%rip), %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    movq -40(%rbp), %rcx
    movq %rcx, (%rax)
    jmp log_start_19
log_start_15:
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    leaq .LC11(%rip), %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    movq -40(%rbp), %rcx
    movq %rcx, (%rax)
    jmp log_start_19
log_start_16:
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    leaq .LC12(%rip), %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    movq -40(%rbp), %rcx
    movq %rcx, (%rax)
    jmp log_start_19
log_start_17:
    leaq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    leaq .LC13(%rip), %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    movq -40(%rbp), %rcx
    movq %rcx, (%rax)
    jmp log_start_19
log_start_18:
    jmp log_start_19
log_start_19:
    leaq logger(%rip), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    addq $8, %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -32(%rbp)
    leaq .LC14(%rip), %rax
    movq %rax, -40(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -48(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -56(%rbp)
    subq $176, %rsp
    movq -32(%rbp), %rdi
    movq -40(%rbp), %rsi
    movq -48(%rbp), %rdx
    movq -56(%rbp), %rcx
    xor %eax, %eax
    call fprintf
    addq $176, %rsp
    movl %eax, -32(%rbp)
    mov %rbp, %rsp
    pop %rbp
    ret
log_message:
    push %rbp
    mov %rsp, %rbp
    subq $240, %rsp
log_message_0:
    movl %edi, -184(%rbp)
    movq %rsi, -192(%rbp)
    movq %rdx, -160(%rbp)
    movq %rcx, -152(%rbp)
    movq %r8, -144(%rbp)
    movq %r9, -136(%rbp)
    movb %al, %al
    cmpb $0, %al
    sete %al
    movzbl %al, %eax
    movl %eax, -224(%rbp)
    movl -224(%rbp), %eax
    testl %eax, %eax
    jnz log_message_1
    movsd %xmm0, -128(%rbp)
    movsd %xmm1, -112(%rbp)
    movsd %xmm2, -96(%rbp)
    movsd %xmm3, -80(%rbp)
    movsd %xmm4, -64(%rbp)
    movsd %xmm5, -48(%rbp)
    movsd %xmm6, %xmm0
    movsd %xmm0, -32(%rbp)
    movsd %xmm7, %xmm0
    movsd %xmm0, -16(%rbp)
log_message_1:
    leaq -184(%rbp), %rax
    movq %rax, -224(%rbp)
    movq -224(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -224(%rbp)
    leaq logger(%rip), %rax
    movq %rax, -232(%rbp)
    movq -232(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -232(%rbp)
    movl -224(%rbp), %eax
    cmpl -232(%rbp), %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -224(%rbp)
    movl -224(%rbp), %eax
    testl %eax, %eax
    jz log_message_3
log_message_2:
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp log_message_3
log_message_3:
    leaq -184(%rbp), %rax
    movq %rax, -224(%rbp)
    movq -224(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -224(%rbp)
    subq $8, %rsp
    movl -224(%rbp), %edi
    call log_start
    addq $8, %rsp
    leaq -216(%rbp), %rax
    movq %rax, -224(%rbp)
    movq -224(%rbp), %rax
    movl $16, %ecx
    movl %ecx, (%rax)
    movq -224(%rbp), %rax
    addq $4, %rax
    movq %rax, -224(%rbp)
    movq -224(%rbp), %rax
    movl $48, %ecx
    movl %ecx, (%rax)
    movq -224(%rbp), %rax
    addq $4, %rax
    movq %rax, -224(%rbp)
    leaq 16(%rbp), %rax
    movq %rax, -232(%rbp)
    movq -224(%rbp), %rax
    movq -232(%rbp), %rcx
    movq %rcx, (%rax)
    movq -224(%rbp), %rax
    addq $8, %rax
    movq %rax, -224(%rbp)
    leaq -176(%rbp), %rax
    movq %rax, -232(%rbp)
    movq -224(%rbp), %rax
    movq -232(%rbp), %rcx
    movq %rcx, (%rax)
    leaq logger(%rip), %rax
    movq %rax, -224(%rbp)
    movq -224(%rbp), %rax
    addq $8, %rax
    movq %rax, -224(%rbp)
    movq -224(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -224(%rbp)
    leaq -192(%rbp), %rax
    movq %rax, -232(%rbp)
    movq -232(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -232(%rbp)
    leaq -216(%rbp), %rax
    movq %rax, -240(%rbp)
    movq -224(%rbp), %rdi
    movq -232(%rbp), %rsi
    movq -240(%rbp), %rdx
    call vfprintf
    movl %eax, -224(%rbp)
    leaq logger(%rip), %rax
    movq %rax, -224(%rbp)
    movq -224(%rbp), %rax
    addq $8, %rax
    movq %rax, -224(%rbp)
    movq -224(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -224(%rbp)
    movq -224(%rbp), %rdi
    call fflush
    movl %eax, -224(%rbp)
    leaq -184(%rbp), %rax
    movq %rax, -224(%rbp)
    movq -224(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -224(%rbp)
    movl -224(%rbp), %eax
    cmpl $4, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -224(%rbp)
    movl -224(%rbp), %eax
    testl %eax, %eax
    jz log_message_5
log_message_4:
    movl $1, %edi
    call exit
    jmp log_message_5
log_message_5:
    mov %rbp, %rsp
    pop %rbp
    ret
align:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
align_0:
    movl %edi, -8(%rbp)
    movl %esi, -16(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -24(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -32(%rbp)
    movl -24(%rbp), %eax
    addl -32(%rbp), %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    subl $1, %eax
    movl %eax, -24(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    subl $1, %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    notl %eax
    movl %eax, -32(%rbp)
    movl -24(%rbp), %eax
    andl -32(%rbp), %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
get_token:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
get_token_0:
    movq %rdi, -8(%rbp)
    movl %esi, -16(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq %rax, -24(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -32(%rbp)
    movq -24(%rbp), %rdi
    movl -32(%rbp), %esi
    call get
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    mov %rbp, %rsp
    pop %rbp
    ret
get_node:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
get_node_0:
    movq %rdi, -8(%rbp)
    movl %esi, -16(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -24(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -32(%rbp)
    movq -24(%rbp), %rdi
    movl -32(%rbp), %esi
    call get
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    mov %rbp, %rsp
    pop %rbp
    ret
set_node:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
set_node_0:
    movq %rdi, -8(%rbp)
    movq %rsi, -16(%rbp)
    movl %edx, -24(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    addq $16, %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movq %rax, -32(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    addq $8, %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -48(%rbp)
    movl -40(%rbp), %eax
    imull -48(%rbp)
    movl %eax, -40(%rbp)
    movslq -40(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    imulq $1, %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    addq -40(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movq %rax, -32(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rdi
    movq -40(%rbp), %rsi
    movq $8, %rdx
    call memcpy
    movq %rax, -32(%rbp)
    mov %rbp, %rsp
    pop %rbp
    ret
insert_node:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
insert_node_0:
    movq %rdi, -8(%rbp)
    movq %rsi, -16(%rbp)
    movl %edx, -24(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -32(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq %rax, -40(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -48(%rbp)
    movq -32(%rbp), %rdi
    movq -40(%rbp), %rsi
    movl -48(%rbp), %edx
    call insert
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    mov %rbp, %rsp
    pop %rbp
    ret
get_symbol_table:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
get_symbol_table_0:
    movq %rdi, -8(%rbp)
    movl %esi, -16(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    addq $32, %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq %rax, -24(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -32(%rbp)
    movq -24(%rbp), %rdi
    movl -32(%rbp), %esi
    call get
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    mov %rbp, %rsp
    pop %rbp
    ret
get_current_symbol_table:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
get_current_symbol_table_0:
    movq %rdi, -8(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movq %rax, -16(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    addq $32, %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    subl $1, %eax
    movl %eax, -24(%rbp)
    movq -16(%rbp), %rdi
    movl -24(%rbp), %esi
    call get_symbol_table
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    mov %rbp, %rsp
    pop %rbp
    ret
get_symbol:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
get_symbol_0:
    movq %rdi, -8(%rbp)
    movl %esi, -16(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq %rax, -24(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -32(%rbp)
    movq -24(%rbp), %rdi
    movl -32(%rbp), %esi
    call get
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    mov %rbp, %rsp
    pop %rbp
    ret
get_enum_field:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
get_enum_field_0:
    movq %rdi, -8(%rbp)
    movl %esi, -16(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    addq $16, %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq %rax, -24(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -32(%rbp)
    movq -24(%rbp), %rdi
    movl -32(%rbp), %esi
    call get
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    mov %rbp, %rsp
    pop %rbp
    ret
get_struct_member:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
get_struct_member_0:
    movq %rdi, -8(%rbp)
    movl %esi, -16(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    addq $16, %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq %rax, -24(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -32(%rbp)
    movq -24(%rbp), %rdi
    movl -32(%rbp), %esi
    call get
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    mov %rbp, %rsp
    pop %rbp
    ret
get_union_member:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
get_union_member_0:
    movq %rdi, -8(%rbp)
    movl %esi, -16(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    addq $16, %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq %rax, -24(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -32(%rbp)
    movq -24(%rbp), %rdi
    movl -32(%rbp), %esi
    call get
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    mov %rbp, %rsp
    pop %rbp
    ret
.global main
main:
    push %rbp
    mov %rsp, %rbp
    subq $704, %rsp
main_0:
    movl %edi, -8(%rbp)
    movq %rsi, -16(%rbp)
    leaq .LC15(%rip), %rax
    movq %rax, -680(%rbp)
    subq $176, %rsp
    movq -680(%rbp), %rdi
    xor %eax, %eax
    call printf
    addq $176, %rsp
    movl %eax, -680(%rbp)
    leaq stdout(%rip), %rax
    movq %rax, -680(%rbp)
    movq -680(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -680(%rbp)
    subq $8, %rsp
    movq -680(%rbp), %rdi
    movl $0, %esi
    call init_logger
    addq $8, %rsp
    leaq -344(%rbp), %rax
    movq %rax, -680(%rbp)
    leaq -672(%rbp), %rax
    movq %rax, -688(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -696(%rbp)
    movq -696(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -696(%rbp)
    movl -696(%rbp), %eax
    movl %eax, -696(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -704(%rbp)
    movq -704(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -704(%rbp)
    movq -688(%rbp), %rdi
    movl -696(%rbp), %esi
    movq -704(%rbp), %rdx
    call begin_compiler
    movq -688(%rbp), %rsi
    movq -680(%rbp), %rdi
    mov $328, %rdx
    call memcpy
    leaq -344(%rbp), %rax
    movq %rax, -680(%rbp)
    movq -680(%rbp), %rdi
    call drive
    leaq -344(%rbp), %rax
    movq %rax, -680(%rbp)
    movq -680(%rbp), %rdi
    call free_compiler
    movl $0, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
