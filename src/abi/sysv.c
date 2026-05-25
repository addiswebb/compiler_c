#include "compiler_c/analyse/analysis_types.h"
#include "compiler_c/compiler.h"
#include "compiler_c/core/arena.h"
#include "compiler_c/core/array.h"
#include <stdio.h>
#ifdef __linux__

#include "../libc/stdbool.h"
#include <compiler_c/abi/abi.h>
#include <compiler_c/analyse/analysis.h>
#include <compiler_c/core/type.h>
#include <compiler_c/ir/ir_builder.h>
#include <compiler_c/ir/ir_gen.h>
#include <compiler_c/ir/ir_module.h>
#include <compiler_c/log/logger.h>
#include <compiler_c/x86/x86.h>

Arena _sret = {.count = 0};
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

Symbol *current_sret() { return arena_get(&_sret, _sret.count - 1); }

void set_sret(Type *return_type) {
    if (_sret.count == 0) arena_init(&_sret, 4, sizeof(Symbol));
    if (_sret.count > 0 && current_sret()->type == return_type) return;

    char *name = malloc(sizeof(char) * 32);
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
    case T_ENUM:
    case T_INT:
    case T_POINTER:
    case T_VOID:
    // TODO make so array type never reaches here (arrays are decayed functionally in genlvalue but not by type)
    case T_ARRAY:
        return (ABI_Result){.class = {ABI_INTEGER, ABI_NO_CLASS}, 0};
    case T_FLOAT:
        return (ABI_Result){.class = {ABI_SSE, ABI_NO_CLASS}, 0};
    case T_STRUCT:
        return classify_struct(type);
    default:
        log_start(LOG_ERROR);
        printf("Classification failed on ");
        print_type(type);
        printf("\n");
        exit(1);
    }
}
IR_Value abi_lower_param_register(Type *type, int i) {
#ifdef __COMPILER_C__
    IR_Value v = (IR_Value){.kind = IR_PHYS_REG};
    v.phys_reg = (PhysReg){.data_kind = REG_DATA_NONE, .size = reg_size(type->size), .offset = 0, .scale = 0};
#else
    IR_Value v = (IR_Value){.kind = IR_PHYS_REG,
                            .phys_reg = (PhysReg){.data_kind = REG_DATA_NONE, .size = reg_size(type->size), .offset = 0, .scale = 0}};
#endif
    if (type->kind == T_FLOAT) {
        ASSERT(i >= 0 && i < FLOAT_PARAM_REGISTERS, "SysV ABI Invalid param arg index %d\n", i);
        v.phys_reg.kind = REG_XMM;
        v.phys_reg.sse_reg = float_param_regs[i];
    } else {
        ASSERT(i >= 0 && i < INTEGER_PARAM_REGISTERS, "SysV ABI Invalid param arg index %d\n", i);
        v.phys_reg.kind = REG_GP;
        v.phys_reg.gp_reg = int_param_regs[i];
    }
    return v;
}

// Does not check members, only space
// TODO: Check members also maybe?
bool is_va_list_type(Type *type) { return type->kind == T_STRUCT && type->size == 24; }

void abi_lower_store(IR_Function *f, IR_Block *b, IR_Instruction *instr, int *i) {
    if (instr->store.type->kind == T_STRUCT) {
        ASSERT(instr->store.type->size <= MAX_STRUCT_SIZE, "[SysV] Cannot IR_STORE structs of 16 bytes or more\n");
        ASSERT(instr->store.type->size <= 8, "[SysV] Not handling tuple sized struct");
        instr->store.type = get_integer_type(instr->store.type->size);
    }
}
void abi_lower_param(IR_Function *f, IR_Block *b, IR_Instruction *instr, int *i, int param_index, int *param_cursor) {
    if (instr->param.param_index == -1) return;
    Type *type = instr->param.type;
    ABI_Result res = abi_classify(type);
    if (res.memory) type = type_u64;
    instr->op_count = 2;
    int param_registers = type->kind == T_FLOAT ? FLOAT_PARAM_REGISTERS : INTEGER_PARAM_REGISTERS;
    const int variadic_space = f->type->_func.is_variadic ? 176 : 0;
    if (instr->param.param_index < param_registers) instr->ops[1] = abi_lower_param_register(type, instr->param.param_index);
    else instr->ops[1] = ir_stack_value(8, 8, 8 * (instr->param.param_index - param_registers) - variadic_space);

    if (instr->param.type->kind == T_STRUCT) {
        if (res.memory) {
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
        } else {
            instr->param.type = type->kind == T_FLOAT ? get_float_type(instr->param.type->size) : get_integer_type(instr->param.type->size);
            ASSERT(res.class[1] == ABI_NO_CLASS, "Structs sized [8 < size <= 16] are not handled yet\n");
        }
    }
}

void abi_lower_ret(IR_Function *f, IR_Block *b, IR_Instruction *instr, int *i) {
    Type *s_t = instr->ret.type;
    if (s_t->kind == T_STRUCT) {
        ABI_Result res = abi_classify(s_t);
        if (res.memory) {
            IR_Value dst = instr->ops[0];
            instr->ops[0] = ir_no_value;
            set_hidden_sret_ptr(s_t);
            instr->ret.type = type_void;
            IR_Instruction memcpy = {
                .op = IR_MEMCPY, .op_count = 2, .ops = {[0] = ir_symbol_value(_hidden_sret_ptr), [1] = dst}, .memcpy = {.size = s_t->size}};
            insert(&b->instruction_array, &memcpy, (*i)++);
        } else {
            instr->ret.type = res.class[0] == ABI_INTEGER ? type_u64 : type_f64;
            ASSERT(res.class[1] == ABI_NO_CLASS, "[SysV] Multi register return types are not yet supported\n");
        }
    }
    return;
}

IR_Value abi_gen_builtin(IR_Context *ctx, const Node *expr) {
    switch (expr->_builtin.kind) {
    case BUILTIN_VA_START: {
        Node *n = get_node(&expr->_builtin.params, 1);
        int param_index = -1;
        // Todo dont use variable at all, just find (type->_func->arg_count -1) *8+16 instead
        for (int z = 0; z < ctx->func->locals_array.count; z++) {
            Symbol *v = get_local_symbol(ctx->func, z);
            if (n->identifier.symbol == v) param_index = z;
        }
        ASSERT(param_index != -1, "Expected named param, got bs.\n");
        Node *ap_node = get_node(&expr->_builtin.params, 0);
        IR_Value ap_addr = ir_gen_lvalue(ctx, ap_node);
        // gp_offset
        ir_store(ctx, ap_addr, ir_integer_literal(ctx->func->type->abi.gp_count * 8), type_u32);
        // fp_offset
        ap_addr = ir_binary(ctx, ADD, ir_next_virtual_reg(ctx->func), ap_addr, ir_integer_literal(4), type_void_ptr);
        ir_store(ctx, ap_addr, ir_integer_literal(48 + ctx->func->type->abi.fp_count * 16), type_u32);
        // overflow_args
        ap_addr = ir_binary(ctx, ADD, ir_next_virtual_reg(ctx->func), ap_addr, ir_integer_literal(4), type_void_ptr);
        IR_Value rbp_addr = ir_address(ctx, ir_stack_value(8, 8, 16), 0);
        ir_store(ctx, ap_addr, rbp_addr, type_void_ptr);
        // reg_save_area
        ap_addr = ir_binary(ctx, ADD, ir_next_virtual_reg(ctx->func), ap_addr, ir_integer_literal(8), type_void_ptr);
        ir_store(ctx, ap_addr, ir_address(ctx, ir_stack_value(8, 8, -176), 0), type_void_ptr);
        return ir_no_value;
    }
    case BUILTIN_VA_ARG: {
        Node *ap_node = get_node(&expr->_builtin.params, 0);
        IR_Value ap_addr = ir_gen_lvalue(ctx, ap_node);
        Type *arg_type = get_node(&expr->_builtin.params, 1)->type;
        if (arg_type->kind == T_FLOAT)
            ap_addr = ir_binary(ctx, ADD, ir_next_virtual_reg(ctx->func), ap_addr, ir_integer_literal(4), type_u32);
        IR_Value offset = ir_load(ctx, ap_addr, type_u32);
        IR_Value is_register_cmp = ir_cmp(ctx, LT, offset, ir_integer_literal(arg_type->kind == T_FLOAT ? 176 : 48), type_i32);
        IR_Block *overflow_block = ir_new_block();
        IR_Block *end_block = ir_new_block();
        ir_branch_cond(ctx, is_register_cmp, NULL, overflow_block);
        /*
            res = *(reg_save_area + offset)
            offset += 8
        */
        IR_Value reg_save_area_addr =
            ir_binary(ctx, ADD, ir_next_virtual_reg(ctx->func), ap_addr, ir_integer_literal(arg_type->kind == T_FLOAT ? 12 : 16), type_u64);
        IR_Value reg_save_area = ir_load(ctx, reg_save_area_addr, type_void_ptr);
        IR_Value reg_save_area_plus_offset = ir_binary(ctx, ADD, ir_next_virtual_reg(ctx->func), reg_save_area, offset, type_u32);
        IR_Value result = ir_load(ctx, reg_save_area_plus_offset, arg_type);

        // Param defines
        IR_Value new_offset =
            ir_binary(ctx, ADD, ir_next_virtual_reg(ctx->func), offset, ir_integer_literal(arg_type->kind == T_FLOAT ? 16 : 8), type_u32);
        ir_store(ctx, ap_addr, new_offset, type_u32);
        ir_branch(ctx, end_block);
        ir_append_block(ctx, overflow_block);
        /*
            res = *overflow_arg_area
            overflow_arg_area += 8
        */
        IR_Value overflow_area_addr = ir_binary(ctx, ADD, ir_next_virtual_reg(ctx->func), ap_addr,
                                                ir_integer_literal(arg_type->kind == T_FLOAT ? 4 : 8), type_void_ptr);
        IR_Value overflow_area = ir_load(ctx, overflow_area_addr, type_void_ptr);
        IR_Value f_res = ir_load(ctx, overflow_area, arg_type);
        IR_Value new_overflow_area =
            ir_binary(ctx, ADD, ir_next_virtual_reg(ctx->func), overflow_area, ir_integer_literal(8), type_void_ptr);
        ir_store(ctx, overflow_area_addr, new_overflow_area, type_void_ptr);
        ir_move(ctx, result, f_res);
        ir_append_block(ctx, end_block);
        return result;
    }
    case BUILTIN_VA_END:
        return ir_no_value;
    case BUILTIN_MEMCPY:
        PANIC("Builtin Memcpy unimplemented\n");
    case BUILTIN_NONE:
        PANIC("Builtin none!\n");
    }
}

void abi_gen_params(IR_Context *ctx, IR_Function *f) {
    int hidde_ptr_offset = 0;
    ABI_Result res = abi_classify(f->type->_func.return_type);
    if (res.memory) {
        set_hidden_sret_ptr(f->type->_func.return_type);
        append(&f->locals_array, &_hidden_sret_ptr);
        ir_append_instruction(ctx, &(IR_Instruction){.op = IR_PARAM,
                                                     .op_count = 1,
                                                     .ops = {[0] = ir_symbol_value(_hidden_sret_ptr)},
                                                     .param = {.param_index = hidde_ptr_offset++, .type = _hidden_sret_ptr->type}});
    }

    int integers_emitted = hidde_ptr_offset;
    int floats_emitted = 0;
    for (int i = 0; i < f->type->_func.params.count; i++) {
        ParamDecl *d = get(&f->type->_func.params, i);
        d->symbol->type = d->type;
        append(&f->locals_array, &d->symbol);
        const int param_index = d->type->kind == T_FLOAT ? floats_emitted++ : integers_emitted++;
        ir_append_instruction(ctx, &(IR_Instruction){.op = IR_PARAM,
                                                     .op_count = 1,
                                                     .ops = {[0] = ir_symbol_value(d->symbol)},
                                                     .param = {.param_index = param_index, .type = d->type}});
    }
    // TODO give more robust way of ensuring correct emission
    // integers_emitted includes all non-float, but gp_count is only non-float registers used.
    // ASSERT(integers_emitted == f->type->abi.gp_count && floats_emitted == f->type->abi.fp_count, "Emission failed\n");
    if (f->type->_func.is_variadic) {
        // Space for this will be allocated later in x86 gen +176 bytes if variadic
        for (int i = integers_emitted; i < INTEGER_PARAM_REGISTERS; i++) {
            ir_append_instruction(ctx, &(IR_Instruction){.op = IR_PARAM,
                                                         .op_count = 1,
                                                         .ops = {[0] = ir_stack_value(8, 8, -8 * (INTEGER_PARAM_REGISTERS - i) - 128)},
                                                         .param = {.param_index = i, .type = type_u64}});
        }
        IR_Value al_equal_zero = ir_cmp(ctx, EQ,
                                        (IR_Value){.kind = IR_PHYS_REG,
                                                   .size = 8,
                                                   .align = 8,
                                                   .phys_reg =
                                                       (PhysReg){
                                                           .kind = REG_GP,
                                                           .gp_reg = RAX,
                                                           .data_kind = REG_DATA_NONE,
                                                           .size = REG_8,
                                                       }},
                                        ir_integer_literal(0), type_u8);

        IR_Block *skip_floats_block = ir_new_block();
        ir_branch_cond(ctx, al_equal_zero, skip_floats_block, NULL);

        // Todo use movaps or move this to x86 lowering
        for (int i = floats_emitted; i < FLOAT_PARAM_REGISTERS; i++) {
            ir_append_instruction(ctx, &(IR_Instruction){.op = IR_PARAM,
                                                         .op_count = 1,
                                                         .ops = {[0] = ir_stack_value(8, 8, -16 * (FLOAT_PARAM_REGISTERS - i))},
                                                         .param = {.param_index = i, .type = type_f64}});
        }
        ir_append_block(ctx, skip_floats_block);
    }
}

void abi_emit_call(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    Type *t = instr->call.type->abi.type->_func.return_type;

    int gp_index = 0;
    int sse_index = 0;

    for (int i = 0; i < instr->call.arg_array.count; i++) {
        IR_CallArg *v = get_call_arg(instr, i);
        if ((v->type->kind == T_INT || v->type->kind == T_POINTER) && gp_index < INTEGER_PARAM_REGISTERS) gp_index++;
        if (v->type->kind == T_FLOAT && sse_index < FLOAT_PARAM_REGISTERS) sse_index++;
    }
    const int spilled_count = instr->call.arg_array.count - (sse_index + gp_index);
    sse_index = 0;
    gp_index = 0;
    const int variadic_space = instr->call.type->_func.is_variadic ? 176 : 0;
    const int param_frame_size = (8 * spilled_count + variadic_space) | 8;
    int param_offset = 0;
    if (param_frame_size > 0) fprintf(fp, "    subq $%d, %%rsp\n", param_frame_size);
    for (int i = 0; i < instr->call.arg_array.count; i++) {
        IR_CallArg *v = get_call_arg(instr, i);
        Type *arg_type = v->type;
        if (arg_type->kind == T_STRUCT) {
            ABI_Result res = abi_classify(arg_type);
            if (res.memory) get_pointer_type(arg_type);
            // else arg_type = res.class[0] == ABI_SSE ? get_float_type(arg_type->size) : get_integer_type(arg_type->size);
            else arg_type = res.class[0] == ABI_INTEGER ? type_u64 : type_f64;
        }
        const char *suffix = x86_op_suffix(arg_type);
        switch (arg_type->kind) {
        case T_INT:
        case T_ENUM:
        case T_POINTER:
            if (gp_index < INTEGER_PARAM_REGISTERS) {
                x86_emit_xr(fp, "mov", x86_op_suffix(arg_type), "", &v->v,
                            gp_register_str[int_param_regs[gp_index++]][reg_size(arg_type->size)]);
            } else {
                const char *reg = x86_rax_reg(arg_type);
                x86_emit_xr(fp, "mov", x86_op_suffix(arg_type), "", &v->v, reg);
                fprintf(fp, "    mov%s %s, %d(%%rsp)\n", x86_op_suffix(arg_type), reg, param_offset);
                param_offset += 8;
            }
            break;
        case T_FLOAT:
            if (sse_index < FLOAT_PARAM_REGISTERS) {
                x86_emit_xr(fp, "mov", suffix, "", &v->v, sse_register_str[float_param_regs[sse_index++]]);
            } else {
                x86_emit_xr(fp, "mov", suffix, "", &v->v, sse_register_str[XMM0]);
                fprintf(fp, "    mov%s %%xmm0, %d(%%rsp)\n", suffix, param_offset);
                param_offset += 8;
            }
            break;
        default:
            PANIC("Tried to emit call arg for unsupported type\n");
        }
    }
    if (instr->call.type->_func.is_variadic) {
        if (sse_index) fprintf(fp, "    movl $%d, %%eax\n", sse_index);
        else fprintf(fp, "    xor %%eax, %%eax\n");
    }

    if (instr->ops[1].kind == IR_PHYS_REG && instr->ops[1].phys_reg.data_kind == REG_DATA_LABEL) {
        fprintf(fp, "    call %s\n", instr->ops[1].phys_reg.label);
    } else {
        x86_emit_xr(fp, "mov", "q", "", &instr->ops[1], "%rax");
        fprintf(fp, "    call *%%rax\n");
    }

    if (param_frame_size > 0) fprintf(fp, "    addq $%d, %%rsp\n", param_frame_size);

    if (t == type_void) return;

    x86_emit_rx(fp, "mov", x86_op_suffix(t), "", x86_rax_reg(t), &instr->ops[0]);
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
            compiler_flags |= FLAG(CF_DEBUG_STRUCT);
            ASSERT(res.class[1] == ABI_NO_CLASS, "[SysV] Not handling tuple return type %t\n", type->_func.return_type);
            abi_type->_func.return_type = res.class[0] == ABI_INTEGER ? type_u64 : type_f64;
        }
    }
    if (abi_type->_func.return_type->kind == T_ENUM) abi_type->_func.return_type = type_i32;
    for (int i = 0; i < abi_type->_func.params.count; i++) {
        ParamDecl *d = get(&abi_type->_func.params, i);
        if (d->type->kind == T_FLOAT && type->abi.fp_count < FLOAT_PARAM_REGISTERS) type->abi.fp_count++;
        else if (type->abi.gp_count < INTEGER_PARAM_REGISTERS) type->abi.gp_count++;
        ABI_Result res = abi_classify(d->type);
        if (res.memory) {
            d->type = get_pointer_type(d->type);
        }
    }
    type->abi.type = abi_type;
}

void abi_gen_memset_instruction(FILE *fp, const IR_Instruction *instr) {
    // TODO: Correctly determine correct lowering for IR_STACK, LITERAL, GLOBAL etc
    switch (instr->ops[0].kind) {
    case IR_CONSTANT:
        x86_emit_xr(fp, "lea", "", "", &instr->ops[0], "%rdi");
        break;
    case IR_PHYS_REG:
        x86_emit_xr(fp, "mov", "q", "", &instr->ops[0], "%rdi");
        break;
    case IR_SYMBOL:
    case IR_INT_LITERAL:
    case IR_VREG:
    case IR_UNDEFINED:
        PANIC("Sanity check failed\n");
    }

    fprintf(fp, "    movl $%d, %%esi\n", instr->memset.c);
    fprintf(fp, "    movq $%d, %%rdx\n", instr->memset.size);
    fprintf(fp, "    call memset\n");
}
void abi_gen_memcpy_instruction(FILE *fp, const IR_Instruction *instr) {
    // TODO: Correctly determine correct lowering for IR_STACK, LITERAL, GLOBAL etc
    switch (instr->ops[1].kind) {
    case IR_CONSTANT:
        x86_emit_xr(fp, "lea", "", "", &instr->ops[1], "%rsi");
        break;
    case IR_PHYS_REG:
        x86_emit_xr(fp, "mov", "q", "", &instr->ops[1], "%rsi");
        break;
    case IR_SYMBOL:
    case IR_INT_LITERAL:
    case IR_VREG:
    case IR_UNDEFINED:
        PANIC("Sanity check failed\n");
    }

    switch (instr->ops[0].kind) {
    case IR_CONSTANT:
        x86_emit_xr(fp, "lea", "", "", &instr->ops[0], "%rdi");
        break;
    case IR_PHYS_REG:
        x86_emit_xr(fp, "mov", "q", "", &instr->ops[0], "%rdi");
        break;
    case IR_SYMBOL:
    case IR_INT_LITERAL:
    case IR_VREG:
    case IR_UNDEFINED:
        PANIC("Sanity check failed\n");
    }
    fprintf(fp, "    movq $%d, %%rdx\n", instr->memcpy.size);
    fprintf(fp, "    call memcpy\n");
}

#endif
