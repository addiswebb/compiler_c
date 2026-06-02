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
typedef struct Array {
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
typedef unsigned long long size_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;
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
    CONST_ZERO,
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
        int zero_bytes;
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
AggrMember *get_member(Type *struct_t, const char *name, int is_root, int *offset, int *index);
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
void print_tokens(Tokenizer *tk);
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
char *_strdup(const char *);
char *_strndup(const char *, size_t);
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
    N_DESIGNATOR,
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
        } designator;
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
    IR_MEMSET,
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
            int size;
            int c;
        } memset;
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
typedef struct IR_InitContext {
    Type *type;
    int offset;
    int index;
} IR_InitContext;
typedef struct {
    IR_Module *module;
    IR_Function *func;
    IR_Block *block;
    Array loop_stack_array;
    IR_Block *true_block;
    IR_Block *false_block;
    Arena *symbol_table;
    int func_not_address;
    IR_InitContext init_ctx;
} IR_Context;
extern const IR_Value ir_no_value;
IR_Context ir_init_ctx(Parser *p);
void free_ir_ctx(IR_Context *ctx);
IR_Module *ir_gen_translation_unit(IR_Context *ctx, const Node *tu);
void ir_push_loop_ctx(IR_Context *ctx, IR_Block *continue_block, IR_Block *break_block);
void ir_pop_loop_ctx(IR_Context *ctx);
void ir_append_instruction(IR_Context *ctx, IR_Instruction *instr);
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
void ir_move(IR_Context *ctx, IR_Value dst, IR_Value src);
IR_Value ir_load(IR_Context *ctx, IR_Value addr, Type *type);
IR_Value ir_store(IR_Context *ctx, IR_Value dst, IR_Value src, Type *type);
IR_Value ir_smart_const(IR_Context *ctx, ConstLiteral *literal, Type *type);
IR_Value ir_const(IR_Context *ctx, int const_index, Type *type);
IR_Value ir_unary(IR_Context *ctx, IR_UNARY_OP op, IR_Value expr_reg, Type *type);
IR_Value ir_binary(IR_Context *ctx, IR_BINOP_OP op, IR_Value dst, IR_Value lhs_reg, IR_Value rhs_reg, Type *type);
IR_Value ir_cmp(IR_Context *ctx, IR_CMP_OP op, IR_Value lhs_reg, IR_Value rhs_reg, Type *type);
IR_Value ir_call(IR_Context *ctx, const Node *expr);
IR_Value ir_return(IR_Context *ctx, IR_Value reg, Type *type);
IR_Value ir_branch(IR_Context *ctx, IR_Block *block);
IR_Value ir_branch_cond(IR_Context *ctx, IR_Value cond_reg, IR_Block *t_block, IR_Block *f_block);
IR_Value ir_label(IR_Context *ctx, const char *name);
IR_Value ir_jmp(IR_Context *ctx, const char *name);
IR_Value ir_cast(IR_Context *ctx, IR_Value src, Type *to, Type *from);
IR_Value ir_address(IR_Context *ctx, IR_Value src, int offset);
IR_Value ir_alloca(IR_Context *ctx, IR_Value dst, int size, int al);
IR_Value ir_memset(IR_Context *ctx, IR_Value dst, int c, int size);
IR_Value ir_memcpy(IR_Context *ctx, IR_Value from_reg, IR_Value to_reg, int size);
void ir_zero(IR_Context *ctx, IR_Value dst, Type *type);
IR_Value ir_builtin_va_start(IR_Context *ctx, IR_Value ap, IR_Value last_named_param);
IR_Value ir_builtin_va_arg(IR_Context *ctx, IR_Value ap, Type *type);
typedef char *va_list;
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
FILE *_popen(const char *, const char *);
int _pclose(FILE *);
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
void abi_gen_memset_instruction(FILE *fp, const IR_Instruction *instr);
void abi_gen_memcpy_instruction(FILE *fp, const IR_Instruction *instr);
extern Symbol *_hidden_sret_ptr;
Symbol *current_sret();
void set_sret(Type *return_type);
void set_hidden_sret_ptr(Type *return_type);
extern const GP_Reg int_param_regs[4];
extern const XMM_Reg float_param_regs[4];
extern const GP_Reg caller_saved_regs[7];
extern const GP_Reg callee_saved_regs[8];
extern const char *gp_register_str[16][4];
extern const char *sse_register_str[16];
IR_Value ir_gen_lvalue(IR_Context *ctx, const Node *expr);
IR_Value ir_gen_rvalue(IR_Context *ctx, const Node *expr);
static void ir_gen_block_item(IR_Context *ctx, const Node *item);
static void ir_gen_compound(IR_Context *ctx, const Node *comp);
static void ir_gen_while_loop(IR_Context *ctx, const Node *_while);
static void ir_gen_for_loop(IR_Context *ctx, const Node *_for);
static void ir_gen_if_statement(IR_Context *ctx, const Node *_if);
static void ir_gen_var_decl(IR_Context *ctx, const Node *var_decl);
static void ir_gen_statement(IR_Context *ctx, const Node *stmt);
static void ir_gen_return(IR_Context *ctx, const Node *_return);
static void ir_gen_goto(IR_Context *ctx, const Node *_goto);
static void ir_gen_label(IR_Context *ctx, const Node *label);
static IR_Function *ir_gen_function(IR_Context *ctx, const Node *func);
int ir_is_within_cond(IR_Context *ctx);
void ir_set_cond_block(IR_Context *ctx, IR_Block *true_block, IR_Block *false_block);
void ir_reset_cond_block(IR_Context *ctx);
IR_CMP_OP ir_cmp_op(const TokenType type);
IR_UNARY_OP ir_unary_op(const TokenType type);
IR_BINOP_OP ir_binary_op(const TokenType type);
void print_ir_module(const IR_Context *ctx,const IR_Module *module);
void print_ir_value(const IR_Value *v);
void print_ir_phys_reg(const PhysReg *r);
static void print_ir_block(const IR_Context *ctx, const IR_Block *block);
void print_ir_function(const IR_Context *ctx, const IR_Function *func);
void print_ir_instruction(const IR_Context *ctx,const IR_Instruction *instr);
static void print_unary_op(IR_UNARY_OP op);
static void print_binary_op(IR_BINOP_OP op);
static void print_cmp_op(IR_CMP_OP op);
void exit(int);
void *malloc(size_t);
void *realloc(void *, size_t);
void *calloc(size_t, size_t);
void free(void *);
int system(const char *);
typedef int (*__compar_fn_t)(const void *, const void *);
extern void qsort(void *__base, size_t __nmemb, size_t __size, __compar_fn_t __compar);
typedef enum {
    LOG_INFO,
    LOG_DEBUG,
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
static FILE *stdin = ((void *)0);
static FILE *stdout = ((void *)0);
static FILE *stderr = ((void *)0);
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
    __builtin_va_start(args, fmt);
    vprint(fmt, args);
    __builtin_va_end(args);
    fflush(logger.file);
    if (lvl == LOG_PANIC) exit(1);
}
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
        do { log_message(LOG_ERROR, "Tried to zero invalid type %t\n", type); exit(1); } while (0);
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
    ctx->func_not_address = 1;
    i.ops[1] = ir_gen_rvalue(ctx, expr->func_call.callee);
    ctx->func_not_address = 0;
    i.call.type = expr->func_call.callee->type;
    if (i.call.type->kind == T_POINTER) i.call.type = i.call.type->base;
    if (!(i.call.type->abi.type)) do { log_message(LOG_ERROR, "Function Type did not recieve ABI type\n"); exit(1); } while (0);
    array_init(&i.call.arg_array, expr->func_call.params_array.count ? expr->func_call.params_array.count : 1, sizeof(IR_CallArg));
    int hidden_ptr_offset = 0;
    Type *return_type = i.call.type->_func.return_type;
    IR_Value sret;
    int use_sret = 0;
    if (return_type != type_void) {
        ABI_Result res = abi_classify(return_type);
        if (res.memory) {
            set_sret(return_type);
            Symbol *current_sret_symbol = current_sret();
            append(&ctx->func->locals_array, &current_sret_symbol);
            hidden_ptr_offset++;
            sret = ((IR_CallArg *)append(&i.call.arg_array, &(IR_CallArg){.v = ir_address(ctx, ir_symbol_value(current_sret_symbol), 0),
                                                                          .type = get_pointer_type(return_type)}))
                       ->v;
            use_sret = 1;
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
        if (res.memory) val = ir_gen_lvalue(ctx, param);
        else if (param->type->kind == T_FUNCTION || is_func_ptr(param->type)) val = ir_gen_lvalue(ctx, param);
        else val = ir_gen_rvalue(ctx, param);
        append(&i.call.arg_array, &(IR_CallArg){.v = val, .type = arg_type});
    }
    i.ops[0] = ir_next_virtual_reg(ctx->func);
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
    if (from->kind == T_ARRAY && to->kind == T_POINTER && from->base->kind == to->base->kind) do { log_message(LOG_ERROR, "HOW"); exit(1); } while (0);
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
