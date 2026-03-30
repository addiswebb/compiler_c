#ifndef _WIN64
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
        log_start(LOG_ERROR);
        printf("Classification failed on ");
        print_type(type);
        printf("\n");
        exit(1);
    }
}
IR_Value abi_lower_param_register(Type *type, int i) {
    IR_Value v = (IR_Value){.kind = IR_PHYS_REG,
                            .phys_reg = (PhysReg){.data_kind = REG_DATA_NONE, .size = reg_size(type->size), .offset = 0, .scale = 0}};
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

void abi_lower_param(IR_Function *f, IR_Block *b, IR_Instruction *instr, int *i) {

    Type *type = instr->param.type;
    ABI_Result res = abi_classify(type);
    if (res.memory) type = type_u64;
    instr->op_count = 2;
    int param_registers = type->kind == T_FLOAT ? FLOAT_PARAM_REGISTERS : INTEGER_PARAM_REGISTERS;
    if (instr->param.param_index < param_registers) instr->ops[1] = abi_lower_param_register(type, instr->param.param_index);
    else instr->ops[1] = ir_stack_value(8, 8, 8 * (instr->param.param_index - param_registers));

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

            set(&b->instruction_array, &param_instr, (*i));
            insert(&b->instruction_array, &addr, (*i + 1));
            (*i)++;
            insert(&b->instruction_array, &memcpy, (*i + 1));
            (*i)++;
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

IR_Value abi_gen_builtin(IR_Context *ctx, const Node *expr) { PANIC("[SysV] Builtins not implemented yet\n"); }

void abi_emit_call(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    Type *t = instr->call.type->abi_func_type->_func.return_type;

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

    const int param_frame_size = 8 * spilled_count;
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

Type *abi_func_type(Type *type) {
    ASSERT(type->kind == T_FUNCTION, "Invalid Func Type\n");
    bool changed = false;
    Type *abi_type = new_type();
    memcpy(abi_type, type, sizeof(Type));
    array_init(&abi_type->_func.params, type->_func.params.capacity, type->_func.params.element_size);
    memcpy(abi_type->_func.params.data, type->_func.params.data, type->_func.params.count * type->_func.params.element_size);
    abi_type->_func.params.count = type->_func.params.count;

    if (abi_type->_func.return_type->kind == T_STRUCT) {

        ABI_Result res = abi_classify(type->_func.return_type);
        if (res.memory) {
            set_sret(type->_func.return_type);
            insert(&abi_type->_func.params,
                   &(ParamDecl){.type = get_pointer_type(abi_type->_func.return_type), .name = "_sret", .symbol = _sret}, 0);
            abi_type->_func.return_type = type_void;
        } else {
            abi_type->_func.return_type = res.class[0] == ABI_INTEGER ? type_u64 : type_f64;
            ASSERT(res.class[1] == ABI_NO_CLASS, "[SysV] Not handling tuple return type\n");
        }
        changed = true;
    }
    for (int i = 0; i < abi_type->_func.params.count; i++) {
        ParamDecl *d = get(&abi_type->_func.params, i);
        ABI_Result res = abi_classify(d->type);
        if (res.memory) {
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
    fprintf(fp, "    mov $%d, %%rdx\n", instr->memcpy.size);
    fprintf(fp, "    call memcpy\n");
}

#endif
