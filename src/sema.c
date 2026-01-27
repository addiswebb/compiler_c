#include "compiler_c/sema.h"
#include "compiler_c/node.h"
#include "compiler_c/parser.h"
#include "compiler_c/tokenizer.h"
#include "compiler_c/type.h"

#include <stdbool.h>
#include <string.h>

// Is this node assignable?
bool is_lvalue(Node *n) { return n->kind == N_IDENTIFIER; }

Type *token_to_type(TokenType t) {
    switch (t) {
    case TK_CHAR:
        return type_char;
    case TK_SHORT:
        return type_short;
    case TK_INT:
        return type_int;
    case TK_LONG:
        return type_long;
    case TK_FLOAT:
        return type_float;
    case TK_DOUBLE:
        return type_double;
    case TK_VOID:
        return type_void;
    default:
        return type_invalid;
    }
}

Type *check_unary_op(Node *unaryop) {
    Node *expr = unaryop->unary.expr;
    TypeKind kind = expr->type->kind;
    switch (unaryop->unary.op) {
    // [Int, Float] => [Int, Float]
    case TK_PLUS:
    case TK_MINUS:
        if (kind == T_INT || kind == T_FLOAT) return expr->type;
        break;
    // [Int, Float] => Int
    case TK_L_NOT:
        if (kind == T_INT || kind == T_FLOAT) return type_int;
        break;
    // Int => Int
    case TK_BW_NOT:
        if (kind == T_INT) return expr->type;
        break;
    case TK_AND:
        if (is_lvalue(expr)) {
            return get_pointer_type(expr->type);
        }
        printf("Tried to reference a non assignable term\n");
        exit(1);
    case TK_MULTIPLY:
        if (expr->type->base != type_invalid) return expr->type->base;
        printf("Tried to derefence some nonexistent term\n");
        exit(1);
    case TK_SIZEOF:
        if (expr->type != type_invalid && expr->type->size) return type_int;
        printf("Tried to get the sizeof something without a size\n");
        exit(1);
    case TK_INCR:
    case TK_DECR:
        return expr->type;
    default:
        break;
    }
    printf("Invalid operand type for the given unary operator\n");
    return type_invalid;
}

Type *check_binary_op(NodeManager *nm, TokenType op, Node *binop) {
    if (binop->binary.lhs->type == type_invalid || binop->binary.rhs->type == type_invalid) {
        printf("Semantic Analysis: Binary op was given expression with an invalid type\n");
        exit(1);
    }
    Node *lhs = binop->binary.lhs;
    Node *rhs = binop->binary.rhs;
    if (is_assignment_op(op)) {
        if (!is_lvalue(lhs)) {
            printf("Semantic Analysis: Binary op lhs is not assignable\n");
            exit(1);
        }
        TokenType underlying = get_underlying_op(op);
        if (is_arithmetic_op(underlying) || is_bitwise_op(underlying)) {
            Type *common = promote_binary_operands(nm, binop);
        }

        if (lhs->type->kind != rhs->type->kind) {
            binop->binary.rhs = cast_node(nm, rhs, lhs->type);
        }
        return lhs->type;
    }

    Type *common = promote_binary_operands(nm, binop);
    if (!common) {
        printf("Invalid arithmetic operands");
        exit(1);
    }

    if (is_arithmetic_op(op)) return common;
    if (is_comparison_op(op)) return type_int;

    if (is_bitwise_op(op)) {
        if (lhs->type != type_int || rhs->type != type_int) {
            printf("Bitwise operation requires integers\n");
            exit(1);
        }
        return type_int;
    }

    if (is_logical_op(op)) return type_int;

    printf("Unknown binary operator\n");
    return type_invalid;
}

Type *promote_binary_operands(NodeManager *nm, Node *binop) {
    Type *common;
    Node *lhs = binop->binary.lhs;
    Node *rhs = binop->binary.rhs;
    if (lhs->type == rhs->type) return lhs->type;
    if (lhs->type->kind == T_FLOAT || rhs->type->kind == T_FLOAT) {
        common = type_float;
    } else {
        common = type_int;
    }

    if (lhs->type != common) binop->binary.lhs = cast_node(nm, lhs, common);
    if (rhs->type != common) binop->binary.rhs = cast_node(nm, rhs, common);
    return common;
}
void semantic_analysis(Parser *p, NodeManager *nm, Node *node, Node *loop) {
    if (!node) return;
    switch (node->kind) {
    case N_TRANSLATION_UNIT:
        for (int i = 0; i < node->translation_unit.count; i++) {
            semantic_analysis(p, nm, node->translation_unit.declarations[i], loop);
        }
        node->type = type_void;
        break;
    case N_FUNCTION:
        for (int i = 0; i < node->func.param_count; i++) {
            p_append_var_decl(p, node->func.params[i]);
        }
        semantic_analysis(p, nm, node->func.body, loop);
        p->var_decl_count -= node->func.param_count;
        break;
    case N_COMPOUND:
        for (int i = 0; i < node->compound.count; i++) {
            semantic_analysis(p, nm, node->compound.items[i], loop);
        }
        break;
    case N_VAR_DECL:
        if (!node->var_decl.expr) break;
        semantic_analysis(p, nm, node->var_decl.expr, loop);
        if (node->var_decl.expr->type != node->type) {
            node->var_decl.expr = cast_node(nm, node->var_decl.expr, node->type);
        }
        break;
    case N_UNARY:
        semantic_analysis(p, nm, node->unary.expr, loop);
        node->type = check_unary_op(node);
        break;
    case N_BINARY:
        semantic_analysis(p, nm, node->binary.lhs, loop);
        semantic_analysis(p, nm, node->binary.rhs, loop);
        node->type = check_binary_op(nm, node->binary.op, node);
        break;
    case N_CAST:
        semantic_analysis(p, nm, node->cast.expr, loop);
        if (is_valid_cast(node->cast.expr->type, node->cast.to)) {
            node->cast.from = node->cast.expr->type;
            node->type = node->cast.to;
            break;
        }
        printf("Invalid cast from ");
        print_type(node->cast.expr->type);
        printf(" to ");
        print_type(node->cast.to);
        printf("\n");
        exit(1);
    case N_FUNCTION_CALL:
        Node *func_def = p_get_func_def(p, node->func_call.identifier->identifier.name);
        if (func_def->func.param_count != node->func_call.param_count) {
            printf("Argument count mismatch: %s expects %d found %d\n", func_def->func.name, func_def->func.param_count,
                   node->func_call.param_count);
            exit(1);
        }
        node->type = func_def->type;
        for (int i = 0; i < func_def->func.param_count; i++) {
            semantic_analysis(p, nm, node->func_call.params[i], loop);
            if (func_def->func.params[i]->type != node->func_call.params[i]->type) {
                node->func_call.params[i] = cast_node(nm, node->func_call.params[i], func_def->func.params[i]->type);
            }
        }
        break;
    case N_IDENTIFIER:
        node->type = p_get_var_decl(p, node->identifier.name)->type;
        break;
    case N_IF:
        semantic_analysis(p, nm, node->_if.cond, loop);
        if (node->_if.cond->type != type_int) {
            node->_if.cond = cast_node(nm, node->_if.cond, type_int);
        }
        semantic_analysis(p, nm, node->_if.if_true, loop);
        semantic_analysis(p, nm, node->_if.if_false, loop);
        break;
    case N_WHILE:
        semantic_analysis(p, nm, node->_while.cond, node);
        if (node->_while.cond->type != type_int) {
            node->_while.cond = cast_node(nm, node->_while.cond, type_int);
        }
        semantic_analysis(p, nm, node->_while.block, node);
        break;
    case N_FOR:
        semantic_analysis(p, nm, node->_for.init, node);
        semantic_analysis(p, nm, node->_for.cond, node);
        if (node->_for.cond->type != type_int) {
            node->_for.cond = cast_node(nm, node->_for.cond, type_int);
        }
        semantic_analysis(p, nm, node->_for.iter, node);
        semantic_analysis(p, nm, node->_for.block, node);
        break;
    case N_RETURN:
        semantic_analysis(p, nm, node->_return.expr, loop);
        if (node->_return.expr->type != type_int) {
            node->_return.expr = cast_node(nm, node->_return.expr, type_int);
        }
        break;
    case N_LITERAL:
        char *data = malloc(node->literal.len + 1);
        if (!data) {
            printf("Failed to allocate for sema literal analysis\n");
            exit(1);
        }
        memcpy(data, node->literal.raw_rata, node->literal.len);
        data[node->literal.len] = '\0';
        switch (node->literal.kind) {
        case L_INT:
            node->type = type_int;
            node->literal.i = atoi(data);
            free(data);
            break;
        case L_FLOAT:
            bool is_float = node->literal.raw_rata[node->literal.len - 1] == 'f';
            node->type = is_float ? type_float : type_double;
            node->literal.f = atof(data);
            free(data);
            break;
        case L_CHAR:
            node->type = type_char;
            node->literal.c = node->literal.raw_rata[0];
            if (node->literal.len != 1) {
                printf("Expected char len of 1, found %d\n", node->literal.len);
                exit(1);
            }
            free(data);
            break;
        case L_STRING:
            node->type = get_array_type(type_char, node->literal.len + 1);
            node->literal.s.data = data;
            node->literal.s.len = node->literal.len + 1;
            break;
        }
        break;
    case N_INDEX:
        semantic_analysis(p, nm, node->index.index, loop);
        semantic_analysis(p, nm, node->index.identifier, loop);
        if (node->index.index->type != type_long) {
            node->index.index = cast_node(nm, node->index.index, type_long);
        }
        node->type = node->index.identifier->type->base;
        break;
    case N_TYPE:
        break;
    case N_CONTINUE:
        if (loop) {
            node->_continue.loop = loop;
            break;
        }
        printf("Cannot call continue outside of a loop\n");
        exit(1);
    case N_BREAK:
        if (loop) {
            node->_break.loop = loop;
            break;
        }
        printf("Cannot call break outside of a loop\n");
        exit(1);
    }
}
