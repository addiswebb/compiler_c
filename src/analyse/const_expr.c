#include "compiler_c/core/node.h"
#include "compiler_c/core/type.h"
#include "compiler_c/log/logger.h"
#include "compiler_c/parse/parser.h"
#include "compiler_c/tokenize/tokenizer.h"
#include <compiler_c/analyse/const_expr.h>
#include <inttypes.h>

ConstLiteral evaluate_const_unary(const Node *node) {
    ConstLiteral e = evaluate_const_expression(node->unary.expr);
    e.type = node->type;
    if (node->unary.op == TK_AND) return e;
    switch (node->type->kind) {
    case T_INT:
        e.kind = CONST_INTEGER;
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
            break;
        case TK_SIZEOF:
            e.i = node->unary.expr->type->size;
            break;
        default:
            PANIC("Invalid unary op for integer const expr\n");
        }
        break;
    case T_FLOAT:
        e.kind = CONST_FLOAT;
        switch (node->unary.op) {
        case TK_PLUS:
            break;
        case TK_MINUS:
            e.f = -e.f;
            break;
        case TK_SIZEOF:
            PANIC("MAybe should be uncreachable\n");
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
    ConstLiteral e = {.type = node->type};
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
    case T_POINTER:
        if (lhs.kind == CONST_REFERENCE && rhs.kind == CONST_REFERENCE) PANIC("Cannot add two const expr pointers\n");
        ConstLiteral *ptr = lhs.kind == CONST_REFERENCE ? &lhs : &rhs;
        ConstLiteral *d = lhs.kind != CONST_REFERENCE ? &lhs : &rhs;
        e = *ptr;
        ASSERT(d->kind == CONST_INTEGER, "Pointer addition must be with an integer\n");
        e.ref.offset += d->i * ptr->type->base->size;
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
        if (node->cast.from->kind == T_INT) break;
        e.i = (int)e.f;
        e.kind = CONST_INTEGER;
        break;
    case T_FLOAT:
        e.f = (int)e.i;
        e.kind = CONST_FLOAT;
        break;
    case T_POINTER:
        if (node->cast.from->kind == T_ARRAY && node->cast.from->base == type_i8) break;
    default:
        log_start(LOG_ERROR);
        printf("Unsupported const expr cast from ");
        print_type(node->cast.from);
        printf(" to ");
        print_type(node->type);
        printf("\n");
    }
    return e;
}
ConstLiteral evaluate_const_init_list(const Node *node) {
    ASSERT(node->kind == N_INIT_LIST, "Expected N_INIT_LIST node.\n");
    ConstLiteral l = {.type = node->type, .kind = CONST_ARRAY};
    array_init(&l.arr, node->type->_array.array_len, sizeof(ConstLiteral));
    // Fill with zeros
    for (int i = 0; i < l.arr.capacity; i++) append(&l.arr, &(ConstLiteral){.type = l.type->base, .kind = CONST_INTEGER, .i = 0});
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
        // Enums are not decayed to integer until after sema, so we must allow them here
    case T_ENUM:
        l.i = node->literal.i;
        l.kind = CONST_INTEGER;
        break;
    case T_FLOAT:
        l.f = node->literal.f;
        l.kind = CONST_FLOAT;
        break;
    case T_POINTER:
        DEBUG("Evaluate const literal from pointer to string\n");
    case T_ARRAY:
        if (l.type->base == type_i8) {
            l.kind = CONST_STRING;
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
        return (ConstLiteral){.type = type_u64, .kind = CONST_INTEGER, .i = node->type->size};
    case N_INIT_LIST:
        return evaluate_const_init_list(node);
    case N_IDENTIFIER:
        return (ConstLiteral){.ref = {.symbol = node->identifier.symbol}, .kind = CONST_REFERENCE};
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
    ASSERT(l, "Recieved null const literal to print\n");
    switch (l->kind) {
    case CONST_INTEGER:
        printf("%ld", l->i);
        break;
    case CONST_FLOAT:
        printf("%lf", l->f);
        break;
    case CONST_STRING:
        printf("\"%.*s\"", l->s.len, l->s.data);
        break;
    case CONST_ARRAY:
        printf("{");
        for (int i = 0; i < l->arr.count; i++) {
            ConstLiteral *e = get(&l->arr, i);
            print_const_literal(e);
            if (i < l->arr.count - 1) printf(", ");
        }
        printf("}");
        break;
    case CONST_LABEL:
        printf(".LC%d", l->const_index);
        break;
    case CONST_REFERENCE:
        printf("&%s", l->ref.symbol->name);
        if (l->ref.offset) printf(" + %d", l->ref.offset);
        break;
    }
}
