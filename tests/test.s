.global main
    jmp main
test:
    push %rbp
    mov %rsp, %rbp
    subq $16, %rsp
test_0:
    movl 16(%rbp), %eax
    movl %eax, -8(%rbp)
;; store
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
;; ret
main:
    push %rbp
    mov %rsp, %rbp
    subq $32, %rsp
main_0:
    movl $5, %eax
    movl %eax, -16(%rbp)
;; const
    movl -16(%rbp), %eax
    push %rax
    call test
    add $8, %rsp
    movl %eax, -24(%rbp)
;; call
    movl -8(%rbp), %eax
    movl %eax, -8(%rbp)
;; store
    movl -8(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
;; ret
