#include "compiler_c/ir/ir_util.h"
#include "compiler_c/abi/abi.h"
#include "compiler_c/analyse/analysis_types.h"
#include "compiler_c/core/type.h"
#include "compiler_c/ir/ir_module.h"
#include "compiler_c/log/logger.h"
#include "compiler_c/tokenize/tokenizer.h"

#include <inttypes.h>

#include <inttypes.h>
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
        log_start(LOG_ERROR);
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
        PANIC("IR Given unknown token as unary operator\n");
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
        return BW_AND;
    case TK_OR:
        return BW_OR;
    case TK_XOR:
        return XOR;
    case TK_SHL:
        return SHL;
    case TK_SHR:
        return SHR;
    case TK_AND_AND:
        return L_AND;
    case TK_OR_OR:
        return L_OR;
    default:
        log_start(LOG_ERROR);
        printf("Given an unsupported token to convert to IR Binary op: ");
        print_token_type(type);
        printf("\n");
        exit(1);
    }
}
static void print_unary_op(const IR_UNARY_OP op) {
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

static void print_binary_op(const IR_BINOP_OP op) {
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
    case BW_AND:
        printf("AND");
        break;
    case BW_OR:
        printf("BW_OR");
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
    case L_AND:
        printf("L_AND");
        break;
    case L_OR:
        printf("L_OR");
        break;
    }
}
static void print_cmp_op(const IR_CMP_OP op) {
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

static char ir_type_suffix(Type *type) {
    switch (type->kind) {
    case T_INT:
        return type->is_signed ? 'i' : 'u';
    case T_FLOAT:
        return 'f';
    case T_ARRAY:
        return 'a';
    case T_POINTER:
        return 'p';
    case T_STRUCT:
        return 's';
    case T_VOID:
        return 'v';
    case T_INVALID:
        return '#';
    case T_ENUM:
        return 'e';
    // PANIC("Tried to print invalid type\n");
    default:
        log_start(LOG_ERROR);
        printf("Not handling this type ir_type_suffix: ");
        print_type(type);
        printf("\n");
        exit(1);
    }
}

static void print_ir_const(const IR_Context *ctx, const IR_Instruction *instr) {
    printf("    ");
    print_ir_value(&instr->ops[0]);
    printf(" = CONST ");
    print_ir_value(&instr->ops[1]);
    if (ctx) {
        IR_Literal *c = NULL;
        if (instr->ops[1].kind == IR_CONSTANT) {
            c = get_const(ctx, instr->ops[1].const_index);
        } else if (instr->ops[1].kind == IR_PHYS_REG && instr->ops[1].phys_reg.data_kind == REG_DATA_CONST_INDEX) {
            c = get_const(ctx, instr->ops[1].phys_reg.const_index);
        } else PANIC("IR_CONST with non const IRVALUE\n");
        printf(", ");
        switch (instr->_const.type->kind) {
        case T_INT:
            if (instr->_const.type->size == 8) printf("%" PRId64, c->i);
            else if (instr->_const.type->size == 1) printf("%c", (char)c->i);
            else printf("%d", (int)c->i);
            break;
        case T_FLOAT:
            printf("%g", c->f);
            break;
        case T_ARRAY:
            if (instr->_const.type->base == type_i8) {
                printf("\"%s\"", c->s.data);
                break;
            }
        default:
            PANIC("Tried to print IR_CONST of unknown type\n");
        }
    }
    printf("\n");
}

static void print_ir_binop(const IR_Instruction *instr) {
    printf("    ");
    print_ir_value(&instr->ops[0]);
    printf(" = BINOP:%c%d ", ir_type_suffix(instr->binop.type), instr->binop.type->size * 8);
    print_ir_value(&instr->ops[1]);
    printf(", ");
    print_ir_value(&instr->ops[2]);
    printf(", ");
    print_binary_op(instr->binop.op);
    printf("\n");
}

static void print_ir_load(const IR_Instruction *instr) {
    printf("    ");
    print_ir_value(&instr->ops[0]);
    printf(" = LOAD:%c%d ", ir_type_suffix(instr->load.type), instr->load.type->size * 8);
    print_ir_value(&instr->ops[1]);
    printf("\n");
}

static void print_ir_store(const IR_Instruction *instr) {
    printf("    ");
    printf("STORE:%c%d ", ir_type_suffix(instr->store.type), instr->store.type->size * 8);
    print_ir_value(&instr->ops[1]);
    printf(" -> ");
    print_ir_value(&instr->ops[0]);
    printf("\n");
}

static void print_ir_ret(const IR_Instruction *instr) {
    printf("    RET:%c%d ", ir_type_suffix(instr->ret.type), instr->ret.type->size * 8);
    if (instr->ret.type != type_void) print_ir_value(&instr->ops[0]);
    printf("\n");
}

static void print_ir_call(const IR_Instruction *instr) {
    printf("    ");
    if (instr->call.type->_func.return_type != type_void) {
        print_ir_value(&instr->ops[0]);
        printf(" = ");
    }
    printf("CALL:%c%d ", ir_type_suffix(instr->call.type->_func.return_type), instr->call.type->_func.return_type->size * 8);
    print_ir_value(&instr->ops[1]);
    printf(", %d:[ ", instr->call.arg_array.count);
    for (int i = 0; i < instr->call.arg_array.count; i++) {
        IR_CallArg *arg = get_call_arg(instr, i);
        print_ir_value(&arg->v);

        if (i < instr->call.arg_array.count - 1) {
            printf(", ");
        }
    }
    printf(" ]\n");
}
static void print_ir_br_cond(const IR_Instruction *instr) {
    printf("    BR_COND ");
    print_ir_value(&instr->ops[0]);
    if (instr->br_cond.t_block) printf(" L%d ", instr->br_cond.t_block->id);
    else printf(" # ");
    if (instr->br_cond.f_block) printf("L%d", instr->br_cond.f_block->id);
    else printf("#");
    printf("\n");
}

static void print_ir_cmp(const IR_Instruction *instr) {
    printf("    ");
    print_ir_value(&instr->ops[0]);
    printf(" = CMP:%c%d ", ir_type_suffix(instr->cmp.type), instr->cmp.type->size * 8);
    print_ir_value(&instr->ops[1]);
    printf(", ");
    print_ir_value(&instr->ops[2]);
    printf(" ");
    print_cmp_op(instr->cmp.op);
    printf("\n");
}

static void print_ir_unop(const IR_Instruction *instr) {
    printf("    ");
    print_ir_value(&instr->ops[0]);
    printf(" = UNARY ");
    print_ir_value(&instr->ops[1]);
    printf(" ");
    print_unary_op(instr->unary.op);
    printf("\n");
}

static void print_ir_cast(const IR_Instruction *instr) {
    printf("    ");
    print_ir_value(&instr->ops[0]);
    printf(" = CAST ");
    print_type(instr->cast.from);
    printf(" to ");
    print_type(instr->cast.to);
    printf(", ");
    print_ir_value(&instr->ops[1]);
    printf("\n");
}

static void print_ir_addr(const IR_Instruction *instr) {
    printf("    ");
    print_ir_value(&instr->ops[0]);
    printf(" = ADDR ");
    print_ir_value(&instr->ops[1]);
    if (instr->addr.offset > 0) {
        printf(", +%d", instr->addr.offset);
    }
    printf("\n");
}

static void print_ir_alloca(const IR_Instruction *instr) {
    printf("    ");
    print_ir_value(&instr->ops[0]);
    printf(" = ALLOCA %d\n", instr->alloca.size);
}

static void print_ir_memcpy(const IR_Instruction *instr) {
    printf("    MEMCPY ");
    print_ir_value(&instr->ops[1]);
    printf(" -> ");
    print_ir_value(&instr->ops[0]);
    printf(", %d\n", instr->memcpy.size);
}

static void print_ir_param(const IR_Instruction *instr) {
    printf("    ");
    print_ir_value(&instr->ops[0]);
    printf(" = PARAM:%c%d ", ir_type_suffix(instr->param.type), instr->param.type->size * 8);
    if (instr->op_count == 2) print_ir_value(&instr->ops[1]);
    printf(" p%d\n", instr->param.param_index);
}
static void print_ir_builtin_va_start(const IR_Instruction *instr) {
    printf("    ");
    print_ir_value(&instr->ops[0]);
    printf(" = BUILTIN_VA_START ");
    print_ir_value(&instr->ops[1]);
    printf("\n");
}
static void print_ir_builtin_va_arg(const IR_Instruction *instr) {
    printf("    BUILTIN_VA_ARG ");
    print_ir_value(&instr->ops[0]);
    printf(", ");
    print_type(instr->builtin_va_arg.type);
    printf("\n");
}
void print_ir_instruction(const IR_Context *ctx, const IR_Instruction *instr) {
    switch (instr->op) {
    case IR_CONST:
        print_ir_const(ctx, instr);
        break;
    case IR_BINOP:
        print_ir_binop(instr);
        break;
    case IR_LOAD:
        print_ir_load(instr);
        break;
    case IR_STORE:
        print_ir_store(instr);
        break;
    case IR_RET:
        print_ir_ret(instr);
        break;
    case IR_CALL:
        print_ir_call(instr);
        break;
    case IR_BR:
        printf("    BR %d\n", instr->br.block->id);
        break;
    case IR_BR_COND:
        print_ir_br_cond(instr);
        break;
    case IR_CMP:
        print_ir_cmp(instr);
        break;
    case IR_UNOP:
        print_ir_unop(instr);
        break;
    case IR_CAST:
        print_ir_cast(instr);
        break;
    case IR_ADDR:
        print_ir_addr(instr);
        break;
    case IR_ALLOCA:
        print_ir_alloca(instr);
        break;
    case IR_MEMCPY:
        print_ir_memcpy(instr);
        break;
    case IR_LABEL:
        printf("%s:\n", instr->label.name);
        break;
    case IR_JMP:
        printf("    JMP %s\n", instr->jmp.name);
        break;
    case IR_BUILTIN_VA_START:
        print_ir_builtin_va_start(instr);
        break;
    case IR_BUILTIN_VA_ARG:
        print_ir_builtin_va_arg(instr);
        break;
    case IR_PARAM:
        print_ir_param(instr);
        break;
    }
}

static void print_ir_block(const IR_Context *ctx, const IR_Block *block) {
    for (int i = 0; i < block->instruction_array.count; i++) {
        print_ir_instruction(ctx, get_instruction(&block->instruction_array, i));
    }
}

void print_ir_function(const IR_Context *ctx, const IR_Function *func) {
    printf("%s: [max_reg: %d]\n ", func->name, func->max_reg);
    for (int i = 0; i < func->blocks_array.count; i++) {
        printf("L%d:\n", i);
        print_ir_block(ctx, get_block(func, i));
    }
}

void print_ir_module(const IR_Context *ctx, const IR_Module *module) {
    for (int i = 0; i < module->functions_array.count; i++) {
        print_ir_function(ctx, get_func(module, i));
    }
}

void print_ir_value(const IR_Value *v) {
    switch (v->kind) {
    case IR_SYMBOL:
        printf("%%%s", v->symbol->name);
        break;
    case IR_VREG:
        printf("r%d", v->vreg);
        break;
    case IR_CONSTANT:
        printf(".LC%d", v->const_index);
        break;
    case IR_PHYS_REG:
        print_ir_phys_reg(&v->phys_reg);
        break;
    case IR_UNDEFINED:
        printf("[#]");
        break;
    case IR_INT_LITERAL:
        printf("$%" PRId64, v->int_literal);
        break;
    }
}

void print_ir_phys_reg(const PhysReg *r) {
    switch (r->data_kind) {
    case REG_DATA_LABEL:
        printf("%s", r->label);
        break;
    case REG_DATA_OFFSET:
        // ASSERT(!r->scale, "Not lowering scale for physical registers yet\n");
        printf("%d", r->offset);
        break;
    case REG_DATA_CONST_INDEX:
        printf(".LC%d", r->const_index);
        break;
    case REG_DATA_NONE:
        break;
    }
    if (r->data_kind != REG_DATA_NONE) printf("(");
    switch (r->kind) {
    case REG_GP:
        printf("%s", gp_register_str[r->gp_reg][r->size]);
        break;
    case REG_XMM:
        printf("%s", sse_register_str[r->sse_reg]);
        break;
    case REG_IP:
        printf("%%rip");
        break;
    }
    if (r->data_kind != REG_DATA_NONE) printf(")");
}
