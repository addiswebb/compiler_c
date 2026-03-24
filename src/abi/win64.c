#include "compiler_c/abi/abi.h"
#include "compiler_c/analyse/analysis.h"
#include "compiler_c/analyse/analysis_types.h"
#include "compiler_c/core/type.h"
#include "compiler_c/ir/ir_module.h"
#include "compiler_c/log/logger.h"
#include "compiler_c/x86/x86.h"

const GP_Reg caller_saved_regs[CALLER_SAVED_REGISTERS] = {RAX, RCX, RDX, R8, R9, R10, R11};
const GP_Reg callee_saved_regs[CALLEE_SAVED_REGISTERS] = {RBX, RBP, RDI, RSI, R12, R13, R14, R15};
const GP_Reg int_param_regs[PARAM_REGISTERS] = {RCX, RDX, R8, R9};
const XMM_Reg float_param_regs[PARAM_REGISTERS] = {XMM0, XMM1, XMM2, XMM3};

ABI_Result abi_classify(Type *type) { return (ABI_Result){.class = {}, .memory = type->size > HIDDEN_PTR_SIZE}; }

IR_Value abi_lower_param_register(Type *type, int i) {
    ASSERT(i >= 0 && i < PARAM_REGISTERS, "Win64 ABI Invalid param arg index %d\n", i);
    IR_Value v = (IR_Value){.kind = IR_PHYS_REG,
                            .phys_reg = (PhysReg){.data_kind = REG_DATA_NONE, .size = reg_size(type->size), .offset = 0, .scale = 0}};
    v.kind = IR_PHYS_REG;
    if (type->kind == T_FLOAT) {
        v.phys_reg.kind = REG_XMM;
        v.phys_reg.sse_reg = float_param_regs[i];
    } else {
        v.phys_reg.kind = REG_GP;
        v.phys_reg.gp_reg = int_param_regs[i];
    }
    return v;
}
void lower_ir_values_to_stack(const IR_Function *f, const Lifetime *lts, const int lts_count, const Array *symbol_slots,
                              const Array *symbol_map) {
    for (int i = 0; i < f->blocks_array.count; i++) {
        const IR_Block *b = get_block(f, i);
        for (int j = 0; j < b->instruction_array.count; j++) {
            IR_Instruction *instr = get_instruction(&b->instruction_array, j);
            const int value_count = instr->op == IR_CALL ? instr->op_count + instr->call.arg_array.count : instr->op_count;
            for (int k = 0; k < value_count; k++) {
                bool is_arg_param = k >= instr->op_count;
                int instr_index = is_arg_param ? k - instr->op_count : k;
                IR_CallArg *arg = is_arg_param ? get_call_arg(instr, instr_index) : NULL;
                IR_Value *a = is_arg_param ? &arg->v : &instr->ops[instr_index];

                // if (is_arg_param && instr_index < PARAM_REGISTERS) {
                //     // TODO implement ir_phys_reg(a, function_registers[k-instr->op_count]);
                //     WARN("ABI dependent code undergoing rewrite\n");
                //     *a = abi_lower_param_register(arg->type, instr_index);
                //     continue;
                // }
                // Lower IR_VREG & IR_SYMBOL to IR_PHYS_REG
                switch (a->kind) {
                case IR_VREG:
                    ir_lower_vreg_value(a, lts, lts_count);
                    break;
                case IR_SYMBOL:
                    ir_lower_symbol_value(a, symbol_slots, symbol_map);
                    break;
                case IR_CONSTANT:
                    ir_lower_const_value(a);
                    break;
                case IR_PHYS_REG:
                case IR_INT_LITERAL:
                    break;
                case IR_UNDEFINED:
                    if (instr->op == IR_RET && instr->ret.type == type_void) break;
                    if (instr->op == IR_CALL && instr->call.type->_func.return_type == type_void) break;
                    PANIC("An undefined IR value made it to analysis!!\n");
                    break;
                }
            }
        }
    }
}
void abi_lower_store(IR_Function *f, IR_Block *b, IR_Instruction *instr, int *i) {
    Type *s_t = instr->store.type;
    if (s_t->size > MAX_STRUCT_SIZE) {
        WARN("ABI lower store\n");
        // Hidden pointer
        IR_Value v = {.kind = IR_VREG, .size = 8, .align = 8, .vreg = f->next_reg++};
        f->max_reg++;
        IR_Instruction addr = {.op = IR_ADDR, .op_count = 2, .ops = {[0] = v, [1] = instr->ops[0]}};
        instr = get_instruction(&b->instruction_array, *i);

        IR_Instruction memcpy = {
            .op = IR_MEMCPY, .op_count = 2, .ops = {[0] = v, [1] = instr->ops[1]}, .memcpy = {.size = instr->store.type->size}};
        memcpy.ops[1].size = 8;
        insert(&b->instruction_array, &addr, (*i)++);
        set(&b->instruction_array, &memcpy, *i);
    } else {
        IR_Instruction store = *instr;
        store.store.type = get_integer_type(s_t->size);
        store.ops[0].size = store.store.type->size;
        store.ops[1].size = store.store.type->size;
        set(&b->instruction_array, &store, *i);
    }
}

void abi_lower_call(IR_Function *f, IR_Block *b, IR_Instruction *instr, int *i) {
    WARN("ABI lower ret\n");
    Type *s_t = instr->call.type->_func.return_type;
    if (s_t->kind == T_STRUCT) {
        //     IR_Value s_v = {.kind = IR_MEM, .size = s_t->size, .align = s_t->align, .mem = f->locals_array.count, .offset = 0};
        instr->call.type = abi_func_type(instr->call.type);
        //     append(&f->locals_array, &(IR_Var){"_s", s_v, s_t});
        //     if (s_t->size > MAX_STRUCT_SIZE) {
        //         IR_Instruction alloca = {.op = IR_ALLOCA, .op_count = 1, .ops = {[0] = s_v}, .alloca = {.size = s_t->size}};
        //         IR_Instruction local_addr = {.op = IR_ADDR, .op_count = 2, .ops = {[0] = instr->ops[0], [1] = s_v}};
        //         insert(&instr->call.arg_array, &(IR_Var){.name = "_sret", .reg = instr->ops[0], .type = get_pointer_type(s_t)}, 0);
        //         instr->ops[0] = ir_no_value;
        //         insert(&b->instruction_array, &alloca, (*i)++);
        //         insert(&b->instruction_array, &local_addr, (*i)++);
        //     } else {
        //         IR_Instruction addr = {.op = IR_ADDR, .op_count = 2, .ops = {[0] = instr->ops[0], [1] = s_v}};
        //         instr->ops[0] = s_v;
        //         IR_Instruction alloca = {.op = IR_ALLOCA, .op_count = 1, .ops = {[0] = s_v}, .alloca = {.size = s_t->size}};
        //         insert(&b->instruction_array, &alloca, (*i)++);
        //         insert(&b->instruction_array, &addr, ++(*i));
        //     }
    }
    // Convert to int chunks or pointer
    // for (int k = 0; k < instr->call.arg_array.count; k++) {
    //     IR_Var *arg = get_arg(instr, k);
    //     if (arg->type->kind == T_STRUCT) {
    //         Type *s_t = arg->type;
    //         if (s_t->kind == T_VOID) continue;
    //         if (s_t->size > MAX_STRUCT_SIZE) {
    //             IR_Value v = {.kind = IR_VREG, .size = 8, .align = 8, .reg = f->next_reg++};
    //             f->max_reg++;
    //             IR_Instruction addr_instr = {.op = IR_ADDR, .op_count = 2, .ops = {[0] = v, [1] = arg->reg}};
    //             arg->type = get_pointer_type(arg->type);
    //             arg->name = "_tmp_s_ptr";
    //             arg->reg = addr_instr.ops[0];
    //             insert(&b->instruction_array, &addr_instr, (*i)++);
    //         } else arg->type = get_integer_type(s_t->size);
    //     }
    // }
}
void abi_lower_param(IR_Function *f, IR_Block *b, IR_Instruction *instr, int *i) {
    Type *type = instr->param.type;
    if (type->size > MAX_STRUCT_SIZE) type = type_u64;
    instr->op_count = 2;
    if (instr->param.param_index < PARAM_REGISTERS) instr->ops[1] = abi_lower_param_register(type, instr->param.param_index);
    else {
        instr->ops[1] = ir_stack_value(8, 8, SHADOW_SPACE + 8 + 16 + 8 * (instr->param.param_index - 4));
    }
    if (instr->param.type->size > MAX_STRUCT_SIZE) {
        IR_Value hidden_ptr = (IR_Value){.kind = IR_VREG, .size = 8, .align = 8, .vreg = f->next_reg++};
        f->max_reg++;
        IR_Value s_addr = (IR_Value){.kind = IR_VREG, .size = 8, .align = 8, .vreg = f->next_reg++};
        f->max_reg++;
        IR_Instruction param_instr = {.op = IR_PARAM,
                                      .op_count = 2,
                                      .ops = {[0] = hidden_ptr, [1] = instr->ops[1]},
                                      .param = {.param_index = -1, .type = get_pointer_type(type)}};
        IR_Instruction addr = {.op = IR_ADDR, .op_count = 2, .ops = {[0] = s_addr, [1] = instr->ops[0]}};
        IR_Instruction memcpy = {
            .op = IR_MEMCPY, .op_count = 2, .ops = {[0] = s_addr, [1] = hidden_ptr}, .memcpy = {.size = instr->param.type->size}};

        set(&b->instruction_array, &param_instr, (*i));
        insert(&b->instruction_array, &addr, (*i + 1));
        (*i)++;
        insert(&b->instruction_array, &memcpy, (*i + 1));
        (*i)++;
    } else instr->param.type = get_integer_type(instr->param.type->size);
}
void abi_lower_ret(IR_Function *f, IR_Block *b, IR_Instruction *instr, int *i) {
    WARN("ABI lower ret\n");
    // Type *s_t = instr->ret.type;
    // if (s_t->size > MAX_STRUCT_SIZE) {
    //     instr->ret.type = get_pointer_type(s_t);
    //     IR_Value local_v = {.kind = IR_VREG, .size = 8, .align = 8, .vreg = f->next_reg++};
    //     f->max_reg++;
    //     IR_Instruction local_addr = {.op = IR_ADDR, .op_count = 2, .ops = {[0] = local_v, [1] = instr->ops[0]}};
    //     IR_Instruction memcpy = {
    //         .op = IR_MEMCPY, .op_count = 2, .ops = {[0] = ir_mem_value(0, instr->ret.type), [1] = local_v}, .memcpy = {.size =
    //         s_t->size}};
    //     instr->ops[0] = ir_no_value;
    //     instr->ret.type = type_void;
    //     insert(&b->instruction_array, &local_addr, (*i)++);
    //     insert(&b->instruction_array, &memcpy, (*i)++);
    // } else instr->ret.type = get_integer_type(s_t->size);
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
            } else if (instr->op == IR_RET) {
                abi_lower_ret(f, b, instr, &j);
            } else if (instr->op == IR_PARAM) {
                abi_lower_param(f, b, instr, &j);
            } else if (instr->op == IR_LOAD) {
                if (instr->load.type->kind == T_STRUCT) instr->load.type = get_integer_type(instr->load.type->size);
            }
        }
    }
}

void abi_emit_call(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    Type *t = instr->call.type->_func.return_type;
    if (t->kind == T_STRUCT) t = get_integer_type(t->size);

    int gp_index = 0;

    const int spilled_count = instr->call.arg_array.count > PARAM_REGISTERS ? instr->call.arg_array.count - PARAM_REGISTERS : 0;
    // +8 for push rbp (call emits push rbp, mov rsp, rbp)
    // 8 * spilled count, for n args after [0-3]
    // SHADOW_SPACE = 32, for windows ABI (linux = 0)
    const int param_frame_size = SHADOW_SPACE + 8 * spilled_count + 8;
    // Place first stack param after rbp and Shadow space
    int param_offset = 8 + SHADOW_SPACE;
    if (param_frame_size > 0) fprintf(fp, "    subq $%d, %%rsp\n", param_frame_size);
    for (int i = 0; i < instr->call.arg_array.count; i++) {
        IR_CallArg *v = get_call_arg(instr, i);
        Type *arg_type = v->type;
        if (arg_type->kind == T_STRUCT) {
            if (arg_type->size > MAX_STRUCT_SIZE) get_pointer_type(arg_type);
            else arg_type = get_integer_type(arg_type->size);
        }
        bool use_register = false;
        use_register = gp_index < PARAM_REGISTERS;
        switch (arg_type->kind) {
        case T_INT:
        case T_POINTER:
            if (use_register) {
                x86_emit_xr(fp, "mov", x86_op_suffix(arg_type), "", &v->v,
                            gp_register_str[int_param_regs[gp_index++]][reg_size(arg_type->size)]);
            } else {
                const char *v_reg = x86_rax_reg(arg_type);
                x86_emit_xr(fp, "mov", x86_op_suffix(arg_type), "", &v->v, v_reg);
                fprintf(fp, "    mov%s %s, %d(%%rsp)\n", x86_op_suffix(arg_type), v_reg, param_offset);
                param_offset += 8;
            }
            break;
        case T_FLOAT:
            const char *f_suffix = x86_op_suffix(arg_type);
            if (use_register) {
                if (instr->call.type->_func.is_variadic) {
                    x86_emit_xr(fp, "mov", x86_integer_op_suffix(arg_type->size), "", &v->v,
                                gp_register_str[int_param_regs[gp_index]][reg_size(arg_type->size)]);
                }
                x86_emit_xr(fp, "mov", f_suffix, "", &v->v, sse_register_str[float_param_regs[gp_index++]]);
            } else {
                x86_emit_xr(fp, "mov", f_suffix, "", &v->v, sse_register_str[XMM0]);
                fprintf(fp, "    mov%s %%xmm0, %d(%%rsp)\n", f_suffix, param_offset);
                param_offset += 8;
            }
            break;
        default:
            log_start(LOG_ERROR);
            printf("Tried to emit call arg for unsupported type ");
            print_type(arg_type);
            printf("\n");
            exit(1);
        }
    }

    if (instr->ops[1].kind == IR_PHYS_REG && instr->ops[1].phys_reg.data_kind == REG_DATA_LABEL) {
        fprintf(fp, "    call %s\n", instr->ops[1].phys_reg.label);
    } else {
        x86_emit_xr(fp, "mov", "q", "", &instr->ops[1], "%rax");
        fprintf(fp, "    call *%%rax\n");
    }
    fprintf(fp, "    addq $%d, %%rsp\n", param_frame_size);

    if (t == type_void) return;

    x86_emit_rx(fp, "mov", x86_op_suffix(t), "", x86_rax_reg(t), &instr->ops[0]);
}

Type *abi_func_type(Type *type) {
    ASSERT(type->kind == T_FUNCTION, "Invalid Func Type\n");
    bool changed = false;
    Type *abi_type = new_type();
    memcpy(abi_type, type, sizeof(Type));
    array_init(&abi_type->_func.params, type->_func.params.capacity, type->_func.params.element_size);
    memcpy(abi_type->_func.params.data, type->_func.params.data, type->_func.params.count * type->_func.params.element_size);
    abi_type->_func.params.count = type->_func.params.count;
    if (type->_func.return_type->kind == T_STRUCT) {
        if (abi_type->_func.return_type->size > MAX_STRUCT_SIZE) {
            insert(&abi_type->_func.params, &(ParamDecl){.type = get_pointer_type(abi_type->_func.return_type), .name = "_sret"}, 0);
            abi_type->_func.return_type = type_void;
        } else abi_type->_func.return_type = get_integer_type(abi_type->_func.return_type->size);
        changed = true;
    }
    for (int i = 0; i < abi_type->_func.params.count; i++) {
        ParamDecl *d = get(&abi_type->_func.params, i);
        if (d->type->size > MAX_STRUCT_SIZE) {
            d->type = get_pointer_type(d->type);
            changed = true;
        }
    }
    return changed ? abi_type : type;
}

void abi_gen_memcpy_instruction(FILE *fp, const IR_Instruction *instr) {
    // TODO: Correctly determine correct lowering for IR_STACK, LITERAL, GLOBAL etc
    switch (instr->ops[1].kind) {
    case IR_CONSTANT:
        x86_emit_xr(fp, "lea", "", "", &instr->ops[1], "%rdx");
        break;
    case IR_PHYS_REG:
        x86_emit_xr(fp, "mov", "q", "", &instr->ops[1], "%rdx");
        break;
    case IR_SYMBOL:
    case IR_INT_LITERAL:
    case IR_VREG:
    case IR_UNDEFINED:
        PANIC("Sanity check failed\n");
        break;
    }

    switch (instr->ops[1].kind) {
    case IR_CONSTANT:
        x86_emit_xr(fp, "lea", "", "", &instr->ops[0], "%rcx");
        break;
    case IR_PHYS_REG:
        x86_emit_xr(fp, "mov", "q", "", &instr->ops[0], "%rcx");
        break;
    case IR_SYMBOL:
    case IR_INT_LITERAL:
    case IR_VREG:
    case IR_UNDEFINED:
        PANIC("Sanity check failed\n");
        break;
    }

    fprintf(fp, "    mov $%d, %%r8\n", instr->memcpy.size);
    fprintf(fp, "    sub $40, %%rsp\n");
    fprintf(fp, "    call memcpy\n");
    fprintf(fp, "    add $40, %%rsp\n");
}
