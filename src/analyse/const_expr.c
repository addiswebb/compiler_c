#include "compiler_c/core/type.h"
#include "compiler_c/log/logger.h"
#include "compiler_c/tokenize/tokenizer.h"
#include <compiler_c/analyse/const_expr.h>
#include <inttypes.h>

ConstExpr evaluate_const_unary(Node *node) {
    ConstExpr e = evaluate_const_expression(node->unary.expr);
    switch (node->type->kind) {
    case T_INT:
        switch (node->unary.op) {
        case TK_PLUS:
            break;
        case TK_MINUS:
            e.i = -e.i;
            break;
        case TK_L_NOT:
            e.i = !e.i;
            break;
        case TK_BW_NOT:
            e.i = ~e.i;
        case TK_AND:
            PANIC("Integer Const expr reference not handled yet\n");
        case TK_MULTIPLY:
            PANIC("Integer Const expr dereference not handled yet\n");
        case TK_SIZEOF:
            e.i = node->unary.expr->type->size;
            break;
        default:
            PANIC("Invalid unary op for integer const expr\n");
        }
        break;
    case T_FLOAT:
        switch (node->unary.op) {
        case TK_PLUS:
            break;
        case TK_MINUS:
            e.f = -e.f;
            break;
        case TK_AND:
            PANIC("Float Const expr reference not handled yet\n");
        case TK_MULTIPLY:
            PANIC("Float Const expr dereference not handled yet\n");
        case TK_SIZEOF:
            e.i = node->unary.expr->type->size;
            break;
        default:
            PANIC("Invalid unary op for float const expr\n");
        }
        break;
    default:
        log_start(LOG_ERROR);
        printf("Invalid const unary type");
        print_type(node->type);
        printf("\n");
    }
    return e;
}
ConstExpr evaluate_const_binary(Node *node) {
    ConstExpr lhs = evaluate_const_expression(node->binary.lhs);
    ConstExpr rhs = evaluate_const_expression(node->binary.rhs);
    ConstExpr e = {};
    switch (node->type->kind) {
    case T_INT:
        e.kind = CONST_INTEGER;
        switch (node->binary.op) {
        case TK_PLUS:
            e.i = lhs.i + rhs.i;
            break;
        case TK_MINUS:
            e.i = lhs.i - rhs.i;
            break;
        case TK_MULTIPLY:
            e.i = lhs.i * rhs.i;
            break;
        case TK_DIVIDE:
            e.i = lhs.i / rhs.i;
            break;
        case TK_XOR:
            e.i = lhs.i ^ rhs.i;
            break;
        case TK_AND_AND:
            e.i = lhs.i && rhs.i;
            break;
        case TK_EQ_EQ:
            e.i = lhs.i == rhs.i;
            break;
        case TK_GE:
            e.i = lhs.i >= rhs.i;
            break;
        case TK_GT:
            e.i = lhs.i > rhs.i;
            break;
        case TK_LE:
            e.i = lhs.i <= rhs.i;
            break;
        case TK_LT:
            e.i = lhs.i < rhs.i;
            break;
        case TK_OR_OR:
            e.i = lhs.i || rhs.i;
            break;
        case TK_NEQ:
            e.i = lhs.i != rhs.i;
            break;
        case TK_SHL:
            e.i = lhs.i << rhs.i;
            break;
        case TK_SHR:
            e.i = lhs.i >> rhs.i;
            break;
        case TK_OR:
            e.i = lhs.i | rhs.i;
            break;
        case TK_AND:
            e.i = lhs.i & rhs.i;
            break;
        case TK_MOD:
            e.i = lhs.i % rhs.i;
            break;
        default:
            PANIC("Invalid binary op for integer const expr\n");
        }
        break;
    case T_FLOAT:
        e.kind = CONST_FLOAT;
        switch (node->binary.op) {
        case TK_PLUS:
            e.f = lhs.f + rhs.f;
            break;
        case TK_MINUS:
            e.f = lhs.f - rhs.f;
            break;
        case TK_MULTIPLY:
            e.f = lhs.f * rhs.f;
            break;
        case TK_DIVIDE:
            e.f = lhs.f / rhs.f;
            break;
        case TK_EQ_EQ:
            e.f = lhs.f == rhs.f;
            break;
        case TK_GE:
            e.f = lhs.f >= rhs.f;
            break;
        case TK_GT:
            e.f = lhs.f > rhs.f;
            break;
        case TK_LE:
            e.f = lhs.f <= rhs.f;
            break;
        case TK_LT:
            e.f = lhs.f < rhs.f;
            break;
        case TK_NEQ:
            e.f = lhs.f != rhs.f;
            break;
        default:
            PANIC("Invalid binary op for float const expr\n");
        }
        break;
    default:
        log_start(LOG_ERROR);
        printf("Invalid const binary type");
        print_type(node->type);
        printf("\n");
    }
    return e;
}
ConstExpr evaluate_const_cast(Node *node) {
    ConstExpr e = evaluate_const_expression(node->cast.expr);
    switch (node->type->kind) {
    case T_INT:
        e.i = (int)e.f;
        break;
    case T_FLOAT:
        e.f = (int)e.i;
        break;
    default:
        log_start(LOG_ERROR);
        printf("Unsupported cast from ");
        print_type(node->cast.from);
        printf(" to ");
        print_type(node->cast.to);
        printf("\n");
    }
    return e;
}
ConstExpr evaluate_const_init_list(Node *node) { return (ConstExpr){}; }

ConstExpr literal_to_const(Node *node) {
    ConstExpr e = {};

    switch (node->type->kind) {
    case T_INT:
    // Enums are not decayed to integer until after sema, so we must allow them here
    case T_ENUM:
        e.kind = CONST_INTEGER;
        e.i = node->literal.i;
        break;
    case T_FLOAT:
        e.kind = CONST_FLOAT;
        e.f = node->literal.f;
        break;
    default:
        log_start(LOG_ERROR);
        printf("Tried to convert literal with an invalid type ");
        print_type(node->type);
        printf(" to ConstExpr.\n");
        exit(1);
    }
    return e;
}

ConstExpr evaluate_const_expression(Node *const_expr) {
    switch (const_expr->kind) {
    case N_UNARY:
        return evaluate_const_unary(const_expr);
    case N_BINARY:
        return evaluate_const_binary(const_expr);
    case N_LITERAL:
        return literal_to_const(const_expr);
    case N_CAST:
        return evaluate_const_cast(const_expr);
    case N_TYPE:
        return (ConstExpr){.i = const_expr->type->size, .kind = CONST_INTEGER};
    case N_INIT_LIST:
        ConstExpr l = {.kind = CONST_INIT_LIST};
        l.init_list.capacity = const_expr->type->_array.array_len;
        l.init_list.count = l.init_list.capacity;
        l.init_list.element_size = sizeof(ConstExpr);
        l.init_list.data = calloc(l.init_list.count, l.init_list.element_size);
        // TODO dont forget to free this shi
        ASSERT(l.init_list.data, "Failed to calloc for const expr init list\n");
        ConstExpr *arr = l.init_list.data;
        for (int i = 0; i < const_expr->init_list.elements_array.count; i++) {
            Node *designated_initializer = get_node(&const_expr->init_list.elements_array, i);
            arr[designated_initializer->designated_init._array.index] =
                evaluate_const_expression(designated_initializer->designated_init.value);
        }
        return l;
    case N_INDEX:
        PANIC("Indexing not handled in const expr yet\n");
    case N_MEMBER_ACCESS:
        PANIC("member access in const expr yet\n");
    default:
        PANIC("Initializer element is not a compile-time constant.\n");
        break;
    }
}

void print_const_expr(const ConstExpr *expr) {
    switch (expr->kind) {
    case CONST_INTEGER:
        printf("%ld", expr->i);
        break;
    case CONST_FLOAT:
        printf("%lf", expr->f);
        break;
    case CONST_INIT_LIST:
        printf("{");
        for (int i = 0; i < expr->init_list.count; i++) {
            ConstExpr *e = get(&expr->init_list, i);
            print_const_expr(e);
            if (i < expr->init_list.count - 1) printf(", ");
        }
        printf("}");
        break;
    }
}
