#include "compiler_c/ir/ir_util.h"
#include "compiler_c/core/type.h"
#include "compiler_c/ir/ir_module.h"
#include "compiler_c/tokenize/tokenizer.h"
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
        printf("IR Given unknown token as unary operator\n");
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
    case T_INVALID:
        printf("Tried to print invalid type\n");
        exit(1);
    case T_INT:
        switch (type->size) {
        case 1:
            return 'c';
        case 2:
            return 's';
        case 4:
            return 'i';
        case 8:
            return 'l';
        default:
            printf("Given invalid size of INT\n");
            exit(1);
        }
    case T_FLOAT:
        switch (type->size) {
        case 4:
            return 'f';
        case 8:
            return 'd';
        default:
            printf("Given invalid size of INT\n");
            exit(1);
        }
    case T_ARRAY:
        return 'a';
    case T_POINTER:
        return 'p';
    case T_STRUCT:
        return 's';
    default:
        printf("Not handling this type ir_type_suffix: ");
        print_type(type);
        printf("\n");
        exit(1);
    }
}

static void print_ir_const(const IR_Context *ctx, const IR_Instruction *instr) {
    const IR_Literal *c = get_const(ctx, instr->ops[1].const_index);
    printf("    ");
    print_ir_value(&instr->ops[0]);
    printf(" = CONST ");
    print_ir_value(&instr->ops[1]);
    printf(", ");
    switch (instr->_const.type->kind) {
    case T_INT:
        if (instr->_const.type->size == 8) printf("%lld", c->i);
        else if (instr->_const.type->size == 1) printf("%c", (char)c->i);
        else printf("%d", (int)c->i);
        break;
    case T_FLOAT:
        printf("%g", c->f);
        break;
    case T_ARRAY:
        if (instr->_const.type->base == type_char) {
            printf("\"%s\"", c->s.data);
            break;
        }
    default:
        printf("Tried to print IR_CONST of unknown type\n");
        exit(1);
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

static void print_ir_load(IR_Context *ctx, const IR_Instruction *instr) {
    printf("    ");
    print_ir_value(&instr->ops[0]);
    printf(" = LOAD:%c%d ", ir_type_suffix(instr->load.type), instr->load.type->size * 8);
    print_ir_value(&instr->ops[1]);
    printf("\n");
}

static void print_ir_store_mem(IR_Context *ctx, const IR_Instruction *instr) {
    printf("    ");
    printf("STORE_MEM:%c%d ", ir_type_suffix(instr->store.type), instr->store.type->size * 8);
    print_ir_value(&instr->ops[1]);
    printf(" -> ");
    print_ir_value(&instr->ops[0]);
    printf("\n");
}
static void print_ir_store(IR_Context *ctx, const IR_Instruction *instr) {
    printf("    ");
    printf("STORE:%c%d ", ir_type_suffix(instr->store.type), instr->store.type->size * 8);
    print_ir_value(&instr->ops[1]);
    printf(" -> ");
    print_ir_value(&instr->ops[0]);
    printf("\n");
}

static void print_ir_ret(IR_Context *ctx, const IR_Instruction *instr) {
    printf("    RET ");
    print_ir_value(&instr->ops[0]);
    printf("\n");
}

static void print_ir_call(IR_Context *ctx, const IR_Instruction *instr) {
    printf("    ");
    print_ir_value(&instr->ops[0]);
    printf(" = CALL:%c%d '%s', %d:[ ", ir_type_suffix(instr->call.type), instr->call.type->size * 8, instr->call.callee->name,
           instr->call.arg_count);
    for (int i = 0; i < instr->call.arg_count; i++) {
        print_type(instr->call.args[i].type);
        printf("=");
        print_ir_value(&instr->call.args[i].reg);

        if (i < instr->call.arg_count - 1) {
            printf(", ");
        }
    }
    printf(" ]\n");
}
static void print_ir_br_cond(IR_Context *ctx, const IR_Instruction *instr) {
    printf("    BR_COND ");
    print_ir_value(&instr->ops[0]);
    if (instr->br_cond.t_block) printf(" L%d ", instr->br_cond.t_block->id);
    else printf(" # ");
    if (instr->br_cond.f_block) printf("L%d", instr->br_cond.f_block->id);
    else printf("#");
    printf("\n");
}

static void print_ir_cmp(IR_Context *ctx, const IR_Instruction *instr) {
    printf("    ");
    print_ir_value(&instr->ops[0]);
    printf(" = CMP ");
    print_ir_value(&instr->ops[1]);
    printf(" , ");
    print_ir_value(&instr->ops[2]);
    printf(" ");
    print_cmp_op(instr->cmp.op);
    printf("\n");
}

static void print_ir_unop(IR_Context *ctx, const IR_Instruction *instr) {
    printf("    ");
    print_ir_value(&instr->ops[0]);
    printf(" = UNARY ");
    print_ir_value(&instr->ops[1]);
    printf(" ");
    print_unary_op(instr->unary.op);
    printf("\n");
}

static void print_ir_cast(IR_Context *ctx, const IR_Instruction *instr) {
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

static void print_ir_addr(IR_Context *ctx, const IR_Instruction *instr) {
    printf("    ");
    print_ir_value(&instr->ops[0]);
    printf(" = ADDR ");
    print_ir_value(&instr->ops[1]);
    if (instr->addr.offset > 0) {
        printf(", +%d", instr->addr.offset);
    }
    printf("\n");
}

static void print_ir_alloca(IR_Context *ctx, const IR_Instruction *instr) {
    printf("    ");
    print_ir_value(&instr->ops[0]);
    printf(" = ALLOCA %d\n", instr->alloca.size);
}

static void print_ir_memcpy(IR_Context *ctx, const IR_Instruction *instr) {
    printf("    MEMCPY ");
    print_ir_value(&instr->ops[1]);
    printf(" -> ");
    print_ir_value(&instr->ops[0]);
    printf(", %d\n", instr->memcpy.size);
}
void print_ir_instruction(IR_Context *ctx, const IR_Instruction *instr) {
    switch (instr->op) {
    case IR_CONST:
        print_ir_const(ctx, instr);
        break;
    case IR_BINOP:
        print_ir_binop(instr);
        break;
    case IR_LOAD:
        print_ir_load(ctx, instr);
        break;
    case IR_STORE:
        print_ir_store(ctx, instr);
        break;
    case IR_RET:
        print_ir_ret(ctx, instr);
        break;
    case IR_CALL:
        print_ir_call(ctx, instr);
        break;
    case IR_BR:
        printf("    BR %d\n", instr->br.block->id);
        break;
    case IR_BR_COND:
        print_ir_br_cond(ctx, instr);
        break;
    case IR_CMP:
        print_ir_cmp(ctx, instr);
        break;
    case IR_UNOP:
        print_ir_unop(ctx, instr);
        break;
    case IR_CAST:
        print_ir_cast(ctx, instr);
        break;
    case IR_ADDR:
        print_ir_addr(ctx, instr);
        break;
    case IR_ALLOCA:
        print_ir_alloca(ctx, instr);
        break;
    case IR_MEMCPY:
        print_ir_memcpy(ctx, instr);
        break;
    case IR_STORE_MEM:
        print_ir_store_mem(ctx, instr);
        break;
    }
}

static void print_ir_block(IR_Context *ctx, const IR_Block *block) {
    for (int i = 0; i < block->instruction_array.count; i++) {
        print_ir_instruction(ctx, get_instruction(&block->instruction_array, i));
    }
}

static void print_ir_function(IR_Context *ctx, const IR_Function *func) {
    printf("%s: [max_reg: %d]\n ", func->name, func->max_reg);
    for (int i = 0; i < func->blocks_array.count; i++) {
        printf("L%d:\n", i);
        print_ir_block(ctx, get_block(func, i));
    }
}

void print_ir_module(IR_Context *ctx, const IR_Module *module) {
    for (int i = 0; i < module->functions_array.count; i++) {
        print_ir_function(ctx, get_func(module, i));
    }
}

void print_ir_value(const IR_Value *v) {
    switch (v->kind) {
    case IR_VREG:
        if (v->reg >= 0) printf("r%d", v->reg);
        else printf("p%d", -v->reg);
        break;
    case IR_MEM:
        printf("v%d", v->mem);
        if (v->offset > 0) printf(":%d", v->offset);
        break;
    case IR_STACK:
        printf("[%d]", v->stack_offset);
        break;
    case IR_LITERAL:
        printf(".LC%d", v->const_index);
        break;
    case IR_UNDEFINED:
        break;
    case IR_GLOBAL:
        printf("g[%s]", v->global->name);
        break;
    case IR_PHYS_REG:
        if (v->phys_reg.kind == REG_GP) {
            printf("%s", gp_register_str[v->phys_reg.gp_reg][v->phys_reg.size]);
        } else {
            printf("%s", xmm_register_str[v->phys_reg.gp_reg]);
        }
        break;
    }
}
