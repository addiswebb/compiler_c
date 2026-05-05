.section .note.GNU-stack,"",@progbits
.section .rodata
.align 8
.LC0:

.text
.global is_alpha
is_alpha:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
is_alpha_0:
    movb %dil, -8(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movb (%rax), %al
    movb %al, -16(%rbp)
    movsbl -16(%rbp), %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    cmpl $97, %eax
    setge %al
    movzbl %al, %eax
    movl %eax, -16(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movb (%rax), %al
    movb %al, -24(%rbp)
    movsbl -24(%rbp), %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    cmpl $122, %eax
    setle %al
    movzbl %al, %eax
    movl %eax, -24(%rbp)
    movl -16(%rbp), %eax
    andl -24(%rbp), %eax
    movl %eax, -16(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movb (%rax), %al
    movb %al, -24(%rbp)
    movsbl -24(%rbp), %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    cmpl $65, %eax
    setge %al
    movzbl %al, %eax
    movl %eax, -24(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movb (%rax), %al
    movb %al, -32(%rbp)
    movsbl -32(%rbp), %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    cmpl $90, %eax
    setle %al
    movzbl %al, %eax
    movl %eax, -32(%rbp)
    movl -24(%rbp), %eax
    andl -32(%rbp), %eax
    movl %eax, -24(%rbp)
    movl -16(%rbp), %eax
    orl -24(%rbp), %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
.global to_lower_case
to_lower_case:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
to_lower_case_0:
    movb %dil, -8(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movb (%rax), %al
    movb %al, -16(%rbp)
    movsbl -16(%rbp), %eax
    movl %eax, -16(%rbp)
    movl $1, %eax
    movl $5, %ecx
    shll %cl, %eax
    movl %eax, -24(%rbp)
    movl -16(%rbp), %eax
    orl -24(%rbp), %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    movb %al, -16(%rbp)
    movb -16(%rbp), %al
    mov %rbp, %rsp
    pop %rbp
    ret
.global is_num
is_num:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
is_num_0:
    movb %dil, -8(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movb (%rax), %al
    movb %al, -16(%rbp)
    movsbl -16(%rbp), %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    cmpl $48, %eax
    setge %al
    movzbl %al, %eax
    movl %eax, -16(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movb (%rax), %al
    movb %al, -24(%rbp)
    movsbl -24(%rbp), %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    cmpl $57, %eax
    setle %al
    movzbl %al, %eax
    movl %eax, -24(%rbp)
    movl -16(%rbp), %eax
    andl -24(%rbp), %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
.global is_alpha_num
is_alpha_num:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
is_alpha_num_0:
    movb %dil, -8(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movb (%rax), %al
    movb %al, -16(%rbp)
    movsbl -16(%rbp), %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    cmpl $95, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -16(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movb (%rax), %al
    movb %al, -24(%rbp)
    movb -24(%rbp), %dil
    call is_num
    movl %eax, -24(%rbp)
    movl -16(%rbp), %eax
    orl -24(%rbp), %eax
    movl %eax, -16(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movb (%rax), %al
    movb %al, -24(%rbp)
    movb -24(%rbp), %dil
    call is_alpha
    movl %eax, -24(%rbp)
    movl -16(%rbp), %eax
    orl -24(%rbp), %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
.global is_hex
is_hex:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
is_hex_0:
    movb %dil, -8(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movb (%rax), %al
    movb %al, -24(%rbp)
    movb -24(%rbp), %dil
    call is_num
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    testl %eax, %eax
    jz is_hex_2
is_hex_1:
    movl $1, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp is_hex_2
is_hex_2:
    leaq -16(%rbp), %rax
    movq %rax, -24(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movb (%rax), %al
    movb %al, -32(%rbp)
    movsbl -32(%rbp), %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    orl $32, %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    movb %al, -32(%rbp)
    movq -24(%rbp), %rax
    movb -32(%rbp), %cl
    movb %cl, (%rax)
    leaq -16(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movb (%rax), %al
    movb %al, -24(%rbp)
    movsbl -24(%rbp), %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    cmpl $102, %eax
    setle %al
    movzbl %al, %eax
    movl %eax, -24(%rbp)
    leaq -16(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax
    movb (%rax), %al
    movb %al, -32(%rbp)
    movsbl -32(%rbp), %eax
    movl %eax, -32(%rbp)
    movl -32(%rbp), %eax
    cmpl $97, %eax
    setge %al
    movzbl %al, %eax
    movl %eax, -32(%rbp)
    movl -24(%rbp), %eax
    andl -32(%rbp), %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
.global is_oct
is_oct:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
is_oct_0:
    movb %dil, -8(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movb (%rax), %al
    movb %al, -16(%rbp)
    movsbl -16(%rbp), %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    cmpl $48, %eax
    setge %al
    movzbl %al, %eax
    movl %eax, -16(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movb (%rax), %al
    movb %al, -24(%rbp)
    movsbl -24(%rbp), %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    cmpl $55, %eax
    setle %al
    movzbl %al, %eax
    movl %eax, -24(%rbp)
    movl -16(%rbp), %eax
    andl -24(%rbp), %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
.global is_binary
is_binary:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
is_binary_0:
    movb %dil, -8(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movb (%rax), %al
    movb %al, -16(%rbp)
    movsbl -16(%rbp), %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    cmpl $48, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -16(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movb (%rax), %al
    movb %al, -24(%rbp)
    movsbl -24(%rbp), %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    cmpl $49, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -24(%rbp)
    movl -16(%rbp), %eax
    orl -24(%rbp), %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
.global is_alpha_numeric_str
is_alpha_numeric_str:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
is_alpha_numeric_str_0:
    movq %rdi, -8(%rbp)
is_alpha_numeric_str_1:
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movb (%rax), %al
    movb %al, -16(%rbp)
    movsbl -16(%rbp), %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    testl %eax, %eax
    jz is_alpha_numeric_str_5
is_alpha_numeric_str_2:
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movb (%rax), %al
    movb %al, -16(%rbp)
    movb -16(%rbp), %al
    movb %al, -16(%rbp)
    movb -16(%rbp), %dil
    call is_alpha_num
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    testl %eax, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    testl %eax, %eax
    jz is_alpha_numeric_str_4
is_alpha_numeric_str_3:
    movl $0, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp is_alpha_numeric_str_4
is_alpha_numeric_str_4:
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -24(%rbp)
    movl $1, %eax
    movq %rax, -32(%rbp)
    movq -24(%rbp), %rax
    addq -32(%rbp), %rax
    movq %rax, -24(%rbp)
    movq -16(%rbp), %rax
    movq -24(%rbp), %rcx
    movq %rcx, (%rax)
    jmp is_alpha_numeric_str_1
is_alpha_numeric_str_5:
    movl $1, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
.global is_int_or_float
is_int_or_float:
    push %rbp
    mov %rsp, %rbp
    subq $48, %rsp
is_int_or_float_0:
    movq %rdi, -8(%rbp)
    movq %rsi, -16(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
    leaq -32(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl $0, %ecx
    movl %ecx, (%rax)
is_int_or_float_1:
    leaq -8(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -40(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -48(%rbp)
    movslq -48(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    imulq $1, %rax
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    addq -48(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movb (%rax), %al
    movb %al, -40(%rbp)
    movsbl -40(%rbp), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    cmpl $0, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz is_int_or_float_9
is_int_or_float_2:
    leaq -8(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -40(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -48(%rbp)
    movslq -48(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    imulq $1, %rax
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    addq -48(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movb (%rax), %al
    movb %al, -40(%rbp)
    movsbl -40(%rbp), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    cmpl $46, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz is_int_or_float_6
    leaq -32(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz is_int_or_float_6
is_int_or_float_3:
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jnz is_int_or_float_4
    leaq -8(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -40(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -48(%rbp)
    movl -48(%rbp), %eax
    addl $1, %eax
    movl %eax, -48(%rbp)
    movslq -48(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    imulq $1, %rax
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    addq -48(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movb (%rax), %al
    movb %al, -40(%rbp)
    movsbl -40(%rbp), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz is_int_or_float_5
is_int_or_float_4:
    movl $0, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp is_int_or_float_5
is_int_or_float_5:
    leaq -32(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl $1, %ecx
    movl %ecx, (%rax)
    jmp is_int_or_float_6
is_int_or_float_6:
    leaq -8(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -40(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -48(%rbp)
    movslq -48(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    imulq $1, %rax
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    addq -48(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movb (%rax), %al
    movb %al, -40(%rbp)
    movb -40(%rbp), %al
    movb %al, -40(%rbp)
    movb -40(%rbp), %dil
    call is_num
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jnz is_int_or_float_7
    leaq -8(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -40(%rbp)
    leaq -24(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -48(%rbp)
    movslq -48(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    imulq $1, %rax
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax
    addq -48(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movb (%rax), %al
    movb %al, -40(%rbp)
    movsbl -40(%rbp), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    cmpl $46, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz is_int_or_float_8
is_int_or_float_7:
    movl $0, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp is_int_or_float_8
is_int_or_float_8:
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -48(%rbp)
    movl -48(%rbp), %eax
    addl $1, %eax
    movl %eax, -48(%rbp)
    movq -40(%rbp), %rax
    movl -48(%rbp), %ecx
    movl %ecx, (%rax)
    jmp is_int_or_float_1
is_int_or_float_9:
    leaq -16(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    testl %eax, %eax
    jz is_int_or_float_11
is_int_or_float_10:
    leaq -16(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq (%rax), %rax
    movq %rax, -40(%rbp)
    leaq -32(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -48(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -48(%rbp)
    movl -48(%rbp), %eax
    testl %eax, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -48(%rbp)
    movq -40(%rbp), %rax
    movl -48(%rbp), %ecx
    movl %ecx, (%rax)
    jmp is_int_or_float_11
is_int_or_float_11:
    leaq -24(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movl (%rax), %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    cmpl $0, %eax
    setg %al
    movzbl %al, %eax
    movl %eax, -40(%rbp)
    movl -40(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
.global is_whitespace
is_whitespace:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
is_whitespace_0:
    movb %dil, -8(%rbp)
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax
    movb (%rax), %al
    movb %al, -16(%rbp)
    movsbl -16(%rbp), %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    cmpl $10, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    testl %eax, %eax
    jz is_whitespace_1
    movl -16(%rbp), %eax
    cmpl $9, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    testl %eax, %eax
    jz is_whitespace_2
    movl -16(%rbp), %eax
    cmpl $12, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    testl %eax, %eax
    jz is_whitespace_3
    movl -16(%rbp), %eax
    cmpl $13, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    testl %eax, %eax
    jz is_whitespace_4
    movl -16(%rbp), %eax
    cmpl $11, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -24(%rbp)
    movl -24(%rbp), %eax
    testl %eax, %eax
    jz is_whitespace_5
    movl -16(%rbp), %eax
    cmpl $32, %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    testl %eax, %eax
    jz is_whitespace_6
    jmp is_whitespace_7
is_whitespace_1:
is_whitespace_2:
is_whitespace_3:
is_whitespace_4:
is_whitespace_5:
is_whitespace_6:
    movl $1, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
is_whitespace_7:
    movl $0, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
is_whitespace_8:
