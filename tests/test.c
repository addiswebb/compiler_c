typedef struct {
    int count;
    int capacity;
    int element_size;
    void *data;
} Array;
void array_init(Array *arr, int initial_capacity, int element_size);
void array_free(Array *arr);
void ptr_array_free(Array *arr);
void *append(Array *arr, const void *element);
void *insert(Array *arr, const void *element, int index);
void pop(Array *arr);
void array_str_cpy(Array *arr, const char *str);
void array_str_catn(Array *arr, const char *str, int n);
void *get(const Array *arr, int index);
void set(const Array *arr, const void *element, int index);
typedef struct {
    Array blocks;
    int element_size;
    int block_capacity;
    int count;
} Arena;
void arena_init(Arena *arena, int block_size, int element_size);
static void arena_add_block(Arena *arena);
void arena_free(Arena *arena);
void *arena_append(Arena *arena, const void *element);
Array *arena_get_block(const Arena *arena, int index);
void *arena_get(const Arena *arena, int index);
void arena_set(Arena *arena, const void *element, int index);
typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long size_t;
typedef long int64_t;
typedef unsigned long uint64_t;
typedef enum {
    T_VOID,
    T_INT,
    T_FLOAT,
    T_POINTER,
    T_ARRAY,
    T_STRUCT,
    T_ENUM,
    T_UNION,
    T_FUNCTION,
    T_INVALID,
} TypeKind;
typedef struct Type Type;
typedef enum {
    MOD_POINTER,
    MOD_ARRAY,
    MOD_FUNCTION,
} ModifierKind;
typedef struct {
    const char *name;
    Array modifiers;
} Declarator;
typedef struct Symbol Symbol;
typedef struct {
    Type *type;
    const char *name;
    Symbol *symbol;
} ParamDecl;
typedef struct Node Node;
typedef struct {
    ModifierKind kind;
    union {
        Node *array_bounds;
        struct {
            Array params;
            int is_variadic;
        } function;
    };
} Modifier;
typedef struct {
    const char *name;
    Type *type;
    int offset;
} AggrMember;
typedef AggrMember StructMember;
typedef AggrMember UnionMember;
typedef struct {
    char *name;
    union {
        int value;
        Node *const_expr;
    };
    Type *_enum_t;
} EnumField;
typedef enum {
    QUAL_NONE = 0u,
    QUAL_CONST = 1u << 0,
    QUAL_VOLATILE = 1u << 1,
} TypeQualifier;
struct Type {
    TypeKind kind;
    int size;
    int align;
    int is_signed;
    unsigned int qualifiers;
    int is_resolved;
    int printing;
    union {
        Type *base;
        struct {
            Type *type;
            int gp_count;
            int fp_count;
        } abi;
    };
    union {
        struct {
            union {
                int64_t array_len;
                Node *const_expr;
            };
        } _array;
        struct {
            int complete;
            char *name;
            Array members_array;
        } _struct;
        struct {
            int complete;
            char *name;
            Array fields_array;
        } _enum;
        struct {
            int complete;
            char *name;
            Array members_array;
        } _union;
        struct {
            Type *return_type;
            Array params;
            int is_variadic;
        } _func;
    };
};
typedef enum {
    CONST_INTEGER,
    CONST_FLOAT,
    CONST_STRING,
    CONST_ARRAY,
    CONST_LABEL,
    CONST_REFERENCE,
} ConstLiteralKind;
typedef struct {
    Type *type;
    ConstLiteralKind kind;
    union {
        double f;
        int64_t i;
        struct {
            const char *data;
            int len;
        } s;
        Array arr;
        int const_index;
        struct {
            Symbol *symbol;
            int offset;
        } ref;
    };
} ConstLiteral;
extern Type *type_i8;
extern Type *type_i16;
extern Type *type_i32;
extern Type *type_i64;
extern Type *type_u8;
extern Type *type_u16;
extern Type *type_u32;
extern Type *type_u64;
extern Type *type_f32;
extern Type *type_f64;
extern Type *type_void;
extern Type *type_void_ptr;
extern Type *type_invalid;
extern Arena typepool;
static inline int align(int size, int align) { return (size + align - 1) & ~(align - 1); }
void init_typepool();
void free_typepool();
Type *init_global_type(TypeKind type, int size, unsigned int qualifiers, int is_signed);
Type *new_type();
Type *new_pointer_type(Type *type);
Type *new_qualified_type(Type *type, unsigned int qualifiers);
Type *new_unsigned_type(Type *type);
Type *new_array_type(Type *type, int len);
Type *new_incomplete_array_type(Type *type, Node *const_expr);
Type *get_float_type(int size);
Type *get_integer_type(int size);
Type *get_pointer_type(Type *type);
Type *get_enum_type(const char *name);
Type *get_union_type(const char *name);
Type *get_struct_type(const char *name);
Type *get_array_type(Type *type, int len);
Type *get_qualified_type(Type *type, unsigned int qualifiers);
Type *get_function_type(Type *type, Array params, int is_variadic);
Type *get_modified_type(Type *type, Declarator *decl);
Type *get_unsigned_type(Type *type);
int cmp_func_types(const Type *a, const Type *b);
Type *promote_integer(Type *from, Type *to);
Type *infer_array_length(Type *arr_type, int len);
void append_union_member(Type *u, UnionMember *m);
void append_enum_field(Type *e, EnumField *f);
void append_struct_member(Type *s, StructMember *f);
Type union_type();
Type struct_type();
Type enum_type();
AggrMember *get_member(Type *struct_t, const char *name, int is_root, int *offset);
int is_func_ptr(const Type *t);
int is_scalar_type(const Type *t);
void print_type(Type *type);
void print_struct_type(Type *s);
void print_param_decl(ParamDecl *decl);
void print_typepool();
typedef struct {
    char buf[1024];
    int size;
} Buffer;
extern const char *KEYWORDS[33];
typedef enum {
    TK_AUTO,
    TK_BREAK,
    TK_CASE,
    TK_CHAR,
    TK_CONST,
    TK_CONTINUE,
    TK_DEFAULT,
    TK_DO,
    TK_DOUBLE,
    TK_ELSE,
    TK_ENUM,
    TK_EXTERN,
    TK_FLOAT,
    TK_FOR,
    TK_GOTO,
    TK_IF,
    TK_INLINE,
    TK_INT,
    TK_LONG,
    TK_REGISTER,
    TK_RETURN,
    TK_SHORT,
    TK_SIGNED,
    TK_SIZEOF,
    TK_STATIC,
    TK_STRUCT,
    TK_SWITCH,
    TK_TYPEDEF,
    TK_UNION,
    TK_UNSIGNED,
    TK_VOID,
    TK_VOLATILE,
    TK_WHILE,
    TK_L_NOT,
    TK_BW_NOT,
    TK_INCR,
    TK_DECR,
    TK_EQ,
    TK_PLUS,
    TK_MINUS,
    TK_MULTIPLY,
    TK_DIVIDE,
    TK_MOD,
    TK_EQ_EQ,
    TK_PLUS_EQ,
    TK_MINUS_EQ,
    TK_MULTIPLY_EQ,
    TK_DIVIDE_EQ,
    TK_MOD_EQ,
    TK_NEQ,
    TK_LT,
    TK_LE,
    TK_GT,
    TK_GE,
    TK_SHL,
    TK_SHR,
    TK_SHL_EQ,
    TK_SHR_EQ,
    TK_AND,
    TK_AND_AND,
    TK_AND_EQ,
    TK_OR,
    TK_OR_OR,
    TK_OR_EQ,
    TK_XOR,
    TK_XOR_EQ,
    TK_TERNARY,
    TK_DOT,
    TK_ARROW,
    TK_OPEN_PAREN,
    TK_CLOSE_PAREN,
    TK_OPEN_CURLY,
    TK_CLOSE_CURLY,
    TK_OPEN_SQUARE,
    TK_CLOSE_SQUARE,
    TK_COMMA,
    TK_SEMI,
    TK_COLON,
    TK_INT_LITERAL,
    TK_FLT_LITERAL,
    TK_CHAR_LITERAL,
    TK_STRING_LITERAL,
    TK_EXPR,
    TK_IDENTIFIER,
    TK_ELLIPSES,
} TokenType;
typedef struct {
    TokenType type;
    char *value;
    int size;
    int line_n;
    int char_n;
} Token;
typedef struct {
    Token *data;
    int size;
    int capacity;
} TokenArray;
typedef struct {
    const char *src;
    int index;
    int size;
    int line_n;
    int char_n;
    Array tokens_array;
    Buffer buf;
} Tokenizer;
typedef struct {
    TokenType type;
    int n_chars;
} TokenMatch;
Tokenizer t_new_tokenizer(const char *src, int src_size);
void t_free(Tokenizer *tokenizer);
void t_tokenize(Tokenizer *tk);
int is_postfix_operator(const TokenType type);
int is_unary_operator(const TokenType type);
int is_binary_operator(const TokenType type);
int is_assignment_op(const TokenType type);
int is_arithmetic_op(const TokenType type);
int is_bitwise_op(const TokenType type);
int is_comparison_op(const TokenType type);
int is_logical_op(const TokenType type);
TokenType get_underlying_op(TokenType type);
int op_associativity(TokenType type);
int op_precedence(TokenType type);
const char *token_type_str(const TokenType type);
void print_token_type(TokenType type);
void print_token(const Token *token);
static inline Token *get_token(Array *arr, int index) { return (Token *)get(arr, index); }
void *memcpy(void *, const void *, size_t);
void *memmove(void *, const void *, size_t);
void *memset(void *, int, size_t);
int memcmp(const void *, const void *, size_t);
char *strcpy(char *, const char *);
char *strncpy(char *, const char *, size_t);
int strcmp(const char *, const char *);
int strncmp(const char *, const char *, size_t);
size_t strlen(const char *);
char *strrchr(const char *, int);
char *strdup(const char *);
char *strndup(const char *, size_t);
typedef enum {
    N_TRANSLATION_UNIT,
    N_FUNCTION,
    N_COMPOUND,
    N_VAR_DECL,
    N_IF,
    N_WHILE,
    N_FOR,
    N_SWITCH,
    N_CASE,
    N_RETURN,
    N_UNARY,
    N_BINARY,
    N_TERNARY,
    N_LITERAL,
    N_IDENTIFIER,
    N_FUNCTION_CALL,
    N_CAST,
    N_INDEX,
    N_TYPE,
    N_TYPEDEF,
    N_CONTINUE,
    N_BREAK,
    N_INIT_LIST,
    N_MEMBER_ACCESS,
    N_GOTO,
    N_LABEL,
    N_COMPOUND_LITERAL,
    N_DESIGNATED_INITIALIZER,
    N_BUILTIN,
    N_NULL,
} NodeKind;
typedef enum {
    BUILTIN_MEMCPY,
    BUILTIN_VA_START,
    BUILTIN_VA_ARG,
    BUILTIN_VA_END,
    BUILTIN_NONE,
} BuiltinKind;
extern const char *builtin_names[4];
typedef enum {
    L_INT,
    L_FLOAT,
    L_CHAR,
    L_STRING,
} LiteralKind;
typedef enum { NONE, EXTERN, STATIC } StorageClass;
typedef struct Node Node;
typedef struct Symbol Symbol;
struct Node {
    NodeKind kind;
    Type *type;
    union {
        struct {
            Array declarations_array;
        } translation_unit;
        struct {
            const char *name;
            Node *body;
            StorageClass storage_class;
            Symbol *symbol;
            int is_global;
            int is_defined;
            int is_inline;
        } func;
        struct {
            Array items_array;
        } compound;
        struct {
            Node *expr;
            TokenType op;
            int associativity;
        } unary;
        struct {
            Node *lhs;
            Node *rhs;
            TokenType op;
            Type *common_type;
        } binary;
        struct {
            Node *cond;
            Node *if_true;
            Node *if_false;
        } ternary;
        struct {
            Node *expr;
        } _return;
        struct {
            Node *cond;
            Node *if_true;
            Node *if_false;
        } _if;
        struct {
            Node *cond;
            Node *block;
            int is_do_while;
        } _while;
        struct {
            Node *init;
            Node *cond;
            Node *iter;
            Node *block;
        } _for;
        struct {
            LiteralKind kind;
            const char *raw_rata;
            int len;
            union {
                int64_t i;
                double f;
                char c;
                struct {
                    char *data;
                    int len;
                } s;
                void *ptr;
            };
            Symbol *symbol;
        } literal;
        struct {
            Node *value;
        } compound_literal;
        struct {
            const char *name;
            int len;
            Symbol *symbol;
        } identifier;
        struct {
            Node *identifier;
            Node *expr;
            ConstLiteral *const_expr;
            StorageClass storage_class;
            int is_global;
            int is_defined;
            Symbol *symbol;
        } var_decl;
        struct {
            Node *callee;
            Array params_array;
            Symbol *symbol;
        } func_call;
        struct {
            Node *expr;
            Type *from;
            Type *to;
        } cast;
        struct {
            Node *identifier;
            Node *index;
        } index;
        struct {
            Node *loop;
        } _break;
        struct {
            Node *loop;
        } _continue;
        struct {
            Array elements_array;
        } init_list;
        struct {
            TokenType op;
            Node *identifier;
            Node *member;
            int offset;
        } member_access;
        struct {
            Node *test;
            Node *block;
            Array cases_array;
        } _switch;
        struct {
            Node *const_expr;
            int64_t test;
            int i;
        } _case;
        struct {
            const char *name;
        } _typedef;
        struct {
            Node *identifier;
        } _goto;
        struct {
            Node *identifier;
        } label;
        struct {
            TypeKind kind;
            union {
                struct {
                    int is_complete;
                    union {
                        unsigned int index;
                        Node *const_expr;
                    };
                } _array;
                struct {
                    const char *name;
                    StructMember *member;
                } _struct;
                struct {
                    const char *name;
                    UnionMember *member;
                } _union;
            };
            Node *value;
        } designated_init;
        struct {
            BuiltinKind kind;
            Array params;
        } _builtin;
    };
};
typedef Arena NodeManager;
NodeManager new_node_manager();
void free_node_manager(NodeManager *nm);
BuiltinKind get_builtin_kind(const char *name);
Node *new_node(NodeManager *nm, NodeKind kind);
Node *init_translation_unit(NodeManager *nm);
Node *new_compound_node(NodeManager *nm);
Node *new_init_list_node(NodeManager *nm);
Node *new_function_node(NodeManager *nm);
Node *new_function_call_node(NodeManager *nm, Node *identifier);
Node *cast_node_unchecked(NodeManager *nm, Node *node, Type *type);
Node *cast_node(NodeManager *nm, Node *node, Type *type);
int is_valid_cast(const Type *from, const Type *to);
LiteralKind literal_kind(TokenType type);
void print_node_type(NodeKind type);
void print_indent(int depth);
void print_node(const Node *node, int depth);
void print_ast(const NodeManager *nm);
void free_node(Node *node);
static inline Node *get_node(const Array *node_array, int index) { return *(Node **)get(node_array, index); }
static inline void set_node(Array *node_array, Node **node, int index) {
    memcpy((char *)node_array->data + index * node_array->element_size, node, sizeof(Node *));
}
static inline Node *insert_node(Array *node_array, Node **node, int index) { return (Node *)insert(node_array, node, index); }
typedef struct {
    const char *new_def;
    Type *type;
} Typedef;
typedef enum {
    ENUM,
    VAR,
    FUNC,
    TYPEDEF,
    ANY,
} SymbolKind;
typedef enum { LINK_NONE, LINK_INTERNAL, LINK_EXTERNAL } Linkage;
typedef enum {
    STORAGE_NONE,
    STORAGE_DATA,
    STORAGE_BSS,
    STORAGE_TEXT,
} Storage;
typedef struct Symbol {
    const char *name;
    Type *type;
    SymbolKind kind;
    Linkage linkage;
    Storage storage;
    int scope_depth;
    union {
        Node *var_decl;
        Node *func_def;
        const EnumField *enum_field;
        Typedef _typedef;
    };
} Symbol;
typedef struct {
    int index;
    int size;
    int current_scope_depth;
    Array *src;
    int expect_semi;
    Array scopes_array;
    Arena symbols_arena;
} Parser;
Parser new_parser();
void init_parser(Parser *p, Array *src, int size);
void free_parser(Parser *p);
int p_is_last_token(const Parser *p);
Token *p_peek_n(const Parser *p, int n);
Token *p_peek(const Parser *p);
Token *p_peek_next(const Parser *p);
Token *p_consume_n(Parser *p, int n);
Token *p_consume(Parser *p);
void p_skip_n(Parser *p, int n);
void p_skip(Parser *p);
void p_expect(const Parser *p, TokenType expected_type);
Token *p_consume_a(Parser *p, TokenType type);
Token *p_consume_semi(Parser *p);
int is_type_token(const Parser *p, const Token *t);
int is_storage_classifier(const TokenType type);
int is_qualifier_token(const TokenType type);
int is_start_of_type(const Parser *p, const Token *tk);
Type *token_to_type(Parser *p, const Token *t);
void p_push_scope(Parser *p);
void p_pop_scope(Parser *p);
Node *p_parse_primary_expression(Parser *p, NodeManager *nm);
Node *p_parse_postfix_expression(Parser *p, NodeManager *nm);
Node *p_parse_builtin(Parser *p, NodeManager *nm, BuiltinKind kind);
Node *p_parse_prefix(Parser *p, NodeManager *nm);
Node *p_parse_literal(Parser *p, NodeManager *nm);
Node *p_parse_init_list(Parser *p, NodeManager *nm);
Node *p_parse_expression(Parser *p, NodeManager *nm, int min_prec);
Node *p_parse_binary(Parser *p, NodeManager *nm, Node *lhs, const int min_prec);
Node *p_parse_ternary(Parser *p, NodeManager *nm, Node *cond);
Node *p_parse_cast(Parser *p, NodeManager *nm);
Node *p_parse_goto_statement(Parser *p, NodeManager *nm);
Node *p_parse_label(Parser *p, NodeManager *nm);
Node *p_parse_block_item(Parser *p, NodeManager *nm);
Node *p_parse_if_statement(Parser *p, NodeManager *nm);
Node *p_parse_while_loop(Parser *p, NodeManager *nm);
Node *p_parse_do_while_loop(Parser *p, NodeManager *nm);
Node *p_parse_switch_statement(Parser *p, NodeManager *nm);
Node *p_parse_case(Parser *p, NodeManager *nm);
Node *p_parse_for_loop(Parser *p, NodeManager *nm);
Node *p_parse_return(Parser *p, NodeManager *nm);
Node *p_parse_continue(Parser *p, NodeManager *nm);
Node *p_parse_break(Parser *p, NodeManager *nm);
Node *p_parse_statement(Parser *p, NodeManager *nm);
Node *p_parse_compound(Parser *p, NodeManager *nm);
Node *p_parse_function(Parser *p, NodeManager *nm, Type *type, const char *name, const StorageClass storage_class, int is_inline);
Modifier p_parse_parameter_list(Parser *p, NodeManager *nm);
Node *p_parse_external_declaration(Parser *p, NodeManager *nm);
Node *p_parse_block_declaration(Parser *p, NodeManager *nm);
Node *p_parse_decl_identifier(Parser *p, NodeManager *nm);
Node *p_parse_declaration(Parser *p, NodeManager *nm, Type *type, const char *name, const StorageClass storage_class, const int global);
Node *p_parse_typedef(Parser *p, NodeManager *nm);
Node *p_parse_translation_unit(Parser *p, NodeManager *nm);
Type *p_parse_abstract_type(Parser *p, NodeManager *nm);
Type *p_parse_type(Parser *p, NodeManager *nm, const char **name);
Declarator p_parse_declarator(Parser *p, NodeManager *nm);
Type *p_parse_enum(Parser *p, NodeManager *nm);
Type *p_parse_struct(Parser *p, NodeManager *nm);
Type *p_parse_union(Parser *p, NodeManager *nm);
StorageClass p_parse_storage_classifier(Parser *p, NodeManager *nm);
Type *parse_int_suffix(const char *raw, int *len);
Type *parse_float_suffix(const char *raw, int *len);
int64_t parse_int(const char *raw, int len);
int64_t parse_binary(const char *raw, int len);
int64_t parse_oct(const char *raw, int len);
int64_t parse_dec(const char *raw, int len);
int64_t parse_hex(const char *raw, int len);
int parse_multi_character(const char *raw, int len);
double parse_float(const char *raw, int len);
Symbol *p_get_symbol(const Parser *p, const char *name, const SymbolKind kind, const int same_depth);
Node *p_get_func_def(const Parser *p, const char *name);
Typedef *p_get_typedef(const Parser *p, const char *name);
Node *p_get_var_decl(const Parser *p, const char *name);
const EnumField *p_get_enum_const(const Parser *p, const char *name);
Symbol *p_new_symbol(Parser *p, const Symbol *s);
void p_append_call_param(Node *func_call, Node *param);
void p_append_param(Node *func, Node *param);
void p_append_enum_const(Parser *p, const EnumField *e);
Symbol *p_append_var_decl_symbol(Parser *p, Node *v);
void update_linkage_storage(Symbol *s, Node *v);
Symbol *p_append_param_decl_symbol(Parser *p, ParamDecl *param);
void p_append_element(Node *init_list, Node *element);
void p_append_symbol_table(Parser *p);
Symbol *p_append_symbol(Array *st, const Symbol *s);
void p_append_typedef(Parser *p, const Typedef *t);
Symbol *p_append_func_def(Parser *p, Node *f);
void p_append_block_item(Node *root, Node *item);
void p_append_case(Node *s, Node *c);
static inline Array *get_symbol_table(const Parser *p, int index) { return (Array *)get(&p->scopes_array, index); }
static inline Array *get_current_symbol_table(Parser *p) { return get_symbol_table(p, p->scopes_array.count - 1); }
static inline Symbol *get_symbol(Array *symbol_table, int index) { return *(Symbol **)get(symbol_table, index); }
static inline EnumField *get_enum_field(const Type *enum_t, int index) { return (EnumField *)get(&enum_t->_enum.fields_array, index); }
static inline StructMember *get_struct_member(const Type *struct_t, int index) {
    return (StructMember *)get(&struct_t->_struct.members_array, index);
}
static inline UnionMember *get_union_member(const Type *union_t, int index) {
    return (UnionMember *)get(&union_t->_union.members_array, index);
}
UnionMember *get_union_member_named(Type *union_t, const char *name);
StructMember *get_struct_member_named(Type *struct_t, const char *name, int *index);
typedef struct {
    Node *func;
    Array compound_stack;
    Array loop_stack;
    Array i_array;
} SemanticContext;
int is_lvalue(const Node *n);
int is_deref(const Node *n);
Type *check_unary_op(NodeManager *nm, Node *unary_op);
Type *check_binary_op(NodeManager *nm, TokenType op, Node *binop);
Type *promote_binary_operands(NodeManager *nm, Node *binop);
void semantic_analysis(SemanticContext *sema_ctx, Parser *p, NodeManager *nm, Node *node);
void handle_builtin_call(BuiltinKind kind, Node *node);
void lower_nodes(NodeManager *nm);
void lower_compound_literal(SemanticContext *sema_ctx, Parser *p, NodeManager *nm, Node *node);
void push_sema_scope(SemanticContext *sema_ctx, Parser *p, Node *n);
void pop_sema_scope(SemanticContext *sema_ctx, Parser *p);
void push_sema_loop(SemanticContext *sema_ctx, const Node *loop);
void pop_sema_loop(SemanticContext *sema_ctx);
Node *sema_current_loop(const SemanticContext *sema_ctx);
Node *sema_current_compound(const SemanticContext *sema_ctx);
static inline int *get_i(SemanticContext *sema_ctx) { return (int *)get(&sema_ctx->i_array, sema_ctx->i_array.count - 1); }
typedef enum{
    RAX, RBX, RCX, RDX,
    RSI, RDI,
    RBP, RSP,
    R8, R9, R10, R11, R12, R13, R14, R15,
}GP_Reg;
typedef enum{
    XMM0, XMM1, XMM2, XMM3,
    XMM4, XMM5, XMM6, XMM7,
    XMM8, XMM9, XMM10, XMM11,
    XMM12, XMM13, XMM14, XMM15,
}XMM_Reg;
typedef enum{
    REG_8,
    REG_16,
    REG_32,
    REG_64,
}RegSize;
typedef enum{
    REG_GP,
    REG_XMM,
    REG_IP
}RegKind;
typedef enum {
    REG_DATA_LABEL,
    REG_DATA_OFFSET,
    REG_DATA_CONST_INDEX,
    REG_DATA_NONE,
}RegDataKind;
struct PhysReg{
    RegKind kind;
    union{
        GP_Reg gp_reg;
        XMM_Reg sse_reg;
    };
    RegSize size;
    RegDataKind data_kind;
    union{
        const char* label;
        struct{
            int offset;
            int scale;
        };
        int const_index;
    };
};
RegSize reg_size(int size);
typedef enum {
    LT,
    LE,
    GT,
    GE,
    EQ,
    NEQ,
} IR_CMP_OP;
typedef enum {
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    BW_AND,
    L_AND,
    BW_OR,
    L_OR,
    XOR,
    SHR,
    SHL,
} IR_BINOP_OP;
typedef enum {
    POS,
    NEG,
    LNOT,
    BNOT,
    REF,
    DEREF,
} IR_UNARY_OP;
typedef enum {
    IR_CONST,
    IR_UNOP,
    IR_BINOP,
    IR_LOAD,
    IR_STORE,
    IR_MOVE,
    IR_RET,
    IR_CALL,
    IR_BR,
    IR_BR_COND,
    IR_CMP,
    IR_CAST,
    IR_ADDR,
    IR_ALLOCA,
    IR_MEMCPY,
    IR_LABEL,
    IR_JMP,
    IR_PARAM,
    IR_BUILTIN_VA_START,
    IR_BUILTIN_VA_ARG,
} IR_OP;
typedef enum {
    IR_UNDEFINED,
    IR_SYMBOL,
    IR_VREG,
    IR_CONSTANT,
    IR_PHYS_REG,
    IR_INT_LITERAL,
} IR_ValueKind;
typedef struct PhysReg PhysReg;
typedef struct IR_Value {
    IR_ValueKind kind;
    int size;
    int align;
    union {
        Symbol *symbol;
        int vreg;
        int const_index;
        PhysReg phys_reg;
        int64_t int_literal;
    };
} IR_Value;
typedef struct {
    Symbol *symbol;
    ConstLiteral val;
} IR_Global;
typedef struct {
    Type *type;
    IR_Value v;
} IR_CallArg;
typedef struct {
    uint8_t def_mask;
    uint8_t use_mask;
} IR_OpInfo;
extern IR_OpInfo op_info[];
typedef struct IR_Block IR_Block;
typedef struct {
    const char *name;
    int index;
    int is_defined;
    int is_variadic;
    StorageClass storage_class;
} IR_Func_Def;
typedef struct {
    IR_OP op;
    IR_Value ops[3];
    int op_count;
    union {
        struct {
            Type *type;
        } _const;
        struct {
            Type *type;
        } load;
        struct {
            Type *type;
        } store;
        struct {
            Type *type;
        } ret;
        struct {
            IR_UNARY_OP op;
            Type *type;
        } unary;
        struct {
            IR_BINOP_OP op;
            Type *type;
        } binop;
        struct {
            IR_CMP_OP op;
            Type *type;
        } cmp;
        struct {
            Array arg_array;
            Type *type;
        } call;
        struct {
            IR_Block *block;
        } br;
        struct {
            IR_Block *t_block;
            IR_Block *f_block;
        } br_cond;
        struct {
            Type *from;
            Type *to;
        } cast;
        struct {
            int offset;
        } addr;
        struct {
            int size;
        } alloca;
        struct {
            int size;
        } memcpy;
        struct {
            const char *name;
        } label;
        struct {
            const char *name;
        } jmp;
        struct {
            Type *type;
        } builtin_va_arg;
        struct {
            Type *type;
            int param_index;
        } param;
    };
} IR_Instruction;
typedef enum {
    SLOT_REGISTER,
    SLOT_STACK,
} RegisterSlotKind;
typedef struct {
    IR_Value v;
    int free_at;
} RegisterSlot;
typedef struct {
    unsigned int *data;
    int num_bits;
    int capacity;
} BitSet;
typedef struct {
    int *succ;
    int succ_count;
    int *pred;
    int pred_count;
} IR_BlockCFG;
typedef struct {
    BitSet def;
    BitSet use;
    BitSet live_in;
    BitSet live_out;
} IR_BlockLiveness;
typedef struct {
    int reg;
    int start;
    int end;
    int stack_slot;
    int stack_offset;
    IR_Value *v;
} Lifetime;
struct IR_Block {
    int id;
    Array instruction_array;
    IR_BlockCFG cfg;
    IR_BlockLiveness live;
};
typedef struct {
    int reg_count;
    int stack_pointer;
    Array var_array;
} IR_Scope;
typedef struct {
    const char *name;
    Array blocks_array;
    int next_reg;
    int max_reg;
    int stack_size;
    Array locals_array;
    Array scopes_array;
    int stack_slot_count;
    int stack_slot_capacity;
    Linkage linkage;
    Storage storage;
    Type *type;
} IR_Function;
typedef struct {
    int count;
    int capacity;
    IR_Global *globals;
} IR_Global_Pool;
typedef struct {
    IR_Block *block;
    const char *label;
    int placeholder;
} IR_LabeledBlock;
typedef struct {
    Array functions_array;
    Array const_array;
    Array global_array;
    Array labeled_block_array;
} IR_Module;
typedef struct {
    IR_Block *continue_block;
    IR_Block *break_block;
} IR_LoopContext;
typedef struct {
    IR_LoopContext *data;
    int size;
    int capacity;
} IR_LoopStack;
typedef struct {
    IR_Module *module;
    IR_Function *func;
    IR_Block *block;
    Array loop_stack_array;
    IR_Block *true_block;
    IR_Block *false_block;
    Arena *symbol_table;
    int func_not_address;
} IR_Context;
extern const IR_Value ir_no_value;
IR_Context ir_init_ctx(Parser *p);
void free_ir_ctx(IR_Context *ctx);
IR_Module *ir_gen_translation_unit(IR_Context *ctx, const Node *tu);
void ir_push_loop_ctx(IR_Context *ctx, IR_Block *continue_block, IR_Block *break_block);
void ir_pop_loop_ctx(IR_Context *ctx);
IR_Value ir_literal_value(int const_index);
void ir_append_instruction(IR_Block *b, IR_Instruction *instr);
void ir_free_module(IR_Module *module);
void ir_begin_scope(IR_Function *func);
void ir_end_scope(IR_Function *func);
IR_Value ir_next_virtual_reg(IR_Function *func);
IR_Value ir_integer_literal(int64_t i);
IR_Module *ir_new_module();
IR_Function *ir_new_function(IR_Context *ctx, const char *name, Type *type);
IR_Func_Def *ir_append_func_def(const IR_Context *ctx, const char *name, const int is_defined, const int is_variadic,
                                const StorageClass storage_class);
IR_Value ir_new_var(IR_Function *func, const char *name, Type *type);
IR_Block *ir_new_block();
IR_Block *ir_add_block(IR_Context *ctx);
IR_Value ir_symbol_value(Symbol *s);
void ir_append_function(const IR_Context *ctx, IR_Function *func);
void ir_append_global(IR_Module *module, Symbol *symbol, const ConstLiteral *literal);
int ir_append_const(IR_Module *module, const ConstLiteral *literal);
IR_Block *ir_append_block(IR_Context *ctx, IR_Block *block);
IR_LabeledBlock *ir_append_labeled_block(IR_Context *ctx, const char *label);
IR_Value ir_get_symbol_value(IR_Context *ctx, const char *name, int give_lvalue);
IR_LabeledBlock *ir_get_labeled_block(IR_Context *ctx, const char *label);
static inline IR_Instruction *get_instruction(const Array *arr, int index) { return (IR_Instruction *)get(arr, index); }
static inline IR_LoopContext *get_loop_ctx(const IR_Context *ctx) {
    return (IR_LoopContext *)get(&ctx->loop_stack_array, ctx->loop_stack_array.count - 1);
}
static inline IR_Scope *get_current_scope(const IR_Function *func) {
    return (IR_Scope *)get(&func->scopes_array, func->scopes_array.count - 1);
}
static inline IR_Scope *get_scope(const IR_Function *func, int index) { return (IR_Scope *)get(&func->scopes_array, index); }
static inline IR_Block *get_block(const IR_Function *func, int index) { return *(IR_Block **)get(&func->blocks_array, index); }
static inline ConstLiteral *get_const(const IR_Context *ctx, int index) { return (ConstLiteral *)get(&ctx->module->const_array, index); }
static inline IR_Global *get_global(const IR_Context *ctx, int index) { return (IR_Global *)get(&ctx->module->global_array, index); }
static inline Symbol *get_local_symbol(const IR_Function *func, int index) { return *(Symbol **)get(&func->locals_array, index); }
static inline int get_var_index(const IR_Scope *scope, int index) { return *(int *)get(&scope->var_array, index); }
static inline IR_Function *get_func(const IR_Module *module, int index) { return *(IR_Function **)get(&module->functions_array, index); }
static inline IR_LabeledBlock *get_labeled_block(const IR_Module *module, int index) {
    return (IR_LabeledBlock *)get(&module->labeled_block_array, index);
}
static inline IR_CallArg *get_call_arg(const IR_Instruction *call, int index) { return (IR_CallArg *)get(&call->call.arg_array, index); }
typedef struct{
    unsigned int gp_offset;
    unsigned int fp_offset;
    void *overflow_args;
    void *reg_save_area;
} va_list;
typedef struct FILE FILE;
extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;
FILE *fopen(const char *, const char *);
FILE *fdopen(int, const char *);
int fclose(FILE *);
int fflush(FILE *);
int fputc(int, FILE *);
int putc(int, FILE *);
int putchar(int);
char *fgets(char *, int, FILE *);
char fgetc(FILE *);
char *gets(char *);
int fputs(const char *, FILE *);
int puts(const char *);
int printf(const char *, ...);
int fprintf(FILE *, const char *, ...);
int sprintf(char *, const char *, ...);
int snprintf(char *, size_t, const char *, ...);
extern int vfprintf(FILE *, const char *, va_list);
FILE *popen(const char *, const char *);
int pclose(FILE *);
typedef enum {
    ABI_NO_CLASS,
    ABI_MEMORY,
    ABI_INTEGER,
    ABI_SSE,
} ABI_TypeClass;
typedef struct {
    ABI_TypeClass class[2];
    int memory;
    char p[8];
} ABI_Result;
ABI_Result abi_classify(Type *type);
IR_Value abi_lower_param_register(Type *type, int i);
int is_va_list_type(Type *type);
void abi_lower_ret(IR_Function *f, IR_Block *b, IR_Instruction *instr, int *i);
void abi_lower_param(IR_Function *f, IR_Block *b, IR_Instruction *instr, int *i, int param_index, int *param_cursor);
void abi_lower_store(IR_Function *f, IR_Block *b, IR_Instruction *instr, int *i);
void abi_emit_call(FILE *fp, IR_Context *ctx, const IR_Instruction *instr);
void abi_func_type_gen(Type *type);
IR_Value abi_gen_builtin(IR_Context *ctx, const Node *expr);
void abi_gen_params(IR_Context *ctx, IR_Function *f);
void abi_gen_memcpy_instruction(FILE *fp, const IR_Instruction *instr);
extern Symbol *_hidden_sret_ptr;
Symbol *current_sret();
void set_sret(Type *return_type);
void set_hidden_sret_ptr(Type *return_type);
ABI_TypeClass merge(ABI_TypeClass chunk_class, ABI_TypeClass field_class);
ABI_Result classify_struct(Type *type);
extern const GP_Reg int_param_regs[6];
extern const XMM_Reg float_param_regs[8];
extern const GP_Reg caller_saved_regs[9];
extern const GP_Reg callee_saved_regs[6];
extern const char *gp_register_str[16][4];
extern const char *sse_register_str[16];
ConstLiteral evaluate_const_expression(const Node *node);
ConstLiteral evaluate_const_literal(const Node *node);
void print_const_literal(const ConstLiteral *node);
void free_const_literal(ConstLiteral *l);
void exit(int);
void *malloc(size_t);
void *realloc(void *, size_t);
void *calloc(size_t, size_t);
void free(void *);
int system(const char *);
typedef int (*__compar_fn_t)(const void *, const void *);
extern void qsort(void *__base, size_t __nmemb, size_t __size, __compar_fn_t __compar);
typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_PANIC,
} LogLevel;
typedef enum {
    STAGE_COMPILER,
    STAGE_TOKENIZING,
    STAGE_PARSING,
    STAGE_SEMA_ANALYSIS,
    STAGE_IR,
    STAGE_X86_GEN,
    STAGE_ASSEMBLER,
    STAGE_LINKER
} LogStage;
typedef struct {
    LogLevel min_level;
    LogStage stage;
    FILE *file;
} Logger;
extern Logger logger;
static inline void init_logger(FILE *fp, LogLevel level) {
    logger.file = fp ? fp : stderr;
    logger.stage = STAGE_COMPILER;
    logger.min_level = level;
}
static inline char *stage_str(LogStage stage) {
    switch (logger.stage) {
    case STAGE_COMPILER:
        return "Compiler";
    case STAGE_TOKENIZING:
        return "Tokenizer";
    case STAGE_PARSING:
        return "Parser";
    case STAGE_IR:
        return "IR";
    case STAGE_X86_GEN:
        return "x86 Gen";
    case STAGE_SEMA_ANALYSIS:
        return "Semantic Analysis";
    case STAGE_ASSEMBLER:
        return "Assembler";
    case STAGE_LINKER:
        return "Linker";
    }
}
static inline void set_log_stage(LogStage stage) { logger.stage = stage; }
static inline void log_start(LogLevel lvl) {
    if (lvl < logger.min_level) return;
    const char *level_str;
    switch (lvl) {
    case LOG_DEBUG:
        level_str = "\x1b[34mDEBUG\x1b[0m";
        break;
    case LOG_INFO:
        level_str = "\x1b[32mINFO\x1b[0m";
        break;
    case LOG_WARN:
        level_str = "\x1b[33mWARN\x1b[0m";
        break;
    case LOG_ERROR:
        level_str = "\x1b[31mERROR\x1b[0m";
        break;
    case LOG_PANIC:
        level_str = "\x1b[1;31mPANIC\x1b[0m";
        break;
    default:
        level_str = "LOG";
        break;
    }
    const char *stage = stage_str(logger.stage);
    fprintf(logger.file, "[%s] %s: ", level_str, stage);
}
void print(const char *fmt, ...);
void vprint(const char *fmt, va_list ap);
static inline void log_message(LogLevel lvl, const char *fmt, ...) {
    if (lvl < logger.min_level) return;
    log_start(lvl);
    va_list args;
    __builtin_va_start(args,fmt);
    vprint(fmt, args);
    __builtin_va_end(args);
    fflush(logger.file);
    if (lvl == LOG_PANIC) exit(1);
}
int is_lvalue(const Node *n) { return n->kind == N_IDENTIFIER || n->kind == N_INDEX || n->kind == N_MEMBER_ACCESS || is_deref(n); }
int is_deref(const Node *n) { return n->kind == N_UNARY && n->unary.op == TK_MULTIPLY; }
Type *check_unary_op(NodeManager *nm, Node *unary_op) {
    const Node *expr = unary_op->unary.expr;
    const TypeKind kind = expr->type->kind;
    switch (unary_op->unary.op) {
    case TK_PLUS:
    case TK_MINUS:
        if (kind == T_INT || kind == T_FLOAT) return expr->type;
        break;
    case TK_L_NOT:
        if (kind == T_INT || kind == T_FLOAT || kind == T_POINTER) return type_i32;
        break;
    case TK_BW_NOT:
        if (kind == T_INT) return expr->type;
        break;
    case TK_AND:
        if (is_lvalue(expr)) {
            return get_pointer_type(expr->type);
        }
        do { log_message(LOG_ERROR, "Tried to reference a non assignable term\n"); exit(1); } while (0);
    case TK_MULTIPLY:
        if (expr->type->kind == T_ARRAY) {
            unary_op->unary.expr = cast_node(nm, unary_op->unary.expr, get_pointer_type(expr->type->base));
        }
        if (expr->type->base && expr->type->base != type_invalid) return expr->type->base;
        do { log_message(LOG_ERROR, "Tried to dereference some nonexistent term\n"); exit(1); } while (0);
    case TK_SIZEOF:
        if (expr->type != type_invalid && expr->type->size) return type_i32;
        do { log_message(LOG_ERROR, "Tried to get the sizeof something without a size\n"); exit(1); } while (0);
    case TK_INCR:
    case TK_DECR:
        return expr->type;
    default:
        break;
    }
    printf("Invalid operand type ");
    print_type(expr->type);
    printf(" for the given unary operator ");
    print_token_type(unary_op->unary.op);
    printf("\n");
    return type_invalid;
}
int is_valid_binary_op(TokenType op, const Node *lhs, const Node *rhs) {
    if (op == TK_EQ) return 1;
    if (is_assignment_op(op)) op = get_underlying_op(op);
    if (op == TK_AND_AND || op == TK_OR_OR) return is_scalar_type(lhs->type) && is_scalar_type(rhs->type);
    int is_lhs_ptr = lhs->type->kind == T_POINTER || lhs->type->kind == T_ARRAY;
    int is_rhs_ptr = rhs->type->kind == T_POINTER || rhs->type->kind == T_ARRAY;
    if ((is_lhs_ptr || is_rhs_ptr) && is_bitwise_op(op)) return 0;
    if (is_lhs_ptr && !is_rhs_ptr) return op == TK_PLUS || op == TK_MINUS || op == TK_EQ_EQ || op == TK_NEQ;
    if (!is_lhs_ptr && is_rhs_ptr) return op == TK_PLUS || op == TK_EQ_EQ || op == TK_NEQ;
    if (is_lhs_ptr && is_rhs_ptr) return op == TK_MINUS || is_comparison_op(op);
    return 1;
}
Type *check_binary_op(NodeManager *nm, const TokenType op, Node *binop) {
    if (binop->binary.lhs->type == type_invalid || binop->binary.rhs->type == type_invalid) {
        do { log_message(LOG_ERROR, "Binary op was given expression with an invalid type\n"); exit(1); } while (0);
    }
    const Node *lhs = binop->binary.lhs;
    Node *rhs = binop->binary.rhs;
    if (!is_valid_binary_op(op, lhs, rhs)) {
        do { log_message(LOG_ERROR, "Invalid arithmetic operands\n"); exit(1); } while (0);
    }
    if (is_assignment_op(op)) {
        if (!(is_lvalue(lhs))) do { log_message(LOG_ERROR, "Binary op lhs is not assignable\n"); exit(1); } while (0);
        if (lhs->type != rhs->type) binop->binary.rhs = cast_node(nm, rhs, lhs->type);
        return lhs->type;
    }
    Type *common = promote_binary_operands(nm, binop);
    if (!common || common == type_invalid) {
        do { log_message(LOG_ERROR, "Invalid arithmetic operands\n"); exit(1); } while (0);
    }
    if (is_arithmetic_op(op)) return common;
    if (is_comparison_op(op)) return common;
    if (is_bitwise_op(op)) {
        if (!(lhs->type->kind == T_INT || lhs->type->kind == T_ENUM) || !(rhs->type->kind == T_INT || rhs->type->kind == T_ENUM)) {
            do { log_message(LOG_ERROR, "Bitwise operation requires integers\n"); exit(1); } while (0);
        }
        return type_i32;
    }
    if (is_logical_op(op)) return type_i32;
    printf("Unknown binary operator\n");
    return type_invalid;
}
Type *promote_binary_operands(NodeManager *nm, Node *binop) {
    Type *common = type_invalid;
    Node **lhs = &binop->binary.lhs;
    Node **rhs = &binop->binary.rhs;
    if ((*lhs)->type->kind == T_ENUM) {
        *lhs = cast_node(nm, (*lhs), type_i32);
    }
    if ((*rhs)->type->kind == T_ENUM) {
        *rhs = cast_node(nm, (*rhs), type_i32);
    }
    if ((*lhs)->type->kind == T_ARRAY) {
        *lhs = cast_node(nm, (*lhs), get_pointer_type((*lhs)->type->base));
    }
    if ((*rhs)->type->kind == T_ARRAY) {
        *rhs = cast_node(nm, (*rhs), get_pointer_type((*rhs)->type->base));
    }
    if (is_arithmetic_op(binop->binary.op) || is_comparison_op(binop->binary.op)) {
        if ((*lhs)->type->kind == T_INT && (*lhs)->type->size < type_i32->size)
            *lhs = cast_node(nm, (*lhs), (*lhs)->type->is_signed ? type_i32 : type_u32);
        if ((*rhs)->type->kind == T_INT && (*rhs)->type->size < type_i32->size)
            *rhs = cast_node(nm, (*rhs), (*rhs)->type->is_signed ? type_i32 : type_u32);
    }
    if ((*lhs)->type->kind == T_FLOAT || (*rhs)->type->kind == T_FLOAT) {
        common = (*lhs)->type->size > (*rhs)->type->size ? (*lhs)->type : (*rhs)->type;
    } else if ((*lhs)->type->kind == T_POINTER && (*rhs)->type->kind == T_INT) {
        if ((*rhs)->type != type_i64) *rhs = cast_node(nm, (*rhs), type_i64);
        return (*lhs)->type;
    } else if ((*lhs)->type->kind == T_INT && (*rhs)->type->kind == T_POINTER) {
        if ((*lhs)->type != type_i64) *lhs = cast_node(nm, (*lhs), type_i64);
        return (*rhs)->type;
    } else if ((*lhs)->type->kind == T_INT && (*rhs)->type->kind == T_INT) {
        common = (*lhs)->type->size >= (*rhs)->type->size ? (*lhs)->type : (*rhs)->type;
    } else if ((*lhs)->type->kind == T_POINTER && (*rhs)->type->kind == T_POINTER) {
        return type_i64;
    } else {
        do { log_message(LOG_ERROR, "UNSURE HOW TO HANDLE COMMON CASE;\n"); exit(1); } while (0);
    }
    if ((*lhs)->type != common) *lhs = cast_node(nm, (*lhs), common);
    if ((*rhs)->type != common) *rhs = cast_node(nm, (*rhs), common);
    return common;
}
void lower_compound_literal(SemanticContext *sema_ctx, Parser *p, NodeManager *nm, Node *node) {
    Node *ident = new_node(nm, N_IDENTIFIER);
    ident->identifier.name = "__tmp_cl";
    ident->identifier.len = 9;
    ident->type = node->type;
    Node *d = new_node(nm, N_VAR_DECL);
    d->type = node->type;
    d->var_decl.expr = node->compound_literal.value;
    d->var_decl.is_defined = 1;
    d->var_decl.storage_class = STATIC;
    d->var_decl.is_global = 0;
    d->var_decl.identifier = ident;
    ident->identifier.symbol = p_append_var_decl_symbol(p, d);
    d->var_decl.symbol = ident->identifier.symbol;
    *node = *ident;
    insert_node(&sema_current_compound(sema_ctx)->compound.items_array, &d, *get_i(sema_ctx));
    (*get_i(sema_ctx))++;
}
void handle_builtin_call(BuiltinKind kind, Node *node) {
    Node builtin;
    builtin.kind = N_BUILTIN;
    builtin._builtin.kind = kind;
    builtin._builtin.params = node->func_call.params_array;
    memcpy(node, &builtin, sizeof(Node));
}
Type *resolve_type(SemanticContext *sema_ctx, Parser *p, NodeManager *nm, Type *t) {
    if (t->is_resolved) return t;
    t->is_resolved = 1;
    switch (t->kind) {
    case T_POINTER:
        Type *new_pt = get_pointer_type(resolve_type(sema_ctx, p, nm, t->base));
        new_pt->is_resolved = 1;
        return new_pt;
    case T_ARRAY:
        Type *base = resolve_type(sema_ctx, p, nm, t->base);
        semantic_analysis(sema_ctx, p, nm, t->_array.const_expr);
        Type *new_at = get_array_type(base, t->_array.const_expr ? evaluate_const_expression(t->_array.const_expr).i : -1);
        new_at->is_resolved = 1;
        return new_at;
    case T_UNION:
        t->size = 0;
        t->align = 0;
        for (int i = 0; i < t->_union.members_array.count; i++) {
            UnionMember *m = get_union_member(t, i);
            m->type = resolve_type(sema_ctx, p, nm, m->type);
            m->offset = 0;
            if (m->type->size > t->size) {
                t->size = m->type->size;
                t->align = m->type->align;
            }
        }
        if (!(t->size > 0)) do { log_message(LOG_ERROR, "Union size resolve failed\n"); exit(1); } while (0);
        return t;
    case T_STRUCT:
        if (t->_struct.name && strcmp(t->_struct.name, "Type") == 0) {
            printf("Here\n");
        }
        t->size = 0;
        t->align = 0;
        for (int i = 0; i < t->_struct.members_array.count; i++) {
            StructMember *m = get_struct_member(t, i);
            m->type = resolve_type(sema_ctx, p, nm, m->type);
            if (m->type->align > t->align) t->align = m->type->align;
            t->size = align(t->size, m->type->align);
            m->offset = t->size;
            t->size += m->type->size;
        }
        t->size = align(t->size, t->align);
        return t;
    case T_ENUM:
        int64_t value = 0;
        for (int i = 0; i < t->_enum.fields_array.count; i++) {
            EnumField *f = get_enum_field(t, i);
            if (f->const_expr) {
                semantic_analysis(sema_ctx, p, nm, f->const_expr);
                value = evaluate_const_expression(f->const_expr).i;
            }
            f->value = value++;
            p_append_enum_const(p, f);
        }
        t->is_resolved = 1;
        return t;
    case T_FUNCTION:
    case T_VOID:
    case T_INT:
    case T_FLOAT:
        t->is_resolved = 1;
        return t;
    case T_INVALID:
        do { log_message(LOG_ERROR, "Trying to resolve invalid type\n"); exit(1); } while (0);
    }
}
void semantic_analysis(SemanticContext *sema_ctx, Parser *p, NodeManager *nm, Node *node) {
    if (!node) return;
    switch (node->kind) {
    case N_TRANSLATION_UNIT:
        for (int i = 0; i < node->translation_unit.declarations_array.count; i++) {
            semantic_analysis(sema_ctx, p, nm, get_node(&node->translation_unit.declarations_array, i));
        }
        node->type = type_void;
        break;
    case N_FUNCTION:
        if (!(!(node->func.is_defined && node->func.storage_class == EXTERN))) do { log_message(LOG_ERROR, "External Function cannot have a definition\n"); exit(1); } while (0);
        Symbol *func_symbol = p_get_symbol(p, node->func.name, FUNC, 0);
        if (func_symbol) {
            if (func_symbol->func_def->func.storage_class == STATIC && node->func.storage_class != STATIC) {
                do { log_message(LOG_ERROR, "Linkage conflict between function declarations of %s\n", node->func.name); exit(1); } while (0);
            }
            if (!func_symbol->func_def->func.is_defined && node->func.is_defined) {
                func_symbol->func_def = node;
            } else if (node->func.is_defined && func_symbol->func_def->func.is_defined) {
                do { log_message(LOG_ERROR, "Redefinition of function %s\n", node->func.name); exit(1); } while (0);
            }
            node->func.symbol = func_symbol;
        } else node->func.symbol = p_append_func_def(p, node);
        if (node->func.is_defined) {
            p_push_scope(p);
            sema_ctx->func = node;
            for (int i = 0; i < node->type->_func.params.count; i++) {
                ParamDecl *param = (ParamDecl *)get(&node->type->_func.params, i);
                if (node->func.is_defined) {
                    if (!(param->name)) do { log_message(LOG_ERROR, "All Defined Function Paramaters must be named\n"); exit(1); } while (0);
                    Node *param_decl = new_node(nm, N_VAR_DECL);
                    Node *ident = new_node(nm, N_IDENTIFIER);
                    ident->identifier.name = param->name;
                    param_decl->var_decl.identifier = ident;
                    param->symbol = p_append_var_decl_symbol(p, param_decl);
                    param_decl->var_decl.symbol = param->symbol;
                    param_decl->type = param->type;
                }
            }
            semantic_analysis(sema_ctx, p, nm, node->func.body);
            p_pop_scope(p);
        }
        break;
    case N_COMPOUND:
        push_sema_scope(sema_ctx, p, node);
        int n_nodes = node->compound.items_array.count;
        for (int i = 0; i < n_nodes; i++) {
            semantic_analysis(sema_ctx, p, nm, get_node(&node->compound.items_array, *get_i(sema_ctx)));
            (*get_i(sema_ctx))++;
        }
        pop_sema_scope(sema_ctx, p);
        break;
    case N_VAR_DECL:
        if (!node->type->is_resolved) node->type = resolve_type(sema_ctx, p, nm, node->type);
        if (node->var_decl.storage_class == EXTERN) {
            if (node->var_decl.is_defined) {
                do { log_message(LOG_ERROR, "External variable cannot be initialized in the same statement\n"); exit(1); } while (0);
            }
        }
        Symbol *var_symbol = p_get_symbol(p, node->var_decl.identifier->identifier.name, VAR, 1);
        if (var_symbol) {
            if (node->var_decl.is_defined && var_symbol->linkage == LINK_EXTERNAL) update_linkage_storage(var_symbol, node);
            if (p->scopes_array.count > 1) {
                if (var_symbol->scope_depth == p->current_scope_depth) {
                    do { log_message(LOG_ERROR, "Redeclaration of local variable %s\n", node->var_decl.identifier->identifier.name); exit(1); } while (0);
                }
            } else if (!var_symbol->var_decl->var_decl.is_defined && node->var_decl.is_defined) {
                var_symbol->var_decl = node;
            } else if (var_symbol->var_decl->var_decl.is_defined && node->var_decl.is_defined) {
                do { log_message(LOG_ERROR, "Redefinition of global variable %s\n", node->var_decl.identifier->identifier.name); exit(1); } while (0);
            }
        } else var_symbol = p_append_var_decl_symbol(p, node);
        node->var_decl.symbol = var_symbol;
        node->var_decl.identifier->identifier.symbol = var_symbol;
        if (!node->var_decl.expr) break;
        if (node->var_decl.expr->kind == N_INIT_LIST) {
            node->var_decl.expr->type = node->type;
            semantic_analysis(sema_ctx, p, nm, node->var_decl.expr);
            if (node->var_decl.is_global) {
                ConstLiteral init_list = evaluate_const_expression(node->var_decl.expr);
                node->var_decl.const_expr = malloc(sizeof(ConstLiteral));
                if (!(node->var_decl.const_expr)) do { log_message(LOG_ERROR, "Failed to allocate for const expr"); exit(1); } while (0);
                *node->var_decl.const_expr = init_list;
            }
            Node *init_list = node->var_decl.expr;
            break;
        }
        semantic_analysis(sema_ctx, p, nm, node->var_decl.expr);
        if (node->var_decl.expr->kind == N_LITERAL && node->var_decl.expr->literal.kind == L_STRING) {
            if (node->var_decl.expr->literal.kind == L_STRING) {
                if (!(node->type->kind == T_ARRAY || node->type->kind == T_POINTER) && node->type->base == type_i8) {
                    log_start(LOG_ERROR);
                    printf("Cannot initialize ");
                    print_type(node->type);
                    printf(" with String Literal\n");
                    exit(1);
                }
                if (node->type->kind == T_ARRAY && node->type->_array.array_len == -1) {
                    node->type = node->var_decl.expr->type;
                    node->var_decl.symbol->type = node->type;
                }
            }
        }
        if (node->var_decl.expr->type != node->type) {
            node->var_decl.expr = cast_node(nm, node->var_decl.expr, node->type);
        }
        if (node->var_decl.is_global) {
            ConstLiteral val = evaluate_const_expression(node->var_decl.expr);
            node->var_decl.const_expr = malloc(sizeof(ConstLiteral));
            if (!(node->var_decl.const_expr)) do { log_message(LOG_ERROR, "Failed to allocate for const expr"); exit(1); } while (0);
            *node->var_decl.const_expr = val;
        }
        break;
    case N_UNARY:
        semantic_analysis(sema_ctx, p, nm, node->unary.expr);
        node->type = check_unary_op(nm, node);
        break;
    case N_BINARY:
        semantic_analysis(sema_ctx, p, nm, node->binary.lhs);
        semantic_analysis(sema_ctx, p, nm, node->binary.rhs);
        node->binary.common_type = check_binary_op(nm, node->binary.op, node);
        node->type = is_comparison_op(node->binary.op) ? type_i32 : node->binary.common_type;
        break;
    case N_TERNARY:
        semantic_analysis(sema_ctx, p, nm, node->ternary.cond);
        semantic_analysis(sema_ctx, p, nm, node->ternary.if_true);
        semantic_analysis(sema_ctx, p, nm, node->ternary.if_false);
        node->type = node->ternary.if_true->type;
        break;
    case N_CAST:
        semantic_analysis(sema_ctx, p, nm, node->cast.expr);
        if (is_valid_cast(node->cast.expr->type, node->cast.to)) {
            node->cast.from = node->cast.expr->type;
            node->type = node->cast.to;
            break;
        }
        do { log_message(LOG_ERROR, "Semantically invalid cast from %t to %t\n", node->cast.expr->type, node->cast.to); exit(1); } while (0);
    case N_FUNCTION_CALL:
        const char *fn_name = node->func_call.callee->kind == N_IDENTIFIER ? node->func_call.callee->identifier.name : "";
        BuiltinKind builtin = get_builtin_kind(fn_name);
        if (builtin != BUILTIN_NONE) {
            handle_builtin_call(builtin, node);
            return semantic_analysis(sema_ctx, p, nm, node);
        }
        semantic_analysis(sema_ctx, p, nm, node->func_call.callee);
        Type *callee_type = node->func_call.callee->type;
        if (!(callee_type->kind == T_FUNCTION || (callee_type->kind == T_POINTER && callee_type->base->kind == T_FUNCTION))) do { log_message(LOG_ERROR, "Cannot call non function type\n"); exit(1); } while (0);
        Type *fn_type = callee_type->kind == T_FUNCTION ? callee_type : callee_type->base;
        if (!fn_type->_func.is_variadic && fn_type->_func.params.count != node->func_call.params_array.count) {
            print_type(fn_type);
            printf("\n");
            do { log_message(LOG_ERROR, "Argument count mismatch: Function %s expects %d found %d\n", fn_name, fn_type->_func.params.count, node->func_call.params_array.count); exit(1); } while (0);
        }
        node->type = fn_type->_func.return_type;
        for (int i = 0; i < node->func_call.params_array.count; i++) {
            Node *fn_call_param = get_node(&node->func_call.params_array, i);
            semantic_analysis(sema_ctx, p, nm, fn_call_param);
            if (fn_call_param->type->kind == T_ARRAY) {
                Node *casted_node = cast_node(nm, fn_call_param, get_pointer_type(fn_call_param->type->base));
                set_node(&node->func_call.params_array, &casted_node, i);
            }
            if (i < fn_type->_func.params.count) {
                ParamDecl *fn_param = get(&fn_type->_func.params, i);
                if (fn_param->type != fn_call_param->type) {
                    Node *casted_node = cast_node(nm, fn_call_param, fn_param->type);
                    set_node(&node->func_call.params_array, &casted_node, i);
                }
            } else {
                if (fn_call_param->type->kind == T_INT && fn_call_param->type->size < type_i32->size) {
                    Node *casted_node = cast_node(nm, fn_call_param, fn_call_param->type->is_signed ? type_i32 : type_u32);
                    set_node(&node->func_call.params_array, &casted_node, i);
                } else if (fn_call_param->type->kind == T_FLOAT && fn_call_param->type->size < type_f64->size) {
                    Node *casted_node = cast_node(nm, fn_call_param, type_f64);
                    set_node(&node->func_call.params_array, &casted_node, i);
                }
            }
        }
        break;
    case N_IDENTIFIER:
        Symbol *ident_symbol = p_get_symbol(p, node->identifier.name, ANY, 0);
        if (!ident_symbol) {
            do { log_message(LOG_ERROR, "Failed to find symbol \"%s\"\n", node->identifier.name); exit(1); } while (0);
        }
        node->identifier.symbol = ident_symbol;
        switch (ident_symbol->kind) {
        case ENUM:
            node->kind = N_LITERAL;
            node->literal.kind = L_INT;
            node->literal.i = (int64_t)ident_symbol->enum_field->value;
            node->type = ident_symbol->enum_field->_enum_t;
            break;
        case VAR:
            node->type = ident_symbol->var_decl->type;
            break;
        case TYPEDEF:
            node->type = ident_symbol->_typedef.type;
            break;
        case FUNC:
            node->type = ident_symbol->func_def->type;
            break;
        case ANY:
            do { log_message(LOG_ERROR, "Should be unreachable\n"); exit(1); } while (0);
        }
        break;
    case N_IF:
        p_push_scope(p);
        semantic_analysis(sema_ctx, p, nm, node->_if.cond);
        if (node->_if.cond->type != type_i32) {
            node->_if.cond = cast_node(nm, node->_if.cond, type_i32);
        }
        semantic_analysis(sema_ctx, p, nm, node->_if.if_true);
        semantic_analysis(sema_ctx, p, nm, node->_if.if_false);
        p_pop_scope(p);
        break;
    case N_WHILE:
        p_push_scope(p);
        push_sema_loop(sema_ctx, node);
        semantic_analysis(sema_ctx, p, nm, node->_while.cond);
        if (node->_while.cond->type != type_i32) {
            node->_while.cond = cast_node(nm, node->_while.cond, type_i32);
        }
        semantic_analysis(sema_ctx, p, nm, node->_while.block);
        pop_sema_loop(sema_ctx);
        p_pop_scope(p);
        break;
    case N_FOR:
        p_push_scope(p);
        push_sema_loop(sema_ctx, node);
        semantic_analysis(sema_ctx, p, nm, node->_for.init);
        semantic_analysis(sema_ctx, p, nm, node->_for.cond);
        if (node->_for.cond && node->_for.cond->type != type_i32) {
            node->_for.cond = cast_node(nm, node->_for.cond, type_i32);
        }
        semantic_analysis(sema_ctx, p, nm, node->_for.iter);
        semantic_analysis(sema_ctx, p, nm, node->_for.block);
        pop_sema_loop(sema_ctx);
        p_pop_scope(p);
        break;
    case N_RETURN:
        if (!sema_ctx->func) {
            do { log_message(LOG_ERROR, "Cannot call return outside of a function\n"); exit(1); } while (0);
        }
        Type *expected_type = sema_ctx->func->type->_func.return_type;
        if (expected_type != type_void)
            if (!(node->_return.expr)) do { log_message(LOG_ERROR, "Non-void type function '%s' should return a value\n", sema_ctx->func->func.name); exit(1); } while (0);
        if (node->_return.expr) {
            semantic_analysis(sema_ctx, p, nm, node->_return.expr);
            Type *return_type = node->_return.expr->type;
            if (node->_return.expr->type != expected_type) {
                node->_return.expr = cast_node(nm, node->_return.expr, expected_type);
            }
        }
        node->type = expected_type;
        break;
    case N_LITERAL:
        if (node->type->kind == T_ENUM) break;
        char *data = malloc(node->literal.len + 1);
        if (!data) {
            do { log_message(LOG_ERROR, "Failed to allocate for sema literal analysis\n"); exit(1); } while (0);
        }
        memcpy(data, node->literal.raw_rata, node->literal.len);
        data[node->literal.len] = '\0';
        switch (node->literal.kind) {
        case L_INT:
            node->type = parse_int_suffix(node->literal.raw_rata, &node->literal.len);
            node->literal.i = parse_int(data, node->literal.len);
            free(data);
            break;
        case L_FLOAT:
            node->type = parse_float_suffix(node->literal.raw_rata, &node->literal.len);
            node->literal.f = parse_float(data, node->literal.len);
            free(data);
            break;
        case L_CHAR:
            node->type = type_u32;
            node->literal.i = parse_multi_character(data, node->literal.len);
            free(data);
            break;
        case L_STRING:
            node->type = get_array_type(type_i8, node->literal.len + 1);
            node->literal.s.data = data;
            node->literal.s.len = node->literal.len + 1;
            break;
        }
        break;
    case N_INDEX:
        semantic_analysis(sema_ctx, p, nm, node->index.index);
        semantic_analysis(sema_ctx, p, nm, node->index.identifier);
        if (node->index.index->type != type_i64) {
            node->index.index = cast_node(nm, node->index.index, type_i64);
        }
        if (node->index.identifier->type->kind != T_POINTER) {
            Type *pointer_type = get_pointer_type(node->index.identifier->type->base);
            node->index.identifier = cast_node(nm, node->index.identifier, pointer_type);
        }
        node->type = node->index.identifier->type->base;
        break;
    case N_CONTINUE:
        if (!(sema_ctx->loop_stack.count > 0)) do { log_message(LOG_ERROR, "Cannot call continue outside of loop scope\n"); exit(1); } while (0);
        node->_continue.loop = sema_current_loop(sema_ctx);
        break;
    case N_BREAK:
        if (!(sema_ctx->loop_stack.count > 0)) do { log_message(LOG_ERROR, "Cannot call break outside of loop scope\n"); exit(1); } while (0);
        node->_break.loop = sema_current_loop(sema_ctx);
        break;
    case N_INIT_LIST:
        if (node->type == type_invalid) {
            do { log_message(LOG_ERROR, "Semantic Analysis recieved an untyped initializer list\n"); exit(1); } while (0);
        }
        switch (node->type->kind) {
        case T_INT:
        case T_FLOAT:
        case T_POINTER:
        case T_UNION:
            if (node->init_list.elements_array.count == 0) break;
            if (node->init_list.elements_array.count > 1) {
                log_start(LOG_ERROR);
                printf("Excess elements in initializer list for ");
                print_type(node->type);
                printf("\n");
                exit(1);
            }
            Node *e = get_node(&node->init_list.elements_array, 0);
            Type *target_type = node->type->kind == T_UNION ? get_union_member(node->type, 0)->type : node->type;
            Node *value = e;
            if (e->kind == N_DESIGNATED_INITIALIZER) {
                if (node->type->kind != T_UNION) {
                    log_start(LOG_ERROR);
                    printf("Cannot use designated initializers for type ");
                    print_type(node->type);
                    printf("\n");
                    exit(1);
                }
                UnionMember *member = get_union_member_named(node->type, e->designated_init._union.name);
                target_type = member->type;
                e->type = target_type;
                e->designated_init._union.member = member;
                value = e->designated_init.value;
            }
            semantic_analysis(sema_ctx, p, nm, value);
            if (value->type != target_type) {
                Node *casted_node = cast_node(nm, value, target_type);
                set_node(&node->init_list.elements_array, &casted_node, 0);
            }
            break;
        case T_ARRAY:
        case T_STRUCT:
            int index = 0;
            int is_array = node->type->kind == T_ARRAY;
            int max_count = 0;
            int infered_length = 0;
            if (is_array) {
                if (node->type->_array.array_len == -1) {
                    if (!node || node->init_list.elements_array.count < 1) {
                        do { log_message(LOG_ERROR, "Inferred array must be initialized, and cannot be empty.\n"); exit(1); } while (0);
                    }
                } else max_count = node->type->_array.array_len;
            } else max_count = node->type->_struct.members_array.count;
            for (int i = 0; i < node->init_list.elements_array.count; i++) {
                Node *e = get_node(&node->init_list.elements_array, i);
                int is_designator = e->kind == N_DESIGNATED_INITIALIZER;
                if (max_count && index >= max_count && !is_designator) do { log_message(LOG_ERROR, "Too many initializers for %d\n", node->type); exit(1); } while (0);
                StructMember *member = ((void *)0);
                if (is_array) {
                    if (is_designator) {
                        if (!e->designated_init._array.is_complete) {
                            semantic_analysis(sema_ctx, p, nm, e->designated_init._array.const_expr);
                            e->designated_init._array.index = evaluate_const_expression(e->designated_init._array.const_expr).i;
                        }
                        index = e->designated_init._array.index;
                        if (node->type->_array.array_len == -1) {
                            infered_length = infered_length > index + 1 ? infered_length : index + 1;
                        }
                    }
                } else
                    member = is_designator ? get_struct_member_named(node->type, e->designated_init._struct.name, &index)
                                           : get_struct_member(node->type, index);
                Type *target_type = is_array ? node->type->base : member->type;
                Node *value = is_designator ? e->designated_init.value : e;
                if (value->kind == N_INIT_LIST) value->type = target_type;
                semantic_analysis(sema_ctx, p, nm, value);
                if (!is_designator) {
                    Node *de = new_node(nm, N_DESIGNATED_INITIALIZER);
                    if (is_array) {
                        de->designated_init._array.index = index;
                    } else de->designated_init._struct.name = member->name;
                    de->designated_init.value = e;
                    set_node(&node->init_list.elements_array, &de, i);
                    e = de;
                }
                e->designated_init.kind = is_array ? T_ARRAY : T_STRUCT;
                e->type = target_type;
                if (is_array) e->designated_init._array.index = index;
                else e->designated_init._struct.member = member;
                if (value->type != target_type) e->designated_init.value = cast_node(nm, value, target_type);
                index++;
            }
            if (is_array && node->type->_array.array_len == -1)
                node->type = infer_array_length(node->type, infered_length ? infered_length : node->init_list.elements_array.count);
            break;
        default:
            log_start(LOG_ERROR);
            printf("Tried to assign initializer list to unsupported type ");
            print_type(node->type);
            printf("\n");
            exit(1);
        }
        break;
    case N_MEMBER_ACCESS:
        semantic_analysis(sema_ctx, p, nm, node->member_access.identifier);
        Type *lhs_t = node->member_access.identifier->type;
        if (node->member_access.op == TK_ARROW) {
            if (lhs_t->kind != T_POINTER) {
                do { log_message(LOG_ERROR, "Dereference '->' can only be used on pointers\n"); exit(1); } while (0);
            }
            lhs_t = lhs_t->base;
            Node *deref = new_node(nm, N_UNARY);
            deref->unary.op = TK_MULTIPLY;
            deref->unary.associativity = 0;
            deref->unary.expr = node->member_access.identifier;
            deref->type = lhs_t;
            node->member_access.identifier = deref;
            node->member_access.op = TK_DOT;
        }
        int nested_offset = 0;
        AggrMember *member_f = get_member(lhs_t, node->member_access.member->identifier.name, 1, &nested_offset);
        node->member_access.member->type = member_f->type;
        node->member_access.offset = member_f->offset + nested_offset;
        node->type = member_f->type;
        break;
    case N_SWITCH:
        p_push_scope(p);
        push_sema_loop(sema_ctx, node);
        semantic_analysis(sema_ctx, p, nm, node->_switch.test);
        if (node->_switch.test->type != type_i32) node->_switch.test = cast_node(nm, node->_switch.test, type_i32);
        semantic_analysis(sema_ctx, p, nm, node->_switch.block);
        node->type = node->_switch.test->type;
        pop_sema_loop(sema_ctx);
        p_pop_scope(p);
        break;
    case N_CASE:
        semantic_analysis(sema_ctx, p, nm, node->_case.const_expr);
        if (!node->_case.const_expr) break;
        if (!(node->_case.const_expr->type->kind == T_INT || node->_case.const_expr->type->kind == T_ENUM)) {
            do { log_message(LOG_ERROR, "Not ready to handle non int test cases\n"); exit(1); } while (0);
        }
        node->_case.test = evaluate_const_expression(node->_case.const_expr).i;
        break;
    case N_COMPOUND_LITERAL:
        node->compound_literal.value->type = node->type;
        semantic_analysis(sema_ctx, p, nm, node->compound_literal.value);
        lower_compound_literal(sema_ctx, p, nm, node);
        break;
    case N_TYPEDEF:
    case N_TYPE:
        if (!node->type->is_resolved) node->type = resolve_type(sema_ctx, p, nm, node->type);
        break;
    case N_BUILTIN:
        switch (node->_builtin.kind) {
        case BUILTIN_VA_START:
            if (!(node->_builtin.params.count == 2)) do { log_message(LOG_ERROR, "%s expects 2 arguments\n", builtin_names[node->_builtin.kind]); exit(1); } while (0);
            Node *dst_ap = get_node(&node->_builtin.params, 0);
            Node *last_named_param = get_node(&node->_builtin.params, 1);
            semantic_analysis(sema_ctx, p, nm, dst_ap);
            semantic_analysis(sema_ctx, p, nm, last_named_param);
            if (!(last_named_param->kind == N_IDENTIFIER)) do { log_message(LOG_ERROR, "Last named param must be an identifier.\n"); exit(1); } while (0);
            if (!(is_va_list_type(dst_ap->type))) do { log_message(LOG_ERROR, "%s expects va_list as first arg.\n", builtin_names[node->_builtin.kind]); exit(1); } while (0);
            node->type = type_void;
            break;
        case BUILTIN_VA_ARG:
            if (!(node->_builtin.params.count == 2)) do { log_message(LOG_ERROR, "%s expects 2 arguments\n", builtin_names[node->_builtin.kind]); exit(1); } while (0);
            dst_ap = get_node(&node->_builtin.params, 0);
            Node *type_info = get_node(&node->_builtin.params, 1);
            semantic_analysis(sema_ctx, p, nm, dst_ap);
            semantic_analysis(sema_ctx, p, nm, type_info);
            if (!(is_va_list_type(dst_ap->type))) do { log_message(LOG_ERROR, "%s expects va_list as first arg.\n", builtin_names[node->_builtin.kind]); exit(1); } while (0);
            if (!(type_info->kind == N_TYPE && type_info->type != type_invalid)) do { log_message(LOG_ERROR, "%s expects a type as second arg.", builtin_names[node->_builtin.kind]); exit(1); } while (0);
            node->type = type_info->type;
            break;
        case BUILTIN_VA_END:
            if (!(node->_builtin.params.count == 1)) do { log_message(LOG_ERROR, "%s expects 1 arguments\n", builtin_names[node->_builtin.kind]); exit(1); } while (0);
            dst_ap = get_node(&node->_builtin.params, 0);
            semantic_analysis(sema_ctx, p, nm, dst_ap);
            if (!(is_va_list_type(dst_ap->type))) do { log_message(LOG_ERROR, "%s expects va_list as first arg.\n", builtin_names[node->_builtin.kind]); exit(1); } while (0);
            node->type = type_void;
            break;
        case BUILTIN_MEMCPY:
            if (!(node->_builtin.params.count == 3)) do { log_message(LOG_ERROR, "%s expects 3 arguments\n", builtin_names[node->_builtin.kind]); exit(1); } while (0);
            Node *memcpy_dst = get_node(&node->_builtin.params, 0);
            Node *memcpy_src = get_node(&node->_builtin.params, 1);
            Node *memcpy_size = get_node(&node->_builtin.params, 2);
            semantic_analysis(sema_ctx, p, nm, memcpy_dst);
            semantic_analysis(sema_ctx, p, nm, memcpy_src);
            semantic_analysis(sema_ctx, p, nm, memcpy_size);
            if (!(memcpy_dst->type->kind == T_POINTER && memcpy_src->type->kind)) do { log_message(LOG_ERROR, "%s expects both src and dst to be pointers\n", builtin_names[node->_builtin.kind]); exit(1); } while (0);
            if (!(memcpy_size->type->kind == T_INT)) do { log_message(LOG_ERROR, "%s expects size to be integer or size_t\n"); exit(1); } while (0);
            node->type = type_u64;
            break;
        case BUILTIN_NONE:
            do { log_message(LOG_ERROR, "given __builtin_none but BUILTIN?\n"); exit(1); } while (0);
        }
        break;
    case N_NULL:
        break;
    case N_GOTO:
    case N_LABEL:
    case N_DESIGNATED_INITIALIZER:
        do { log_message(LOG_ERROR, "Unreachable\n"); exit(1); } while (0);
    }
}
void push_sema_loop(SemanticContext *sema_ctx, const Node *loop) { append(&sema_ctx->loop_stack, &loop); }
void pop_sema_loop(SemanticContext *sema_ctx) { pop(&sema_ctx->loop_stack); }
Node *sema_current_loop(const SemanticContext *sema_ctx) {
    if (!(sema_ctx->loop_stack.count > 0)) do { log_message(LOG_ERROR, "Tried to retrieve current loop outside of loop scope\n"); exit(1); } while (0);
    return *(Node **)get(&sema_ctx->loop_stack, sema_ctx->loop_stack.count - 1);
}
Node *sema_current_compound(const SemanticContext *sema_ctx) {
    if (!(sema_ctx->compound_stack.count > 0)) do { log_message(LOG_ERROR, "Tried to retrieve current compound outside of compound scope\n"); exit(1); } while (0);
    return *(Node **)get(&sema_ctx->compound_stack, sema_ctx->compound_stack.count - 1);
}
void push_sema_scope(SemanticContext *sema_ctx, Parser *p, Node *n) {
    p_push_scope(p);
    append(&sema_ctx->compound_stack, &n);
    int tmp = 0;
    append(&sema_ctx->i_array, &tmp);
}
void pop_sema_scope(SemanticContext *sema_ctx, Parser *p) {
    pop(&sema_ctx->i_array);
    pop(&sema_ctx->compound_stack);
    p_pop_scope(p);
}
void lower_nodes(NodeManager *nm) {
    for (int i = 0; i < nm->count; i++) {
        Node *n = arena_get(nm, i);
        if (n->type->kind == T_ENUM) {
            n->type = type_i32;
        }
        if (n->kind == N_CAST) {
            if (n->cast.from && n->cast.from->kind == T_ENUM) {
                n->cast.from = type_i32;
            }
            if (n->cast.to && n->cast.to->kind == T_ENUM) {
                n->cast.to = type_i32;
                n->type = type_i32;
            }
            if (n->cast.from == n->type) {
                *n = *n->cast.expr;
            }
        }
    }
}
