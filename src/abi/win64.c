#ifdef _WIN64

#include "compiler_c/abi/abi.h"
#include "compiler_c/analyse/analysis.h"
#include "compiler_c/core/type.h"
#include "compiler_c/ir/ir_builder.h"
#include "compiler_c/ir/ir_gen.h"
#include "compiler_c/ir/ir_module.h"
#include "compiler_c/log/logger.h"
#include "compiler_c/x86/x86.h"

Arena _sret = {.count = 0};
Symbol *_hidden_sret_ptr = NULL;

Symbol *current_sret() { return arena_get(&_sret, _sret.count - 1); }

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
    if (_sret.count == 0) arena_init(&_sret, 4, sizeof(Symbol));

    char *name = malloc(32);
    ASSERT(name, "Failed to malloc _sret name\n");
    snprintf(name, 32, "_sret%d", _sret.count);
    arena_append(&_sret, &(Symbol){.name = name,
                                   .kind = VAR,
                                   .linkage = LINK_NONE,
                                   .storage = STORAGE_NONE,
                                   .var_decl = NULL,
                                   .type = return_type,
                                   .scope_depth = 0});
}

const GP_Reg caller_saved_regs[CALLER_SAVED_REGISTERS] = {RAX, RCX, RDX, R8, R9, R10, R11};
const GP_Reg callee_saved_regs[CALLEE_SAVED_REGISTERS] = {RBX, RBP, RDI, RSI, R12, R13, R14, R15};
const GP_Reg int_param_regs[PARAM_REGISTERS] = {RCX, RDX, R8, R9};
const XMM_Reg float_param_regs[PARAM_REGISTERS] = {XMM0, XMM1, XMM2, XMM3};

ABI_Result abi_classify(Type *type) {
    return (ABI_Result){.class = {[0] = type->kind == T_FLOAT ? ABI_SSE : ABI_INTEGER, [1] = ABI_NO_CLASS},
                        .memory = type->size > HIDDEN_PTR_SIZE};
}

void abi_lower_store(IR_Function *f, IR_Block *b, IR_Instruction *instr, int *i) {
    if (instr->store.type->kind == T_STRUCT) {
        ASSERT(instr->store.type->size <= 8, "[SysV] IR_STORE only for 8 bytes or less given %d", instr->store.type->size);
        instr->store.type = get_integer_type(instr->store.type->size);
    }
}
IR_Value abi_lower_param_register(Type *type, int i) {
    ASSERT(i >= 0 && i < PARAM_REGISTERS, "Win64 ABI Invalid param arg index %d\n", i);
    // TODO investigate if below is still needed after rework
#ifdef __COMPILER_C__
    IR_Value v = (IR_Value){.kind = IR_PHYS_REG};
    v.phys_reg = (PhysReg){.data_kind = REG_DATA_NONE, .size = reg_size(type->size), .offset = 0, .scale = 0};
#else
    IR_Value v = (IR_Value){.kind = IR_PHYS_REG,
                            .phys_reg = (PhysReg){.data_kind = REG_DATA_NONE, .size = reg_size(type->size), .offset = 0, .scale = 0}};
#endif
    if (type->kind == T_FLOAT) {
        v.phys_reg.kind = REG_XMM;
        v.phys_reg.sse_reg = float_param_regs[i];
    } else {
        v.phys_reg.kind = REG_GP;
        v.phys_reg.gp_reg = int_param_regs[i];
    }
    return v;
}

void abi_lower_param(IR_Function *f, IR_Block *b, IR_Instruction *instr, int *i, int param_index, int *param_cursor) {
    Type *type = instr->param.type;
    if (type->size > MAX_STRUCT_SIZE) type = type_u64;
    instr->op_count = 2;
    if (instr->param.param_index < PARAM_REGISTERS) instr->ops[1] = abi_lower_param_register(type, instr->param.param_index);
    else instr->ops[1] = ir_stack_value(8, 8, SHADOW_SPACE + 16 + 8 * (instr->param.param_index - 4));

    if (instr->param.type->kind == T_STRUCT) {
        if (instr->param.type->size > MAX_STRUCT_SIZE) {
            IR_Value hidden_ptr = (IR_Value){.kind = IR_VREG, .size = 8, .align = 8, .vreg = f->next_reg++};
            f->max_reg++;
            IR_Value s_addr = (IR_Value){.kind = IR_VREG, .size = 8, .align = 8, .vreg = f->next_reg++};
            f->max_reg++;
            IR_Instruction param_instr = {.op = IR_PARAM,
                                          .op_count = 2,
                                          .ops = {[0] = hidden_ptr, [1] = instr->ops[1]},
                                          .param = {.param_index = -1, .type = get_pointer_type(instr->param.type)}};
            IR_Instruction addr = {.op = IR_ADDR, .op_count = 2, .ops = {[0] = s_addr, [1] = instr->ops[0]}};
            IR_Instruction memcpy = {
                .op = IR_MEMCPY, .op_count = 2, .ops = {[0] = s_addr, [1] = hidden_ptr}, .memcpy = {.size = instr->param.type->size}};

            set(&b->instruction_array, &param_instr, param_index);
            insert(&b->instruction_array, &addr, *param_cursor);
            (*i)++;
            (*param_cursor)++;
            insert(&b->instruction_array, &memcpy, *param_cursor);
            (*i)++;
            (*param_cursor)++;
        } else instr->param.type = get_integer_type(instr->param.type->size);
    } else if (instr->param.type->kind == T_ENUM) instr->param.type = type_i32;
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
}
IR_Value abi_gen_builtin(IR_Context *ctx, const Node *expr) {
    switch (expr->_builtin.kind) {
    case BUILTIN_VA_START:
        Node *n = get_node(&expr->_builtin.params, 1);
        int param_index = -1;
        // Todo make more robust, have symbol store is_param_symbol and check it
        for (int z = 0; z < ctx->func->locals_array.count; z++) {
            Symbol *v = get_local_symbol(ctx->func, z);
            if (n->identifier.symbol == v) param_index = z;
        }
        ASSERT(param_index != -1, "Expected named param, got bs.\n");
        Node *ap_node = get_node(&expr->_builtin.params, 0);
        IR_Value ap_addr = ir_gen_lvalue(ctx, ap_node);
        IR_Value addr = ir_address(ctx, ir_stack_value(8, 8, param_index * 8 + 16), 0);
        return ir_store(ctx, ap_addr, addr, ap_node->type);
    case BUILTIN_VA_ARG:
        ap_node = get_node(&expr->_builtin.params, 0);
        ap_addr = ir_gen_lvalue(ctx, ap_node);
        IR_Value new_addr =
            ir_binary(ctx, ADD, ir_next_virtual_reg(ctx->func), ir_load(ctx, ap_addr, ap_node->type), ir_integer_literal(8), ap_node->type);
        ir_store(ctx, ap_addr, new_addr, ap_node->type);
        return ir_load(ctx, new_addr, get_node(&expr->_builtin.params, 1)->type);
    case BUILTIN_VA_END:
        return ir_no_value;
    case BUILTIN_NONE:
    case BUILTIN_MEMCPY:
        PANIC("Builtin none!\n");
    }
}

void abi_gen_params(IR_Context *ctx, IR_Function *f) {
    int hidden_ptr_offset = 0;
    ABI_Result res = abi_classify(f->type->_func.return_type);
    if (res.memory) {
        set_hidden_sret_ptr(f->type->_func.return_type);
        append(&f->locals_array, &_hidden_sret_ptr);
        ir_append_instruction(ctx, &(IR_Instruction){.op = IR_PARAM,
                                                     .op_count = 1,
                                                     .ops = {[0] = ir_symbol_value(_hidden_sret_ptr)},
                                                     .param = {.param_index = hidden_ptr_offset++, .type = _hidden_sret_ptr->type}});
    }

    int params_emitted = hidden_ptr_offset;
    for (int i = 0; i < f->type->_func.params.count; i++) {
        ParamDecl *d = get(&f->type->_func.params, i);
        d->symbol->type = d->type;
        append(&f->locals_array, &d->symbol);
        ir_append_instruction(ctx, &(IR_Instruction){.op = IR_PARAM,
                                                     .op_count = 1,
                                                     .ops = {[0] = ir_symbol_value(d->symbol)},
                                                     .param = {.param_index = params_emitted++, .type = d->type}});
    }
    if (f->type->_func.is_variadic) {
        for (int i = params_emitted; i < PARAM_REGISTERS; i++) {
            ir_append_instruction(ctx, &(IR_Instruction){.op = IR_PARAM,
                                                         .op_count = 1,
                                                         .ops = {[0] = ir_stack_value(8, 8, 16 + i * 8)},
                                                         .param = {.param_index = params_emitted++, .type = type_u64}});
        }
    }
}

void abi_func_type_gen(Type *type) {
    ASSERT(type->kind == T_FUNCTION, "Invalid Func Type\n");
    Type *abi_type = new_type();
    memcpy(abi_type, type, sizeof(Type));
    array_init(&abi_type->_func.params, type->_func.params.capacity, type->_func.params.element_size);
    memcpy(abi_type->_func.params.data, type->_func.params.data, type->_func.params.count * type->_func.params.element_size);
    abi_type->_func.params.count = type->_func.params.count;
    type->abi.fp_count = 0;
    type->abi.gp_count = 0;
    if (abi_type->_func.return_type->kind == T_STRUCT) {
        ABI_Result res = abi_classify(type->_func.return_type);
        if (res.memory) {
            set_sret(type->_func.return_type);
            Symbol *_sret = current_sret();
            insert(&abi_type->_func.params,
                   &(ParamDecl){.type = get_pointer_type(abi_type->_func.return_type), .name = _sret->name, .symbol = _sret}, 0);
            abi_type->_func.return_type = type_void;
        } else {
            ASSERT(res.class[1] == ABI_NO_CLASS, "[Win64] Not handling tuple return type\n");
            abi_type->_func.return_type = res.class[0] == ABI_INTEGER ? type_u64 : type_f64;
        }
    }
    int register_count = 0;

    if (abi_type->_func.return_type->kind == T_ENUM) abi_type->_func.return_type = type_i32;
    for (int i = 0; i < abi_type->_func.params.count; i++) {
        ParamDecl *d = get(&abi_type->_func.params, i);
        ABI_Result res = abi_classify(d->type);
        if (type->abi.gp_count < PARAM_REGISTERS) type->abi.gp_count++;
        if (res.memory) d->type = get_pointer_type(d->type);
    }
    type->abi.type = abi_type;
}

bool is_va_list_type(Type *type) { return type->kind == T_POINTER && type->base == type_i8; }

Type *to_arg_type(Type *t, ABI_Result *res) {
    switch (t->kind) {
    case T_INT:
    case T_FLOAT:
    case T_POINTER:
        return t;
    case T_ENUM:
        return type_i32;
    case T_ARRAY:
    case T_STRUCT:
    case T_UNION:
        if (res->memory) return get_pointer_type(t);
        else return res->class[0] == ABI_INTEGER ? get_integer_type(t->size) : get_float_type(t->size);
    default:
        PANIC("Invalid arg type %t\n", t);
    }
}

void abi_emit_call(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    Type *t = instr->call.type->abi.type->_func.return_type;

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
        ABI_Result res = abi_classify(v->type);
        Type *arg_type = to_arg_type(v->type, &res);

        bool use_register = false;
        use_register = gp_index < PARAM_REGISTERS;
        const char *suffix = x86_op_suffix(arg_type);
        switch (arg_type->kind) {
        case T_INT:
        case T_ENUM:
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

// void *abi_func_type(Type *type) { instr->param.type = type_i32;
//     ASSERT(type->kind == T_FUNCTION, "Invalid Func Type\n");
//     Type *abi_type = new_type();
//     memcpy(abi_type, type, sizeof(Type));
//     array_init(&abi_type->_func.params, type->_func.params.capacity, type->_func.params.element_size);
//     memcpy(abi_type->_func.params.data, type->_func.params.data, type->_func.params.count * type->_func.params.element_size);
//     abi_type->_func.params.count = type->_func.params.count;

//     type->abi.fp_count = 0;
//     type->abi.gp_count = 0;
//     if (type->_func.return_type->kind == T_STRUCT) {
//         if (abi_type->_func.return_type->size > MAX_STRUCT_SIZE) {
//             set_sret(type->_func.return_type);
//             Symbol *_sret = current_sret();
//             insert(&abi_type->_func.params,
//                    &(ParamDecl){.type = get_pointer_type(abi_type->_func.return_type), .name = _sret->name, .symbol = _sret}, 0);
//             abi_type->_func.return_type = type_void;
//         } else
//             abi_type->_func.return_type = abi_type->_func.return_type->kind == T_FLOAT
//                                               ? get_float_type(abi_type->_func.return_type->size)
//                                               : get_integer_type(abi_type->_func.return_type->size);
//     }
//     if (abi_type->_func.return_type->kind == T_ENUM) abi_type->_func.return_type = type_i32;
//     for (int i = 0; i < abi_type->_func.params.count; i++) {
//         ParamDecl *d = get(&abi_type->_func.params, i);
//         ABI_Result res = abi_classify(d->type);
//         if (res.memory) d->type = get_pointer_type(d->type);
//         else if (type->abi.gp_count < PARAM_REGISTERS) type->abi.gp_count++;
//     }
//     type->abi.type = abi_type;
// }

void abi_gen_memset_instruction(FILE *fp, const IR_Instruction *instr) {
    // TODO: Correctly determine correct lowering for IR_STACK, LITERAL, GLOBAL etc
    switch (instr->ops[0].kind) {
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

    fprintf(fp, "    mov $%d, %%rdx\n", instr->memset.c);
    fprintf(fp, "    movq $%d, %%r8\n", instr->memset.size);
    fprintf(fp, "    sub $32, %%rsp\n");
    fprintf(fp, "    call memset\n");
    fprintf(fp, "    add $32, %%rsp\n");
}
void abi_gen_memcpy_instruction(FILE *fp, const IR_Instruction *instr) {
    // TODO: Correctly determine correct lowering for IR_STACK, LITERAL, GLOBAL etc

    switch (instr->ops[0].kind) {
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

    fprintf(fp, "    mov $%d, %%r8\n", instr->memcpy.size);
    fprintf(fp, "    sub $32, %%rsp\n");
    fprintf(fp, "    call memcpy\n");
    fprintf(fp, "    add $32, %%rsp\n");
}
#endif
