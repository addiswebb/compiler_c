#include "compiler_c/ir/ir_builder.h"
#include "compiler_c/abi/abi.h"
#include "compiler_c/core/array.h"
#include "compiler_c/core/node.h"
#include "compiler_c/core/type.h"
#include "compiler_c/ir/ir_gen.h"
#include "compiler_c/ir/ir_module.h"
#include "compiler_c/log/logger.h"

void ir_move(IR_Context *ctx, IR_Value dst, IR_Value src) {
    IR_Instruction i;
    i.op = IR_MOVE;
    i.ops[1] = src;
    i.ops[0] = dst;
    i.op_count = 2;
    ir_append_instruction(ctx, &i);
}

IR_Value ir_load(IR_Context *ctx, IR_Value addr, Type *type) {
    if (type->kind == T_STRUCT && type->size > 8) {
        printf("heere\n");
    }
    IR_Instruction i;
    i.op = IR_LOAD;
    i.ops[1] = addr;
    i.load.type = type;
    i.ops[0] = ir_next_virtual_reg(ctx->func);
    i.op_count = 2;
    ir_append_instruction(ctx, &i);
    return i.ops[0];
}
void ir_zero(IR_Context *ctx, IR_Value dst, Type *type) {
    switch (type->kind) {
    case T_INT:
    case T_ENUM:
    case T_FLOAT:
    case T_POINTER:
        ir_store(ctx, dst, ir_integer_literal(0), ctx->init_ctx.type);
        return;
    case T_ARRAY:
    case T_STRUCT:
    case T_UNION:
        if (type->size <= 8) ir_store(ctx, dst, ir_integer_literal(0), get_integer_type(type->size));
        else ir_memset(ctx, dst, 0, type->size);
        return;
    default:
        PANIC("Tried to zero invalid type %t\n", type);
    }
}
IR_Value ir_store(IR_Context *ctx, IR_Value dst, IR_Value src, Type *type) {
    IR_Instruction i;
    i.op = IR_STORE;
    i.ops[1] = src;
    i.store.type = type->kind == T_ENUM ? type_i32 : type;
    i.ops[0] = dst;
    i.op_count = 2;
    ir_append_instruction(ctx, &i);
    return i.ops[0];
}
IR_Value ir_smart_const(IR_Context *ctx, ConstLiteral *literal, Type *type) {
    // TODO places where ir_integer_literal are used must ensure that $x is allowed in context
    // E.g movss $x, %xmm0 is invalid
    // TODO use %xmm9 for all movements, as %rax for fp, instead of %xmm0 as
    // if %xmm0 holds p0, and %xmm0 is later used to store p5 later, it gets clobbered.
    // Use a safe xmm0 for general operations or place reg params last.
    if (literal->type->kind == T_INT) return ir_integer_literal(literal->i);
    IR_Value l = ir_const(ctx, ir_append_const(ctx->module, literal), type);
    if (literal->type->kind == T_ARRAY && literal->type->base == type_i8) return ir_address(ctx, l, 0);
    else return l;
}
IR_Value ir_const(IR_Context *ctx, int const_index, Type *type) {
    IR_Instruction i;
    i.op = IR_CONST;
    i.ops[1] = (IR_Value){.kind = IR_CONSTANT, .const_index = const_index, .size = type->size, .align = type->align};
    i._const.type = type;
    // Use the .LCx literal for strings, otherwise it was lowered to an asm literal and stored in a register.
    i.ops[0] = type->kind == T_ARRAY && type->base == type_i8 ? i.ops[1] : ir_next_virtual_reg(ctx->func);
    i.op_count = 2;
    ir_append_instruction(ctx, &i);
    return i.ops[0];
}
IR_Value ir_unary(IR_Context *ctx, IR_UNARY_OP op, IR_Value expr_reg, Type *type) {
    IR_Instruction i;
    i.op = IR_UNOP;
    i.unary.op = op;
    i.unary.type = type;
    i.ops[1] = expr_reg;
    i.ops[0] = ir_next_virtual_reg(ctx->func);
    i.op_count = 2;
    ir_append_instruction(ctx, &i);
    return i.ops[0];
}
IR_Value ir_binary(IR_Context *ctx, IR_BINOP_OP op, IR_Value dst, IR_Value lhs_reg, IR_Value rhs_reg, Type *type) {
    IR_Instruction i;
    i.op = IR_BINOP;
    i.binop.op = op;
    i.binop.type = type;
    i.ops[1] = lhs_reg;
    i.ops[2] = rhs_reg;
    i.ops[0] = dst;
    i.op_count = 3;
    ir_append_instruction(ctx, &i);
    return i.ops[0];
}
IR_Value ir_cmp(IR_Context *ctx, IR_CMP_OP op, IR_Value lhs_reg, IR_Value rhs_reg, Type *type) {
    if (type == type_invalid) {
        printf("Found invalid\n");
    }
    IR_Instruction i;
    i.op = IR_CMP;
    i.cmp.op = op;
    i.cmp.type = type;
    i.ops[1] = lhs_reg;
    i.ops[2] = rhs_reg;
    i.ops[0] = ir_next_virtual_reg(ctx->func);
    i.op_count = 3;
    ir_append_instruction(ctx, &i);
    return i.ops[0];
}
IR_Value ir_call(IR_Context *ctx, const Node *expr) {
    IR_Instruction i;
    i.op = IR_CALL;
    ctx->func_not_address = true;
    i.ops[1] = ir_gen_rvalue(ctx, expr->func_call.callee);
    ctx->func_not_address = false;

    i.call.type = expr->func_call.callee->type;
    if (i.call.type->kind == T_POINTER) i.call.type = i.call.type->base;
    ASSERT(i.call.type->abi.type, "Function Type did not recieve ABI type\n");
    array_init(&i.call.arg_array, expr->func_call.params_array.count ? expr->func_call.params_array.count : 1, sizeof(IR_CallArg));
    int hidden_ptr_offset = 0;
    Type *return_type = i.call.type->_func.return_type;
    IR_Value sret;
    bool use_sret = false;
    // TODO Abstract the condition to ABI, so it works for both SysV and Win64
    if (return_type != type_void) {

        ABI_Result res = abi_classify(return_type);
        if (res.memory) {
            // Pass hidden pointer as first arg
            set_sret(return_type);
            Symbol *current_sret_symbol = current_sret();

            append(&ctx->func->locals_array, &current_sret_symbol);
            hidden_ptr_offset++;
            sret = ((IR_CallArg *)append(&i.call.arg_array, &(IR_CallArg){.v = ir_address(ctx, ir_symbol_value(current_sret_symbol), 0),
                                                                          .type = get_pointer_type(return_type)}))
                       ->v;
            use_sret = true;
        }
    }

    for (int j = 0; j < expr->func_call.params_array.count; j++) {
        Node *param = get_node(&expr->func_call.params_array, j);
        Type *arg_type = param->type;

        if (j < i.call.type->abi.type->_func.params.count) {
            ParamDecl *d = (ParamDecl *)get(&i.call.type->abi.type->_func.params, j + hidden_ptr_offset);
            arg_type = d->type;
        }

        IR_Value val;
        ABI_Result res = abi_classify(arg_type);
        // #ifdef _WIN64
        if (res.memory) val = ir_gen_lvalue(ctx, param);
        else if (param->type->kind == T_FUNCTION || is_func_ptr(param->type)) val = ir_gen_lvalue(ctx, param);
        // else if (is_func_ptr(param->type)) val = ir_gen_rvalue(ctx, param);
        else val = ir_gen_rvalue(ctx, param);
        // #else
        //         if (param->type->kind == T_FUNCTION || is_func_ptr(param->type)) val = ir_gen_lvalue(ctx, param);
        //         else val = ir_gen_rvalue(ctx, param);
        // #endif
        append(&i.call.arg_array, &(IR_CallArg){.v = val, .type = arg_type});
    }
    i.ops[0] = ir_next_virtual_reg(ctx->func);
    // Used because ir_call redefines i.ops[0]
    i.op_count = 2;
    ir_append_instruction(ctx, &i);
    if (use_sret) ir_move(ctx, i.ops[0], sret);

    return i.ops[0];
}
IR_Value ir_return(IR_Context *ctx, IR_Value reg, Type *type) {
    IR_Instruction i;
    i.op = IR_RET;
    i.ops[0] = reg;
    i.op_count = 1;
    i.ret.type = type;
    ir_append_instruction(ctx, &i);
    return ir_no_value;
}
IR_Value ir_branch(IR_Context *ctx, IR_Block *block) {
    IR_Instruction i;
    i.op = IR_BR;
    i.br.block = block;
    i.op_count = 0;
    ir_append_instruction(ctx, &i);
    return ir_no_value;
}

IR_Value ir_jmp(IR_Context *ctx, const char *name) {
    IR_Instruction i;
    i.op = IR_JMP;
    i.jmp.name = name;
    i.op_count = 0;
    ir_append_instruction(ctx, &i);
    return ir_no_value;
}

IR_Value ir_label(IR_Context *ctx, const char *name) {
    IR_Instruction i;
    i.op = IR_LABEL;
    i.label.name = name;
    i.op_count = 0;
    ir_append_instruction(ctx, &i);
    return ir_no_value;
}
IR_Value ir_branch_cond(IR_Context *ctx, IR_Value cond_reg, IR_Block *t_block, IR_Block *f_block) {
    IR_Instruction i;
    i.op = IR_BR_COND;
    i.ops[0] = cond_reg;
    i.br_cond.t_block = t_block;
    i.br_cond.f_block = f_block;
    i.op_count = 1;
    ir_append_instruction(ctx, &i);
    return ir_no_value;
}
IR_Value ir_cast(IR_Context *ctx, IR_Value src, Type *to, Type *from) {
    if (get_qualified_type(from, QUAL_NONE) == get_qualified_type(to, QUAL_NONE)) return src;
    if (src.kind == IR_INT_LITERAL && (to->kind == T_INT || to->kind == T_POINTER)) return src;
    if (from->kind == T_ARRAY && to->kind == T_POINTER && from->base->kind == to->base->kind) PANIC("HOW");
    IR_Instruction i;
    i.op = IR_CAST;
    i.cast.from = from;
    i.cast.to = to;
    i.ops[1] = src;
    i.ops[0] = ir_next_virtual_reg(ctx->func);
    i.op_count = 2;
    ir_append_instruction(ctx, &i);
    return i.ops[0];
}
IR_Value ir_address(IR_Context *ctx, IR_Value src, int offset) {
    IR_Instruction i;
    i.op = IR_ADDR;
    i.ops[1] = src;
    i.addr.offset = offset;
    i.ops[0] = ir_next_virtual_reg(ctx->func);
    i.op_count = 2;
    ir_append_instruction(ctx, &i);
    return i.ops[0];
}
IR_Value ir_alloca(IR_Context *ctx, IR_Value dst, int size, int align) {
    IR_Instruction i;
    i.op = IR_ALLOCA;
    i.alloca.size = size;
    i.op_count = 1;
    i.ops[0] = dst;
    ir_append_instruction(ctx, &i);
    return i.ops[0];
}

IR_Value ir_memset(IR_Context *ctx, IR_Value dst, int c, int size) {
    IR_Instruction i;
    i.op = IR_MEMSET;
    i.memset.size = size;
    i.memset.c = c;
    i.ops[0] = dst;
    i.op_count = 1;
    ir_append_instruction(ctx, &i);
    return i.ops[0];
}
IR_Value ir_memcpy(IR_Context *ctx, IR_Value from_reg, IR_Value to_reg, int size) {
    IR_Instruction i;
    i.op = IR_MEMCPY;
    i.ops[1] = from_reg;
    i.memcpy.size = size;
    i.ops[0] = to_reg;
    i.op_count = 2;
    ir_append_instruction(ctx, &i);
    return i.ops[0];
}

IR_Value ir_builtin_va_start(IR_Context *ctx, IR_Value ap, IR_Value first_arg) {
    IR_Instruction i;
    i.op = IR_BUILTIN_VA_START;
    i.ops[0] = ap;
    i.ops[1] = first_arg;
    i.op_count = 2;
    ir_append_instruction(ctx, &i);
    return ir_no_value;
}

IR_Value ir_builtin_va_arg(IR_Context *ctx, IR_Value ap, Type *type) {
    IR_Instruction i;
    i.op = IR_BUILTIN_VA_ARG;
    i.ops[0] = ir_next_virtual_reg(ctx->func);
    i.ops[1] = ap;
    i.op_count = 2;
    i.builtin_va_arg.type = type;
    ir_append_instruction(ctx, &i);
    return i.ops[0];
}

void ir_builtin_va_end() { return; }
