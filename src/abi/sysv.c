#ifndef _WIN64
#include "compiler_c/abi/abi.h"
#include "compiler_c/analyse/analysis.h"
#include "compiler_c/analyse/analysis_types.h"
#include "compiler_c/core/type.h"
#include "compiler_c/log/logger.h"
#include "compiler_c/parse/parser.h"
#include "compiler_c/x86/x86.h"

const GP_Reg caller_saved_regs[CALLER_SAVED_REGISTERS] = {RAX, RCX, RDX, RSI, RDI, R8, R9, R10, R11};
const GP_Reg callee_saved_regs[CALLEE_SAVED_REGISTERS] = {RBX, RBP, R12, R13, R14, R15};
const GP_Reg int_param_regs[INTEGER_PARAM_REGISTERS] = {RDI, RSI, RDX, RCX, R8, R9};
const XMM_Reg float_param_regs[FLOAT_PARAM_REGISTERS] = {XMM0, XMM1, XMM2, XMM3, XMM4, XMM5, XMM6, XMM7};

ABI_TypeClass merge(ABI_TypeClass a, ABI_TypeClass b) {
    if (a == b) return a;
    if (a == ABI_NO_CLASS) return b;
    if (b == ABI_NO_CLASS) return a;
    if (a == ABI_INTEGER || b == ABI_INTEGER) return ABI_INTEGER;
    if (a == ABI_SSE && b == ABI_SSE) return ABI_SSE;
    PANIC("Invalid classification merge\n");
}
ABI_Result classify_struct(Type *type) {
    if (type->size > 16) return (ABI_Result){.memory = true};
    ABI_Result res = {.class = {ABI_NO_CLASS, ABI_NO_CLASS}, .memory = false};
    for (int i = 0; i < type->_struct.members_array.count; i++) {
        StructMember *m = get_struct_member(type, i);
        ABI_Result field_res = abi_classify(m->type);
        if (field_res.memory) return field_res;

        int start = m->offset;
        int end = m->offset + m->type->size - 1;

        int low = start / 8;
        int high = end / 8;
        if (field_res.memory) return field_res;
        for (int j = low; j <= high; j++) {
            res.class[j] = merge(res.class[j], field_res.class[j - low]);
        }
    }
    return res;
}

ABI_Result abi_classify(Type *type) {
    if (type->size > HIDDEN_PTR_SIZE) return (ABI_Result){.class = {}, .memory = true};
    switch (type->kind) {
    case T_INT:
    case T_POINTER:
        return (ABI_Result){.class = {ABI_INTEGER, ABI_NO_CLASS}, 0};
    case T_FLOAT:
        return (ABI_Result){.class = {ABI_SSE, ABI_NO_CLASS}, 0};
    case T_STRUCT:
        return classify_struct(type);
    default:
        PANIC("Classification failed\n");
    }
}
void lower_ir_values_to_stack(const IR_Function *f, const Lifetime *lts, const int lts_count, const StackSlot *mem_slots) {
    for (int i = 0; i < f->blocks_array.count; i++) {
        const IR_Block *b = get_block(f, i);
        for (int j = 0; j < b->instruction_array.count; j++) {
            IR_Instruction *instr = get_instruction(&b->instruction_array, j);
            const int value_count = instr->op == IR_CALL ? instr->op_count + instr->call.arg_array.count : instr->op_count;
            for (int k = 0; k < value_count; k++) {
                IR_Value *a = k < instr->op_count ? &instr->ops[k] : &get_arg(instr, k - instr->op_count)->reg;
                if (a->kind == IR_LITERAL) continue;
                // offload the following switch to a function which correctly lowers IR_VREG, IR_MEM to IR_STACK
                // leaving IR_GLOBAL, IR_LITERAL, IR_STACK
                switch (a->kind) {
                case IR_VREG:
                    // a->reg is negative for function parameters
                    if (a->reg < 0) {
                        if (-a->reg - 1 < PARAM_REGISTERS) {
                            physical_register(a);
                        } else {
                            param_offset(a);
                        }
                    } else stack_offset(a, lts, lts_count);
                    break;
                case IR_MEM:
                    a->stack_offset = -(mem_slots[a->mem].offset - a->offset);
                    a->kind = IR_STACK;
                    break;
                case IR_STACK:
                case IR_LITERAL:
                case IR_GLOBAL:
                case IR_PHYS_REG:
                    break;
                case IR_UNDEFINED:
                    if (f->return_type == type_void && instr->op == IR_RET) break;
                    PANIC("An undefined IR value made it to analysis!!\n");
                }
            }
        }
    }
}

void lower_ir_for_asm(IR_Function *f) {
    for (int i = 0; i < f->blocks_array.count; i++) {
        IR_Block *b = get_block(f, i);
        for (int j = 0; j < b->instruction_array.count; j++) {
            IR_Instruction *instr = get_instruction(&b->instruction_array, j);
            if (instr->op == IR_CALL) {
                // Convert to int chunks or pointer
                for (int k = 0; k < instr->call.arg_array.count; k++) {
                    IR_Var *arg = get_arg(instr, k);
                    if (arg->type->kind == T_STRUCT) {
                        Type *s_t = arg->type;
                        ABI_Result res = abi_classify(s_t);
                        if (res.memory) {
                            f->max_reg++;
                            IR_Value v = {.kind = IR_VREG, .size = 8, .align = 8, .reg = f->next_reg++};
                            IR_Instruction i = {.op = IR_ADDR, .op_count = 2, .ops = {[0] = v, [1] = arg->reg}};
                            arg->type = get_pointer_type(arg->type);
                            arg->name = "_tmp_s_ptr";
                            arg->reg = i.ops[0];
                            insert(&b->instruction_array, &i, j++);
                        } else {
                            arg->type = res.class[0] == ABI_INTEGER ? type_u64 : type_f64;
                            if (res.class[1] != ABI_NO_CLASS) {
                                IR_Var a = *arg;
                                a.reg.offset += 8;
                                a.type = res.class[1] == ABI_INTEGER ? type_u64 : type_f64;
                                append(&instr->call.arg_array, &a);
                            }
                        }
                    }
                }
            } else if (instr->op == IR_STORE && instr->store.type->kind == T_STRUCT) {
                // Lower to memcpy or reg reading,
                // Turns out this is for SysV calls not Win64 ABI ;_;
                Type *s_t = instr->store.type;
                ABI_Result res = abi_classify(s_t);
                if (res.memory) {
                    // Hidden pointer
                    IR_Value v = {.kind = IR_VREG, .size = 8, .align = 8, .reg = f->next_reg++};
                    f->max_reg++;
                    IR_Instruction addr = {.op = IR_ADDR, .op_count = 2, .ops = {[0] = v, [1] = instr->ops[0]}};
                    insert(&b->instruction_array, &addr, j++);
                    instr = get_instruction(&b->instruction_array, j);

                    IR_Instruction memcpy = {
                        .op = IR_MEMCPY, .op_count = 2, .ops = {[0] = v, [1] = instr->ops[1]}, .memcpy = {.size = instr->store.type->size}};
                    memcpy.ops[1].size = 8;
                    set(&b->instruction_array, &memcpy, j);
                } else {
                    IR_Instruction store = *instr;
                    store.ops[0].size = 8;
                    store.ops[1].size = 8;
                    store.store.type = res.class[0] == ABI_INTEGER ? type_u64 : type_f64;
                    set(&b->instruction_array, &store, j);
                    if (res.class[1] != ABI_NO_CLASS) {
                        store.ops[0].offset += 8;
                        store.ops[1].reg -= 1;
                        store.store.type = res.class[1] == ABI_INTEGER ? type_u64 : type_f64;
                        insert(&b->instruction_array, &store, ++j);
                    }
                }
            }
        }
    }
}

void abi_emit_call(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    const int dst_offset = instr->ops[0].stack_offset;
    Type *t = instr->call.type;

    int gp_index = 0;
    int sse_index = 0;

    for (int i = 0; i < instr->call.arg_array.count; i++) {
        const IR_Var *v = get_arg(instr, i);
        if ((v->type->kind == T_INT || v->type->kind == T_POINTER) && gp_index < INTEGER_PARAM_REGISTERS) gp_index++;
        if (v->type->kind == T_FLOAT && sse_index < FLOAT_PARAM_REGISTERS) sse_index++;
    }
    const int spilled_count = instr->call.arg_array.count - (sse_index + gp_index);
    sse_index = 0;
    gp_index = 0;

    const int param_frame_size = 8 * spilled_count;
    int param_offset = 0;
    if (param_frame_size > 0) fprintf(fp, "    subq $%d, %%rsp\n", param_frame_size);
    for (int i = 0; i < instr->call.arg_array.count; i++) {
        const IR_Var *v = get_arg(instr, i);
        switch (v->type->kind) {
        case T_INT:
        case T_POINTER:
            const char *suffix = x86_op_suffix(v->type);
            if (gp_index < INTEGER_PARAM_REGISTERS) {
                fprintf(fp, "    mov%s %d(%%rbp), %s\n", suffix, v->reg.stack_offset,
                        gp_register_str[int_param_regs[gp_index++]][reg_size(v->type->size)]);
            } else {
                const char *reg = x86_rax_reg(v->type);
                fprintf(fp, "    mov%s %d(%%rbp), %s\n", suffix, v->reg.stack_offset, reg);
                fprintf(fp, "    mov%s %s, %d(%%rsp)\n", suffix, reg, param_offset);
                param_offset += 8;
            }
            break;
        case T_FLOAT:
            const char *f_suffix = x86_op_suffix(v->type);
            if (sse_index < FLOAT_PARAM_REGISTERS) {
                fprintf(fp, "    mov%s %d(%%rbp), %s\n", f_suffix, v->reg.stack_offset, sse_register_str[float_param_regs[sse_index++]]);
            } else {
                fprintf(fp, "    mov%s %d(%%rbp), %%xmm0\n", f_suffix, v->reg.stack_offset);
                fprintf(fp, "    mov%s %%xmm0, %d(%%rsp)\n", f_suffix, param_offset);
                param_offset += 8;
            }
            break;
        default:
            PANIC("Tried to emit call arg for unsupported type\n");
        }
    }
    if (instr->call.callee->is_variadic) {
        if (sse_index) fprintf(fp, "    movl $%d, %%eax\n", sse_index);
        else fprintf(fp, "    xor %%eax, %%eax\n");
    }

    fprintf(fp, "    call %s\n", instr->call.callee->name);
    fprintf(fp, "    addq $%d, %%rsp\n", param_frame_size);

    if (t == type_void) return;

    fprintf(fp, "    mov%s %s, %d(%%rbp)\n", x86_op_suffix(t), x86_rax_reg(t), dst_offset);
}

void abi_gen_memcpy_instruction(FILE *fp, const IR_Instruction *instr) {
    // TODO: Correctly determine correct lowering for IR_STACK, LITERAL, GLOBAL etc
    switch (instr->ops[1].kind) {
    case IR_LITERAL:
    case IR_GLOBAL:
        x86_emit_xr(fp, "lea", "", "", &instr->ops[1], "%rsi");
        break;
    case IR_STACK:
    case IR_PHYS_REG:
        x86_emit_xr(fp, "mov", "q", "", &instr->ops[1], "%rsi");
        break;
    case IR_VREG:
    case IR_MEM:
    case IR_UNDEFINED:
        PANIC("Sanity check failed\n");
    }

    switch (instr->ops[0].kind) {
    case IR_LITERAL:
    case IR_GLOBAL:
        x86_emit_xr(fp, "lea", "", "", &instr->ops[0], "%rdi");
        break;
    case IR_STACK:
    case IR_PHYS_REG:
        x86_emit_xr(fp, "mov", "q", "", &instr->ops[0], "%rdi");
        break;
    case IR_VREG:
    case IR_MEM:
    case IR_UNDEFINED:
        PANIC("Sanity check failed\n");
    }
    fprintf(fp, "    mov $%d, %%rdx\n", instr->memcpy.size);
    fprintf(fp, "    call memcpy\n");
}
#endif
