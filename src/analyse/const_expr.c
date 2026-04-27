#include "compiler_c/core/node.h"
#include "compiler_c/core/type.h"
#include "compiler_c/log/logger.h"
#include "compiler_c/tokenize/tokenizer.h"
#include <compiler_c/analyse/const_expr.h>
#include <inttypes.h>

ConstLiteral evaluate_const_unary(const Node *node) {
    ConstLiteral e = evaluate_const_expression(node->unary.expr);
    e.type = node->type;
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
ConstLiteral evaluate_const_binary(const Node *node) {
    ConstLiteral lhs = evaluate_const_expression(node->binary.lhs);
    ConstLiteral rhs = evaluate_const_expression(node->binary.rhs);
    ConstLiteral e = {};
    e.type = node->type;
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
ConstLiteral evaluate_const_cast(const Node *node) {
    ConstLiteral e = evaluate_const_expression(node->cast.expr);
    e.type = node->type;
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
ConstLiteral evaluate_const_init_list(const Node *node) {
    ASSERT(node->kind == N_INIT_LIST, "Expected N_INIT_LIST node.\n");
    ConstLiteral l = {.kind = CONST_INIT_LIST};
    l.type = node->type;
    array_init(&l.arr, node->type->_array.array_len, sizeof(ConstLiteral));
    for (int i = 0; i < l.arr.capacity; i++) append(&l.arr, &(ConstLiteral){.type = l.type->base, .i = 0});

    // TODO dont forget to free this shi
    ConstLiteral *arr = l.arr.data;
    for (int i = 0; i < node->init_list.elements_array.count; i++) {
        Node *designated_initializer = get_node(&node->init_list.elements_array, i);
        ConstLiteral e = evaluate_const_expression(designated_initializer->designated_init.value);
        set(&l.arr, &e, designated_initializer->designated_init._array.index);
    }
    return l;
}

ConstLiteral evaluate_const_literal(const Node *node) {
    ASSERT(node->kind == N_LITERAL, "Expected a N_LITERAL node.\n");
    ConstLiteral l = {};
    l.type = node->type;
    switch (node->type->kind) {
    case T_INT:
    case T_ENUM:
        // Enums are not decayed to integer until after sema, so we must allow them here
        l.i = node->literal.i;
        break;
    case T_FLOAT:
        l.f = node->literal.f;
        break;
    case T_POINTER:
    case T_ARRAY:
        if (l.type->base == type_i8) {
            l.s.data = node->literal.s.data;
            l.s.len = node->literal.s.len;
            break;
        }
    default:
        log_start(LOG_ERROR);
        printf("Tried to convert literal with an invalid type ");
        print_type(node->type);
        printf(" to ConstExpr.\n");
        exit(1);
    }
    return l;
}

ConstLiteral evaluate_const_expression(const Node *node) {
    switch (node->kind) {
    case N_UNARY:
        return evaluate_const_unary(node);
    case N_BINARY:
        return evaluate_const_binary(node);
    case N_LITERAL:
        return evaluate_const_literal(node);
    case N_CAST:
        return evaluate_const_cast(node);
    case N_TYPE:
        return (ConstLiteral){.type = type_u64, .i = node->type->size, .kind = CONST_INTEGER};
    case N_INIT_LIST:
        return evaluate_const_init_list(node);
    case N_INDEX:
        PANIC("Indexing not handled in const expr yet\n");
    case N_MEMBER_ACCESS:
        PANIC("member access in const expr yet\n");
    default:
        PANIC("Initializer element is not a compile-time constant.\n");
        break;
    }
}

void print_const_literal(const ConstLiteral *l) {
    if (l == NULL) {
        printf("[#]");
        return;
    };

    switch (l->kind) {
    case CONST_INTEGER:
        printf("%ld", l->i);
        break;
    case CONST_FLOAT:
        printf("%lf", l->f);
        break;
    case CONST_INIT_LIST:
        printf("{");
        for (int i = 0; i < l->arr.count; i++) {
            ConstLiteral *e = get(&l->arr, i);
            print_const_literal(e);
            if (i < l->arr.count - 1) printf(", ");
        }
        printf("}");
        break;
    case CONST_STRING:
        printf("\"%.*s\"", l->s.len, l->s.data);
        break;
    }
}
