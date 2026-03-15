# SysV ABI

## Requirements
- [] Stack must be 16 byte aligned after `call`
- [x] `%al` Carries number of FP registers used in variadic function
- [x] 6 GP registers, 8 FP registers for args, then stack spilled
- [x] Stack args passed from right to left
- [x] Struct Register returns/128bit integer returns
- [] Floating point return values `xmm0` or `xmm0:xmm1`
- [] Structs as args,
