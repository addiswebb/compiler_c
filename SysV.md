# SysV ABI

## Requirements
- [x] Stack must be 16 byte aligned after `call`
- [x] `%al` Carries number of FP registers used in variadic function
- [x] 6 GP registers, 8 FP registers for args, then stack spilled
- [x] Stack args passed from right to left
- [x] Struct Register returns/128bit integer returns
- [x] Structs as args.
- [] <= 16b aggregate by double register `%rax:%rdx` / `%xmm0:%xmm1`
- [] Structs return values.
