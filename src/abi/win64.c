#include "compiler_c/ir/ir_module.h"
#ifdef _WIN64

#include "compiler_c/abi/abi.h"
#include "compiler_c/analyse/analysis.h"
#include "compiler_c/core/type.h"
#include "compiler_c/log/logger.h"
#include "compiler_c/x86/x86.h"

Symbol *_sret = NULL;
Symbol *_hidden_sret_ptr = NULL;

void set_hidden_sret_ptr(Type *return_type) {
    if (_hidden_sret_ptr && _hidden_sret_ptr->type->base == return_type) return;
    if (!_hidden_sret_ptr) {
        _hidden_sret_ptr = malloc(sizeof(Symbol));
        ASSERT(_hidden_sret_ptr, "Failed to allocate _sret symbol\n");
    }
    *_hidden_sret_ptr = (Symbol){.name = "_hidden_sret_ptr",
                                 .kind = VAR,
                                 .linkage = LINK_NONE,
                                 .storage = STORAGE_NONE,
                                 .var_decl = NULL,
                                 .type = get_pointer_type(return_type),
                                 .scope_depth = 0};
}
void set_sret(Type *return_type) {
    if (_sret && _sret->type == return_type) return;
    if (!_sret) {
        _sret = malloc(sizeof(Symbol));
        ASSERT(_sret, "Failed to allocate _sret symbol\n");
    }
    *_sret = (Symbol){.name = "_sret",
                      .kind = VAR,
                      .linkage = LINK_NONE,
                      .storage = STORAGE_NONE,
                      .var_decl = NULL,
                      .type = return_type,
                      .scope_depth = 0};
}

const GP_Reg caller_saved_regs[CALLER_SAVED_REGISTERS] = {RAX, RCX, RDX, R8, R9, R10, R11};
const GP_Reg callee_saved_regs[CALLEE_SAVED_REGISTERS] = {RBX, RBP, RDI, RSI, R12, R13, R14, R15};
const GP_Reg int_param_regs[PARAM_REGISTERS] = {RCX, RDX, R8, R9};
const XMM_Reg float_param_regs[PARAM_REGISTERS] = {XMM0, XMM1, XMM2, XMM3};

ABI_Result abi_classify(Type *type) { return (ABI_Result){.class = {}, .memory = type->size > HIDDEN_PTR_SIZE}; }

IR_Value abi_lower_param_register(Type *type, int i) {
    ASSERT(i >= 0 && i < PARAM_REGISTERS, "Win64 ABI Invalid param arg index %d\n", i);
    IR_Value v = (IR_Value){.kind = IR_PHYS_REG,
                            .phys_reg = (PhysReg){.data_kind = REG_DATA_NONE, .size = reg_size(type->size), .offset = 0, .scale = 0}};
    if (type->kind == T_FLOAT) {
        v.phys_reg.kind = REG_XMM;
        v.phys_reg.sse_reg = float_param_regs[i];
    } else {
        v.phys_reg.kind = REG_GP;
        v.phys_reg.gp_reg = int_param_regs[i];
    }
    return v;
}
void abi_lower_store(IR_Function *f, IR_Block *b, IR_Instruction *instr, int *i) {
    Type *s_t = instr->store.type;
    if (s_t->kind != T_STRUCT) return;
    if (s_t->size > MAX_STRUCT_SIZE) {
        PANIC("ABI lower store\n");
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
        WARN("Should be unreachable\n");
        IR_Instruction store = *instr;
        store.store.type = get_integer_type(s_t->size);
        store.ops[0].size = store.store.type->size;
        store.ops[1].size = store.store.type->size;
        set(&b->instruction_array, &store, *i);
    }
}

void abi_lower_param(IR_Function *f, IR_Block *b, IR_Instruction *instr, int *i) {
    Type *type = instr->param.type;
    if (type->size > MAX_STRUCT_SIZE) type = type_u64;
    instr->op_count = 2;
    if (instr->param.param_index < PARAM_REGISTERS) instr->ops[1] = abi_lower_param_register(type, instr->param.param_index);
    else {
        instr->ops[1] = ir_stack_value(8, 8, SHADOW_SPACE + 8 + 16 + 8 * (instr->param.param_index - 4));
    }
    if (instr->param.type->kind == T_STRUCT) {
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
}
void abi_lower_ret(IR_Function *f, IR_Block *b, IR_Instruction *instr, int *i) {
    Type *s_t = instr->ret.type;
    if (s_t->kind == T_STRUCT) {
        if (s_t->size > MAX_STRUCT_SIZE) {
            IR_Value dst = instr->ops[0];
            instr->ops[0] = ir_no_value;
            set_hidden_sret_ptr(s_t);
            instr->ret.type = type_void;
            IR_Instruction memcpy = {
                .op = IR_MEMCPY, .op_count = 2, .ops = {[0] = ir_symbol_value(_hidden_sret_ptr), [1] = dst}, .memcpy = {.size = s_t->size}};
            insert(&b->instruction_array, &memcpy, (*i)++);
        } else instr->ret.type = get_integer_type(s_t->size);
    }
    //         instr->ret.type = get_pointer_type(s_t);
    //         IR_Value local_v = {.kind = IR_VREG, .size = 8, .align = 8, .vreg = f->next_reg++};
    //         f->max_reg++;
    //         IR_Instruction local_addr = {.op = IR_ADDR, .op_count = 2, .ops = {[0] = local_v, [1] = instr->ops[0]}};
    //         IR_Instruction memcpy = {.op = IR_MEMCPY,
    //                                  .op_count = 2,
    //                                  .ops = {[0] = ir_mem_value(0, instr->ret.type), [1] = local_v},
    //                                  .memcpy = {.size = s_t->size}};
    //         instr->ops[0] = ir_no_value;
    //         instr->ret.type = type_void;
    //         insert(&b->instruction_array, &local_addr, (*i)++);
    //         insert(&b->instruction_array, &memcpy, (*i)++);
    //     } else instr->ret.type = get_integer_type(s_t->size);
    // }
}

void abi_emit_call(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    // Type *t = instr->call.type->_func.return_type;
    // if (t->kind == T_STRUCT) t = get_integer_type(t->size);
    Type *t = instr->call.type->abi_func_type->_func.return_type;

    int gp_index = 0;

    const int spilled_count = instr->call.arg_array.count > PARAM_REGISTERS ? instr->call.arg_array.count - PARAM_REGISTERS : 0;
    // +8 for push rbp (call emits push rbp, mov rsp, rbp)
    // 8 * spilled count, for n args after [0-3]
    // SHADOW_SPACE = 32, for windows ABI (linux = 0)
    const int param_frame_size = align(SHADOW_SPACE + 8 * spilled_count, 16);
    // Place first stack param after rbp and Shadow space
    int param_offset = SHADOW_SPACE;
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
        const char *suffix = x86_op_suffix(arg_type);
        switch (arg_type->kind) {
        case T_INT:
        case T_POINTER:
            if (use_register) {
                x86_emit_xr(fp, "mov", suffix, "", &v->v, gp_register_str[int_param_regs[gp_index++]][reg_size(arg_type->size)]);
            } else {
                const char *v_reg = x86_rax_reg(arg_type);
                x86_emit_xr(fp, "mov", suffix, "", &v->v, v_reg);
                fprintf(fp, "    mov%s %s, %d(%%rsp)\n", suffix, v_reg, param_offset);
                param_offset += 8;
            }
            break;
        case T_FLOAT:
            if (use_register) {
                if (instr->call.type->_func.is_variadic) {
                    x86_emit_xr(fp, "mov", x86_integer_op_suffix(arg_type->size), "", &v->v,
                                gp_register_str[int_param_regs[gp_index]][reg_size(arg_type->size)]);
                }
                x86_emit_xr(fp, "mov", suffix, "", &v->v, sse_register_str[float_param_regs[gp_index++]]);
            } else {
                x86_emit_xr(fp, "mov", suffix, "", &v->v, sse_register_str[XMM0]);
                fprintf(fp, "    mov%s %%xmm0, %d(%%rsp)\n", suffix, param_offset);
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
            // Below needs a symbol
            set_sret(type->_func.return_type);
            insert(&abi_type->_func.params,
                   &(ParamDecl){.type = get_pointer_type(abi_type->_func.return_type), .name = "_sret", .symbol = _sret}, 0);
            abi_type->_func.return_type = type_void;
        } else
            abi_type->_func.return_type = abi_type->_func.return_type->kind == T_FLOAT
                                              ? get_float_type(abi_type->_func.return_type->size)
                                              : get_integer_type(abi_type->_func.return_type->size);
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
    fprintf(fp, "    sub $32, %%rsp\n");
    fprintf(fp, "    call memcpy\n");
    fprintf(fp, "    add $32, %%rsp\n");
}
#endif
