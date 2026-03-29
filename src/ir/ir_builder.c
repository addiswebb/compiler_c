#include "compiler_c/ir/ir_builder.h"
#include "compiler_c/abi/abi.h"
#include "compiler_c/core/array.h"
#include "compiler_c/core/node.h"
#include "compiler_c/core/type.h"
#include "compiler_c/ir/ir_gen.h"
#include "compiler_c/ir/ir_module.h"
#include "compiler_c/log/logger.h"

IR_Value ir_load(IR_Context *ctx, IR_Value addr, Type *type) {
    IR_Instruction i;
    i.op = IR_LOAD;
    i.ops[1] = addr;
    i.load.type = type;
    i.ops[0] = ir_next_virtual_reg(ctx->func);
    i.op_count = 2;
    ir_append_instruction(ctx->block, &i);
    return i.ops[0];
}
IR_Value ir_store(IR_Context *ctx, IR_Value dst, IR_Value src, Type *type) {
    IR_Instruction i;
    i.op = IR_STORE;
    i.ops[1] = src;
    i.store.type = type->kind == T_ENUM ? type_i32 : type;
    i.ops[0] = dst;
    i.op_count = 2;
    ir_append_instruction(ctx->block, &i);
    return i.ops[0];
}
IR_Value ir_smart_const(IR_Context *ctx, IR_Literal *literal, Type *type) {
    // TODO places where ir_integer_literal are used must ensure that $x is allowed in context
    // E.g movss $x, %xmm0 is invalid
    // Also note that if %xmm0 holds p0, and %xmm0 is later used to store p5 later, it gets clobbered.
    // Use a safe xmm0 for general operations or place reg params last.
    if (literal->type->kind == T_INT) return ir_integer_literal(literal->i);
    IR_Value l = ir_const(ctx, ir_append_literal(ctx->module, literal), type);
    if (literal->type->kind == T_ARRAY && literal->type->base == type_i8) return ir_address(ctx, l, 0);
    else return l;
}
// TODO: Always use the .LCx label, and replace it in analysis with the int literal (if type is compatible integer)
IR_Value ir_const(IR_Context *ctx, int const_index, Type *type) {
    IR_Instruction i;
    i.op = IR_CONST;
    i.ops[1] = (IR_Value){.kind = IR_CONSTANT, .const_index = const_index, .size = type->size, .align = type->align};
    i._const.type = type;
    // Use the .LCx literal for strings, otherwise it was lowered to an asm literal and stored in a register.
    i.ops[0] = type->kind == T_ARRAY && type->base == type_i8 ? i.ops[1] : ir_next_virtual_reg(ctx->func);
    i.op_count = 2;
    ir_append_instruction(ctx->block, &i);
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
    ir_append_instruction(ctx->block, &i);
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
    ir_append_instruction(ctx->block, &i);
    return i.ops[0];
}
IR_Value ir_cmp(IR_Context *ctx, IR_CMP_OP op, IR_Value lhs_reg, IR_Value rhs_reg, Type *type) {
    IR_Instruction i;
    i.op = IR_CMP;
    i.cmp.op = op;
    i.cmp.type = type;
    i.ops[1] = lhs_reg;
    i.ops[2] = rhs_reg;
    i.ops[0] = ir_next_virtual_reg(ctx->func);
    i.op_count = 3;
    ir_append_instruction(ctx->block, &i);
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
    ASSERT(i.call.type->abi_func_type, "Function Type did not recieve ABI type\n");
    array_init(&i.call.arg_array, expr->func_call.params_array.count ? expr->func_call.params_array.count : 1, sizeof(IR_CallArg));
    for (int j = 0; j < expr->func_call.params_array.count; j++) {
        Node *param = get_node(&expr->func_call.params_array, j);
        Type *arg_type = param->type;
        if (j < i.call.type->abi_func_type->_func.params.count)
            arg_type = ((ParamDecl *)get(&i.call.type->abi_func_type->_func.params, j))->type;

        IR_Value val;
        if (param->type->kind == T_STRUCT && param->type->size > MAX_STRUCT_SIZE) val = ir_gen_lvalue(ctx, param);
        else if (param->type->kind == T_FUNCTION || is_func_ptr(param->type)) val = ir_gen_lvalue(ctx, param);
        // else if (is_func_ptr(param->type)) val = ir_gen_rvalue(ctx, param);
        else val = ir_gen_rvalue(ctx, param);

        append(&i.call.arg_array, &(IR_CallArg){.v = val, .type = arg_type});
    }
    i.ops[0] = ir_next_virtual_reg(ctx->func);
    i.op_count = 2;
    ir_append_instruction(ctx->block, &i);
    return i.ops[0];
}
IR_Value ir_return(IR_Context *ctx, IR_Value reg, Type *type) {
    IR_Instruction i;
    i.op = IR_RET;
    i.ops[0] = reg;
    i.op_count = 1;
    i.ret.type = type;
    ir_append_instruction(ctx->block, &i);
    return ir_no_value;
}
IR_Value ir_branch(IR_Context *ctx, IR_Block *block) {
    IR_Instruction i;
    i.op = IR_BR;
    i.br.block = block;
    i.op_count = 0;
    ir_append_instruction(ctx->block, &i);
    return ir_no_value;
}

IR_Value ir_jmp(IR_Context *ctx, const char *name) {
    IR_Instruction i;
    i.op = IR_JMP;
    i.jmp.name = name;
    i.op_count = 0;
    ir_append_instruction(ctx->block, &i);
    return ir_no_value;
}

IR_Value ir_label(IR_Context *ctx, const char *name) {
    IR_Instruction i;
    i.op = IR_LABEL;
    i.label.name = name;
    i.op_count = 0;
    ir_append_instruction(ctx->block, &i);
    return ir_no_value;
}
IR_Value ir_branch_cond(IR_Context *ctx, IR_Value cond_reg, IR_Block *t_block, IR_Block *f_block) {
    IR_Instruction i;
    i.op = IR_BR_COND;
    i.ops[0] = cond_reg;
    i.br_cond.t_block = t_block;
    i.br_cond.f_block = f_block;
    i.op_count = 1;
    ir_append_instruction(ctx->block, &i);
    return ir_no_value;
}
IR_Value ir_cast(IR_Context *ctx, IR_Value src, Type *to, Type *from) {
    if (src.kind == IR_INT_LITERAL) return src;
    if (from->kind == T_ARRAY && to->kind == T_POINTER && from->base->kind == to->base->kind) PANIC("HOW");
    IR_Instruction i;
    i.op = IR_CAST;
    i.cast.from = from;
    i.cast.to = to;
    i.ops[1] = src;
    i.ops[0] = ir_next_virtual_reg(ctx->func);
    i.op_count = 2;
    ir_append_instruction(ctx->block, &i);
    return i.ops[0];
}
IR_Value ir_address(IR_Context *ctx, IR_Value src, int offset) {
    IR_Instruction i;
    i.op = IR_ADDR;
    i.ops[1] = src;
    i.addr.offset = offset;
    i.ops[0] = ir_next_virtual_reg(ctx->func);
    i.op_count = 2;
    ir_append_instruction(ctx->block, &i);
    return i.ops[0];
}
IR_Value ir_alloca(IR_Context *ctx, IR_Value dst, int size, int al) {
    IR_Instruction i;
    i.op = IR_ALLOCA;
    i.alloca.size = size;
    i.op_count = 1;
    i.ops[0] = dst;
    ir_append_instruction(ctx->block, &i);
    return i.ops[0];
}

IR_Value ir_memcpy(IR_Context *ctx, IR_Value from_reg, IR_Value to_reg, int size) {
    IR_Instruction i;
    i.op = IR_MEMCPY;
    i.ops[1] = from_reg;
    i.memcpy.size = size;
    i.ops[0] = to_reg;
    i.op_count = 2;
    ir_append_instruction(ctx->block, &i);
    return i.ops[0];
}

IR_Value ir_builtin_va_start(IR_Context *ctx, IR_Value ap, IR_Value last_named_param) {
    IR_Instruction i;
    i.op = IR_BUILTIN_VA_START;
    i.ops[0] = ap;
    i.ops[1] = last_named_param;
    i.op_count = 2;
    ir_append_instruction(ctx->block, &i);
    return ir_no_value;
}

IR_Value ir_builtin_va_arg(IR_Context *ctx, IR_Value ap, Type *type) {
    IR_Instruction i;
    i.op = IR_BUILTIN_VA_ARG;
    i.ops[0] = ir_next_virtual_reg(ctx->func);
    i.ops[1] = ap;
    i.op_count = 2;
    i.builtin_va_arg.type = type;
    ir_append_instruction(ctx->block, &i);
    return i.ops[0];
}

void ir_builtin_va_end() { return; }
