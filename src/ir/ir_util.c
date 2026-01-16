#include "compiler_c/ir.h"
#include "compiler_c/type.h"
#include <stdio.h>
#include <stdlib.h>
IR_CMP_OP ir_cmp_op(const TokenType type) {
    switch (type) {
    case TK_EQ_EQ:
        return EQ;
    case TK_NEQ:
        return NEQ;
    case TK_LT:
        return LT;
    case TK_LE:
        return LE;
    case TK_GT:
        return GT;
    case TK_GE:
        return GE;
    default:
        printf("Given an unsupported token to convert to IR cmp op: ");
        print_token_type(type);
        printf("\n");
        exit(1);
    }
}
IR_UNARY_OP ir_unary_op(const TokenType type) {
    switch (type) {
    case TK_PLUS:
        return POS;
    case TK_MINUS:
        return NEG;
    case TK_L_NOT:
        return LNOT;
    case TK_BW_NOT:
        return BNOT;
    default:
        printf("Given a token which is not a unary operator\n");
        exit(1);
    }
}
IR_BINOP_OP ir_binary_op(const TokenType type) {
    switch (type) {
    case TK_PLUS:
        return ADD;
    case TK_MINUS:
        return SUB;
    case TK_MULTIPLY:
        return MUL;
    case TK_DIVIDE:
        return DIV;
    case TK_MOD:
        return MOD;
    case TK_AND:
        return AND;
    case TK_OR:
        return OR;
    case TK_XOR:
        return XOR;
    case TK_SHL:
        return SHL;
    case TK_SHR:
        return SHR;
    default:
        printf("Given an unsupported token to convert to IR Binary op: ");
        print_token_type(type);
        printf("\n");
        exit(1);
    }
}
static void print_unary_op(IR_UNARY_OP op) {
    switch (op) {
    case POS:
        printf("POS");
        break;
    case NEG:
        printf("NEG");
        break;
    case LNOT:
        printf("LNOT");
        break;
    case BNOT:
        printf("BNOT");
        break;
    case REF:
        printf("REF");
        break;
    case DEREF:
        printf("DEREF");
        break;
    }
}

static void print_binary_op(IR_BINOP_OP op) {
    switch (op) {
    case ADD:
        printf("ADD");
        break;
    case SUB:
        printf("SUB");
        break;
    case MUL:
        printf("MUL");
        break;
    case DIV:
        printf("DIV");
        break;
    case MOD:
        printf("MOD");
        break;
    case AND:
        printf("AND");
        break;
    case OR:
        printf("OR");
        break;
    case XOR:
        printf("XOR");
        break;
    case SHR:
        printf("SHR");
        break;
    case SHL:
        printf("SHL");
        break;
    }
}
static void print_cmp_op(IR_CMP_OP op) {
    switch (op) {
    case LT:
        printf("LT");
        break;
    case LE:
        printf("LE");
        break;
    case GT:
        printf("GT");
        break;
    case GE:
        printf("GE");
        break;
    case EQ:
        printf("EQ");
        break;
    case NEQ:
        printf("NEQ");
        break;
    }
}

static const char ir_type_suffix(Type *type) {
    switch (type->kind) {
    case T_INVALID:
        printf("Tried to print invalid type\n");
        exit(1);
    case T_INT:
        return 'i';
    case T_FLOAT:
        return 'f';
    case T_CHAR:
        return 'c';
    case T_POINTER:
        return 'p';
    }
}

static void print_ir_instruction(IR_Context *ctx, const IR_Instruction *instr) {
    switch (instr->op) {
    case IR_CONST:
        switch (instr->_const.type->kind) {
        case T_INT:
            printf("    r%d = CONST c%d, %d", instr->dst, instr->_const.pool_index,
                   ctx->module->const_pool.consts[instr->_const.pool_index].i);
            break;
        case T_FLOAT:
            printf("    r%d = CONST c%d, %g", instr->dst, instr->_const.pool_index,
                   ctx->module->const_pool.consts[instr->_const.pool_index].f);
            break;
        case T_CHAR:
            printf("    r%d = CONST c%d, %c", instr->dst, instr->_const.pool_index,
                   ctx->module->const_pool.consts[instr->_const.pool_index].c);
            break;
        case T_POINTER:
            if (instr->_const.type->ptr_to->kind == T_CHAR) {
                printf("    r%d = CONST c%d, \"%s\"", instr->dst, instr->_const.pool_index,
                       ctx->module->const_pool.consts[instr->_const.pool_index].s);
                break;
            }
        default:
            printf("Tried to print IR_CONST of unknown type\n");
            exit(1);
        }
        break;
    case IR_BINOP:
        printf("    r%d = BINOP:%c r%d, r%d, ", instr->dst, ir_type_suffix(instr->binop.type), instr->binop.lhs, instr->binop.rhs);
        print_binary_op(instr->binop.op);
        break;
    case IR_LOAD:
        printf("    r%d = LOAD:%c r%d", instr->dst, ir_type_suffix(instr->load.type), instr->load.addr);
        break;
    case IR_STORE:
        printf("    STORE:%c r%d -> r%d", ir_type_suffix(instr->store.type), instr->store.src, instr->dst);
        break;
    case IR_RET:
        printf("    RET r%d", instr->ret.value);
        break;
    case IR_CALL:
        printf("    r%d = CALL:%c %s, %d:", ir_type_suffix(instr->call.type), instr->dst, ctx->module->defs[instr->call.callee].name,
               instr->call.arg_count);
        printf("[ ");
        for (int i = 0; i < instr->call.arg_count; i++) {
            printf("r%d", instr->call.args[i]);
            if (i < instr->call.arg_count - 1) {
                printf(", ");
            }
        }
        printf(" ]");
        break;
    case IR_BR:
        printf("    BR %d", instr->br.label);
        break;
    case IR_BR_COND:
        printf("    BR_COND r%d, %d %d", instr->br_cond.cond, instr->br_cond.t_label, instr->br_cond.f_label);
        break;
    case IR_CMP:
        printf("    r%d = CMP ", instr->dst);
        printf("r%d, r%d, ", instr->cmp.lhs, instr->cmp.rhs);
        print_cmp_op(instr->cmp.op);
        break;
    case IR_UNOP:
        printf("    r%d = UNARYOP r%d, ", instr->dst, instr->unary.expr);
        print_unary_op(instr->unary.op);
        break;
    case IR_CAST:
        printf("    r%d = CAST %c to %c, r%d, ", instr->dst, ir_type_suffix(instr->cast.from), ir_type_suffix(instr->cast.to),
               instr->cast.src);
        break;
    case IR_ADDR:
        printf("    r%d = ADDR r%d+%d", instr->dst, instr->addr.src, instr->addr.offset);
        break;
    }
    printf("\n");
}

static void print_ir_block(IR_Context *ctx, const IR_Block *block) {
    for (int i = 0; i < block->count; i++) {
        print_ir_instruction(ctx, &block->instructions[i]);
    }
}

static void print_ir_function(IR_Context *ctx, const IR_Function *func) {
    printf("%s:\n", func->name);
    for (int i = 0; i < func->block_count; i++) {
        printf("L%d:\n", i);
        print_ir_block(ctx, &func->blocks[i]);
    }
}

void print_ir_module(IR_Context *ctx, const IR_Module *module) {
    for (int i = 0; i < module->func_count; i++) {
        print_ir_function(ctx, module->functions[i]);
    }
}
