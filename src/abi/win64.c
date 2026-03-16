#include "compiler_c/abi/abi.h"
#include "compiler_c/analyse/analysis.h"
#include "compiler_c/core/type.h"
#include "compiler_c/ir/ir_module.h"
#include "compiler_c/log/logger.h"
#include "compiler_c/x86/x86.h"

const GP_Reg caller_saved_regs[CALLER_SAVED_REGISTERS] = {RAX, RCX, RDX, R8, R9, R10, R11};
const GP_Reg callee_saved_regs[CALLEE_SAVED_REGISTERS] = {RBX, RBP, RDI, RSI, R12, R13, R14, R15};
const GP_Reg int_param_regs[PARAM_REGISTERS] = {RCX, RDX, R8, R9};
const XMM_Reg float_param_regs[PARAM_REGISTERS] = {XMM0, XMM1, XMM2, XMM3};

ABI_Result classify(Type *type) { return (ABI_Result){.class = {}, .memory = type->size > HIDDEN_PTR_SIZE}; }

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
                case IR_FUNCTION:
                    break;
                case IR_UNDEFINED:
                    if (f->return_type == type_void && instr->op == IR_RET) break;
                    PANIC("An undefined IR value made it to analysis!!\n");
                }
            }
        }
    }
}

void abi_lower_store(IR_Function *f, IR_Block *b, IR_Instruction *instr, int *j) {
    Type *s_t = instr->store.type;
    if (s_t->size > MAX_STRUCT_SIZE) {
        // Hidden pointer
        IR_Value v = {.kind = IR_VREG, .size = 8, .align = 8, .reg = f->next_reg++};
        f->max_reg++;
        IR_Instruction addr = {.op = IR_ADDR, .op_count = 2, .ops = {[0] = v, [1] = instr->ops[0]}};
        insert(&b->instruction_array, &addr, (*j)++);
        instr = get_instruction(&b->instruction_array, *j);

        IR_Instruction memcpy = {
            .op = IR_MEMCPY, .op_count = 2, .ops = {[0] = v, [1] = instr->ops[1]}, .memcpy = {.size = instr->store.type->size}};
        memcpy.ops[1].size = 8;
        set(&b->instruction_array, &memcpy, *j);
    } else {
        IR_Instruction store = *instr;
        store.store.type = get_integer_type(s_t->size);
        store.ops[0].size = store.store.type->size;
        store.ops[1].size = store.store.type->size;
        set(&b->instruction_array, &store, *j);
    }
}

void abi_lower_call(IR_Function *f, IR_Block *b, IR_Instruction *instr, int *i) {
    // Convert to int chunks or pointer
    for (int k = 0; k < instr->call.arg_array.count; k++) {
        IR_Var *arg = get_arg(instr, k);
        if (arg->type->kind == T_STRUCT) {
            Type *s_t = arg->type;
            if (s_t->size > MAX_STRUCT_SIZE) {
                f->max_reg++;
                IR_Value v = {.kind = IR_VREG, .size = 8, .align = 8, .reg = f->next_reg++};
                IR_Instruction addr_instr = {.op = IR_ADDR, .op_count = 2, .ops = {[0] = v, [1] = arg->reg}};
                arg->type = get_pointer_type(arg->type);
                arg->name = "_tmp_s_ptr";
                arg->reg = addr_instr.ops[0];
                insert(&b->instruction_array, &addr_instr, (*i)++);
            } else {
                arg->type = get_integer_type(s_t->size);
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
                abi_lower_call(f, b, instr, &j);
            } else if (instr->op == IR_STORE && instr->store.type->kind == T_STRUCT) {
                abi_lower_store(f, b, instr, &j);
            }
        }
    }
}

void abi_emit_call(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    const int dst_offset = instr->ops[0].stack_offset;
    Type *t = instr->call.type->_func.return_type;

    int gp_index = 0;

    const int spilled_count = instr->call.arg_array.count > PARAM_REGISTERS ? instr->call.arg_array.count - PARAM_REGISTERS : 0;
    // +8 for push rbp (call emits push rbp, mov rsp, rbp)
    // 8 * spilled count, for n args after [0-3]
    // SHADOW_SPACE = 32, for windows ABI (linux = 0)
    // Prop shouldnt use/need align here,
    const int param_frame_size = align(SHADOW_SPACE + 8 * spilled_count + 8, 16);
    int param_offset = SHADOW_SPACE;
    if (param_frame_size > 0) fprintf(fp, "    subq $%d, %%rsp\n", param_frame_size);
    for (int i = 0; i < instr->call.arg_array.count; i++) {
        const IR_Var *v = get_arg(instr, i);
        bool use_register = false;
        use_register = gp_index < PARAM_REGISTERS;
        switch (v->type->kind) {
        case T_INT:
        case T_POINTER:
            if (use_register) {
                fprintf(fp, "    mov%s %d(%%rbp), %s\n", x86_op_suffix(v->type), v->reg.stack_offset,
                        gp_register_str[int_param_regs[gp_index++]][reg_size(v->type->size)]);
            } else {
                const char *v_reg = x86_rax_reg(v->type);
                fprintf(fp, "    mov%s %d(%%rbp), %s\n", x86_op_suffix(v->type), v->reg.stack_offset, v_reg);
                fprintf(fp, "    mov%s %s, %d(%%rsp)\n", x86_op_suffix(v->type), v_reg, param_offset);
                param_offset += 8;
            }
            break;
        case T_FLOAT:
            const char *f_suffix = x86_op_suffix(v->type);
            if (use_register) {
                if (instr->call.type->_func.is_variadic) {
                    fprintf(fp, "    mov%s %d(%%rbp), %s\n", x86_integer_op_suffix(v->type->size), v->reg.stack_offset,
                            gp_register_str[int_param_regs[gp_index]][reg_size(v->type->size)]);
                }
                fprintf(fp, "    mov%s %d(%%rbp), %s\n", f_suffix, v->reg.stack_offset, sse_register_str[float_param_regs[gp_index++]]);
            } else {
                fprintf(fp, "    mov%s %d(%%rbp), %%xmm0\n", f_suffix, v->reg.stack_offset);
                fprintf(fp, "    mov%s %%xmm0, %d(%%rsp)\n", f_suffix, param_offset);
                param_offset += 8;
            }
            break;
        default:
            log_start(LOG_ERROR);
            printf("Tried to emit call arg for unsupported type ");
            print_type(v->type);
            printf("\n");
            exit(1);
        }
    }

    if (instr->ops[1].kind == IR_FUNCTION) {
        fprintf(fp, "    call %s\n", instr->ops[1].func.name);
    } else {
        x86_emit_xr(fp, "mov", "q", "", &instr->ops[1], "%rax");
        fprintf(fp, "    call *%%rax\n");
    }
    fprintf(fp, "    addq $%d, %%rsp\n", param_frame_size);

    if (t == type_void) return;

    fprintf(fp, "    mov%s %s, %d(%%rbp)\n", x86_op_suffix(t), x86_rax_reg(t), dst_offset);
}

void abi_gen_memcpy_instruction(FILE *fp, const IR_Instruction *instr) {
    // TODO: Correctly determine correct lowering for IR_STACK, LITERAL, GLOBAL etc
    switch (instr->ops[1].kind) {
    case IR_LITERAL:
    case IR_GLOBAL:
        x86_emit_xr(fp, "lea", "", "", &instr->ops[1], "%rdx");
        break;
    case IR_STACK:
    case IR_PHYS_REG:
        x86_emit_xr(fp, "mov", "q", "", &instr->ops[1], "%rdx");
        break;
    case IR_VREG:
    case IR_MEM:
    case IR_FUNCTION:
    case IR_UNDEFINED:
        PANIC("Sanity check failed\n");
    }

    switch (instr->ops[0].kind) {
    case IR_LITERAL:
    case IR_GLOBAL:
        x86_emit_xr(fp, "lea", "", "", &instr->ops[0], "%rcx");
        break;
    case IR_STACK:
    case IR_PHYS_REG:
        x86_emit_xr(fp, "mov", "q", "", &instr->ops[0], "%rcx");
        break;
    case IR_VREG:
    case IR_MEM:
    case IR_FUNCTION:
    case IR_UNDEFINED:
        PANIC("Sanity check failed\n");
    }
    fprintf(fp, "    mov $%d, %%r8\n", instr->memcpy.size);
    fprintf(fp, "    sub $40, %%rsp\n");
    fprintf(fp, "    call memcpy\n");
    fprintf(fp, "    add $40, %%rsp\n");
}
