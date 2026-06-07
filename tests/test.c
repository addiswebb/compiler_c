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
void arena_set(const Arena *arena, const void *element, int index);
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
static int align(const int size, const int align) { return (size + align - 1) & ~(align - 1); }
void init_typepool();
void free_typepool();
Type *init_global_type(TypeKind type, int size, unsigned int qualifiers, int is_signed);
Type *new_type();
Type *new_pointer_type(Type *type);
Type *new_qualified_type(const Type *type, unsigned int qualifiers);
Type *new_unsigned_type(Type *type);
Type *new_array_type(Type *type, int len);
Type *new_incomplete_array_type(Type *type, Node *const_expr);
Type *get_float_type(int size);
Type *get_integer_type(int size);
Type *get_pointer_type(Type *type);
Type *get_array_type(Type *type, int len);
Type *get_qualified_type(const Type *type, unsigned int qualifiers);
Type *get_function_type(Type *type, Array params, int is_variadic);
Type *get_modified_type(Type *type, const Declarator *decl);
Type *get_unsigned_type(Type *type);
Type *get_enum_type(const char *name);
Type *get_union_type(const char *name);
Type *get_struct_type(const char *name);
int cmp_func_types(const Type *a, const Type *b);
Type *infer_array_length(Type *arr_type, int inferred_len);
void append_union_member(Type *u, const UnionMember *m);
void append_struct_member(Type *s, StructMember *f);
void append_enum_field(Type *e, EnumField *f);
Type union_type();
Type struct_type();
Type enum_type();
AggrMember *get_member(const Type *struct_t, const char *name, int is_root, int *offset, int *index);
int is_func_ptr(const Type *t);
int is_scalar_type(const Type *t);
void print_type(Type *type);
void print_param_decl(const ParamDecl *decl);
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
int is_postfix_operator(TokenType type);
int is_unary_operator(TokenType type);
int is_binary_operator(TokenType type);
int is_assignment_op(TokenType type);
int is_arithmetic_op(TokenType type);
int is_bitwise_op(TokenType type);
int is_comparison_op(TokenType type);
int is_logical_op(TokenType type);
TokenType get_underlying_op(TokenType type);
int op_associativity(TokenType type);
int op_precedence(TokenType type);
const char *token_type_str(TokenType type);
void print_token_type(TokenType type);
void print_token(const Token *token);
void print_tokens(Tokenizer *tk);
static Token *get_token(const Array *arr, const int index) { return get(arr, index); }
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
BuiltinKind get_builtin_kind(const char *name);
Node *new_node(NodeManager *nm, NodeKind kind);
Node *new_init_translation_unit_node(NodeManager *nm);
Node *new_compound_node(NodeManager *nm);
Node *new_init_list_node(NodeManager *nm);
Node *new_function_node(NodeManager *nm);
Node *new_function_call_node(NodeManager *nm, Node *identifier);
void free_node(Node *node);
Node *cast_node(NodeManager *nm, Node *node, Type *type);
int is_valid_cast(const Type *from, const Type *to);
LiteralKind literal_kind(TokenType type);
void print_node_type(NodeKind type);
void print_indent(int depth);
void print_node(const Node *node, int depth);
void print_ast(const NodeManager *nm);
static Node *get_node(const Array *node_array, const int index) { return *(Node **)get(node_array, index); }
static void set_node(const Array *node_array, Node **node, const int index) {
    memcpy((char *)node_array->data + index * node_array->element_size, node, sizeof(Node *));
}
static Node *insert_node(Array *node_array, Node **node, const int index) { return insert(node_array, node, index); }
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
int is_storage_classifier(TokenType type);
int is_qualifier_token(TokenType type);
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
Node *p_parse_binary(Parser *p, NodeManager *nm, Node *lhs);
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
Node *p_parse_function(Parser *p, NodeManager *nm, Type *type, const char *name, StorageClass storage_class, int is_inline);
Modifier p_parse_parameter_list(Parser *p, NodeManager *nm);
Node *p_parse_external_declaration(Parser *p, NodeManager *nm);
Node *p_parse_block_declaration(Parser *p, NodeManager *nm);
Node *p_parse_decl_identifier(Parser *p, NodeManager *nm);
Node *p_parse_declaration(Parser *p, NodeManager *nm, Type *type, const char *name, StorageClass storage_class, int global);
Node *p_parse_typedef(Parser *p, NodeManager *nm);
Node *p_parse_translation_unit(Parser *p, NodeManager *nm);
Type *p_parse_type(Parser *p, NodeManager *nm, const char **name);
Type *p_parse_abstract_type(Parser *p, NodeManager *nm);
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
Symbol *p_get_symbol(const Parser *p, const char *name, SymbolKind kind, int same_depth);
Typedef *p_get_typedef(const Parser *p, const char *name);
Node *p_get_var_decl(const Parser *p, const char *name);
const EnumField *p_get_enum_const(const Parser *p, const char *name);
Symbol *p_new_symbol(Parser *p, const Symbol *s);
void p_append_call_param(Node *func_call, Node *param);
void p_append_param(const Node *func, Node *param);
void p_append_enum_const(Parser *p, const EnumField *e);
Symbol *p_append_var_decl_symbol(Parser *p, Node *v);
void update_linkage_storage(Symbol *s, const Node *v);
Symbol *p_append_param_decl_symbol(Parser *p, const ParamDecl *param);
void p_append_element(Node *init_list, Node *element);
void p_append_symbol_table(Parser *p);
Symbol *p_append_symbol(Array *st, const Symbol *s);
void p_append_typedef(Parser *p, const Typedef *t);
Symbol *p_append_func_def(Parser *p, Node *f);
void p_append_block_item(Node *root, Node *item);
void p_append_case(Node *s, Node *c);
static Array *get_symbol_table(const Parser *p, const int index) { return get(&p->scopes_array, index); }
static Array *get_current_symbol_table(const Parser *p) { return get_symbol_table(p, p->scopes_array.count - 1); }
static Symbol *get_symbol(const Array *symbol_table, const int index) { return *(Symbol **)get(symbol_table, index); }
static EnumField *get_enum_field(const Type *enum_t, const int index) { return get(&enum_t->_enum.fields_array, index); }
static StructMember *get_struct_member(const Type *struct_t, const int index) { return get(&struct_t->_struct.members_array, index); }
static UnionMember *get_union_member(const Type *union_t, const int index) { return get(&union_t->_union.members_array, index); }
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
IR_Value ir_gp_register_value(GP_Reg reg);
IR_Value ir_gp_register_offset_value(GP_Reg reg, int offset);
IR_Value ir_stack_value(int size, int align, int offset);
void ir_lower_vreg_value(IR_Value *v, const Lifetime *lts, int lts_count);
void ir_lower_symbol_value(IR_Value *v, const Array *symbol_slots, const Array *symbol_map);
void analysis(const IR_Context *ctx);
void gen_abi_func_types();
void symbol_slot_allocation(const IR_Context *ctx, const IR_Function *f, int *frame_size, Array *symbol_slots, Array *symbol_map);
int get_symbol_index(const Array *symbol_map, const Symbol *symbol);
int bitset_add_defined(const BitSet *defined, const IR_Value *v);
void bitset_add_used(const BitSet *defined, const BitSet *used, const IR_Value *v);
int reg_bitset(const IR_Function *f);
void lower_ir_for_asm(IR_Function *f);
void lower_ir_values_to_stack(const IR_Function *f, const Lifetime *lts, const int lts_count, const Array *symbol_slots,
                              const Array *symbol_map);
void add_successor(IR_Function *func, IR_Block *from, IR_Block *to);
Lifetime *compute_lifetimes(const IR_Function *f, const int defined, const int *rpo);
void compute_reverse_postorder(IR_Function *func, int *rpo);
void compute_bitset(const IR_Function *f, const int *rpo);
int cmp_lifetime(const void *a, const void *b);
void linear_stack_slot_allocation(Lifetime *lts, const int count, int *stack_size);
void bitset_init(BitSet *s, int reg_count);
void bitset_expand(BitSet *s);
void bitset_add(const BitSet *s, int reg);
void bitset_remove(const BitSet *s, int reg);
int bitset_has(const BitSet *s, int reg);
void bitset_union(const BitSet *dst, const BitSet *src);
void bitset_intersect(const BitSet *dst, const BitSet *src);
void bitset_difference(const BitSet *dst, const BitSet *src);
void bitset_copy(const BitSet *dst, const BitSet *src);
int bitset_equal(const BitSet *a, const BitSet *b);
void print_bitset(const BitSet *bs);
void print_cfg(const IR_Function *func);
typedef struct {
    unsigned int gp_offset;
    unsigned int fp_offset;
    void *overflow_args;
    void *reg_save_area;
} va_list[1];
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
typedef enum ABI_TypeClass {
    ABI_NO_CLASS,
    ABI_MEMORY,
    ABI_INTEGER,
    ABI_SSE,
} ABI_TypeClass;
typedef struct ABI_Result {
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
ABI_TypeClass merge(ABI_TypeClass chunk_class, ABI_TypeClass field_class);
ABI_Result classify_struct(Type *type);
extern const GP_Reg int_param_regs[6];
extern const XMM_Reg float_param_regs[8];
extern const GP_Reg caller_saved_regs[9];
extern const GP_Reg callee_saved_regs[6];
extern const char *gp_register_str[16][4];
extern const char *sse_register_str[16];
typedef struct {
    char *output;
    Array current_source;
    Array current_output;
    Array source_files;
    Array passthrough_args;
    char *src;
    int src_size;
    NodeManager nm;
    Parser p;
    Tokenizer tk;
} Compiler;
typedef enum {
    CF_STOP_AFTER_AST,
    CF_STOP_AFTER_IR,
    CF_STOP_AFTER_COMPILE,
    CF_STOP_AFTER_ASSEMBLE,
    CF_DEBUG_TYPEPOOL,
    CF_DEBUG_LIFETIMES,
    CF_DEBUG_ENUM,
    CF_DEBUG_STRUCT,
    CF_DEBUG_UNION,
    CF_DEBUG_LOWERED_IR,
    CF_DEBUG_IR_INSTR,
    CF_DEBUG_PARSER,
    CF_DEBUG_TOKENIZER,
    CF_DEBUG_TOKENS,
    CF_DEBUG_SYMBOLS,
    CF_COUNT,
} CompilerFlag;
extern unsigned int compiler_flags;
extern const char *flag_strings[CF_COUNT];
int has_flag(CompilerFlag f);
Compiler begin_compiler(int argc, char *argv[]);
void init_compiler(Compiler *compiler);
void clear_compiler(Compiler *compiler);
void free_compiler(Compiler *compiler);
void drive(Compiler *c);
void assemble(Compiler *c);
void link(const Compiler *c, const Array *objs);
int compile(Compiler *compiler);
static int load_src_file(Compiler *compiler, const char *file);
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
static void init_logger(FILE *fp, const LogLevel level) {
    logger.file = fp ? fp : stderr;
    logger.stage = STAGE_COMPILER;
    logger.min_level = level;
}
static char *stage_str(const LogStage stage) {
    switch (stage) {
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
static void set_log_stage(const LogStage stage) { logger.stage = stage; }
static void log_start(const LogLevel lvl) {
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
static void log_message(const LogLevel lvl, const char *fmt, ...) {
    if (lvl < logger.min_level) return;
    log_start(lvl);
    va_list args;
    __builtin_va_start(args, fmt);
    vprint(fmt, args);
    __builtin_va_end(args);
    fflush(logger.file);
    if (lvl == LOG_PANIC) exit(1);
}
const char *gp_register_str[16][4] = {
    [RAX] = {"%al", "%ax", "%eax", "%rax"}, [RBX] = {"%bl", "%bx", "%ebx", "%rbx"}, [RCX] = {"%cl", "%cx", "%ecx", "%rcx"},
    [RDX] = {"%dl", "%dx", "%edx", "%rdx"}, [RSI] = {"%sil", "%si", "%esi", "%rsi"}, [RDI] = {"%dil", "%di", "%edi", "%rdi"},
    [RBP] = {"%bpl", "%bp", "%ebp", "%rbp"}, [RSP] = {"%spl", "%sp", "%esp", "%rsp"}, [R8] = {"%r8b", "%r8w", "%r8d", "%r8"},
    [R9] = {"%r9b", "%r9w", "%r9d", "%r9"}, [R10] = {"%r10b", "%r10w", "%r10d", "%r10"}, [R11] = {"%r11b", "%r11w", "%r11d", "%r11"},
    [R12] = {"%r12b", "%r12w", "%r12d", "%r12"}, [R13] = {"%r13b", "%r13w", "%r13d", "%r13"}, [R14] = {"%r14b", "%r14w", "%r14d", "%r14"},
    [R15] = {"%r15b", "%r15w", "%r15d", "%r15"},
};
const char *sse_register_str[16] = {
    [XMM0] = "%xmm0", [XMM1] = "%xmm1", [XMM2] = "%xmm2", [XMM3] = "%xmm3", [XMM4] = "%xmm4", [XMM5] = "%xmm5",
    [XMM6] = "%xmm6", [XMM7] = "%xmm7", [XMM8] = "%xmm8", [XMM9] = "%xmm9", [XMM10] = "%xmm10", [XMM11] = "%xmm11",
    [XMM12] = "%xmm12", [XMM13] = "%xmm13", [XMM14] = "%xmm14", [XMM15] = "%xmm15",
};
void bitset_init(BitSet *s, const int reg_count) {
    s->num_bits = reg_count;
    s->capacity = (s->num_bits + 31) / 32;
    if (reg_count) {
        s->data = calloc(s->capacity, sizeof(int));
        if (!(s->data)) do { log_message(LOG_ERROR, "Failed to alloc for bitset init\n"); exit(1); } while (0);
    } else {
        s->data = ((void *)0);
    }
}
void bitset_clear(const BitSet *s) {
    for (int i = 0; i < s->capacity; i++) {
        s->data[i] = 0;
    }
}
void bitset_add(const BitSet *s, const int reg) {
    if (reg >= s->num_bits) {
        do { log_message(LOG_ERROR, "%d is too large a register for this bitset\n", reg); exit(1); } while (0);
    }
    if (!(reg >= 0)) do { log_message(LOG_ERROR, "Reg must be >= 0\n"); exit(1); } while (0);
    const int word = reg / 32;
    const unsigned int offset = reg % 32;
    s->data[word] |= (1u << offset);
}
void bitset_remove(const BitSet *s, const int reg) {
    if (!(reg >= 0)) do { log_message(LOG_ERROR, "Reg must be >= 0\n"); exit(1); } while (0);
    const int word = reg / 32;
    const unsigned int offset = reg % 32;
    s->data[word] &= ~(1u << offset);
}
int bitset_has(const BitSet *s, const int reg) {
    if (!(reg >= 0)) do { log_message(LOG_ERROR, "Reg must be >= 0\n"); exit(1); } while (0);
    const int word = reg / 32;
    const unsigned int offset = reg % 32;
    return (s->data[word] & (1u << offset)) != 0;
}
void bitset_union(const BitSet *dst, const BitSet *src) {
    if (!(dst->capacity == src->capacity)) do { log_message(LOG_ERROR, "Capacities must be equal\n"); exit(1); } while (0);
    for (int i = 0; i < dst->capacity; i++) dst->data[i] |= src->data[i];
}
void bitset_intersect(const BitSet *dst, const BitSet *src) {
    if (!(dst->capacity == src->capacity)) do { log_message(LOG_ERROR, "Capacities must be equal\n"); exit(1); } while (0);
    for (int i = 0; i < dst->capacity; i++) dst->data[i] &= src->data[i];
}
void bitset_difference(const BitSet *dst, const BitSet *src) {
    if (!(dst->capacity == src->capacity)) do { log_message(LOG_ERROR, "Capacities must be equal\n"); exit(1); } while (0);
    for (int i = 0; i < dst->capacity; i++) dst->data[i] &= ~src->data[i];
}
void bitset_copy(const BitSet *dst, const BitSet *src) {
    if (!(dst->capacity == src->capacity)) do { log_message(LOG_ERROR, "Capacities must be equal\n"); exit(1); } while (0);
    for (int i = 0; i < dst->capacity; i++) dst->data[i] = src->data[i];
}
int bitset_equal(const BitSet *a, const BitSet *b) {
    if (!(a->capacity == b->capacity)) do { log_message(LOG_ERROR, "Capacities must be equal\n"); exit(1); } while (0);
    for (int i = 0; i < a->capacity; i++) {
        if (a->data[i] != b->data[i]) return 0;
    }
    return 1;
}
static int contains(const int *arr, const int count, const int val) {
    for (int i = 0; i < count; i++) {
        if (arr[i] == val) return 1;
    }
    return 0;
}
void add_successor(IR_Function *func, IR_Block *from, IR_Block *to) {
    if (!contains(from->cfg.succ, from->cfg.succ_count, to->id)) {
        from->cfg.succ = realloc(from->cfg.succ, sizeof(int) * (from->cfg.succ_count + 1));
        from->cfg.succ[from->cfg.succ_count++] = to->id;
    }
    if (!contains(to->cfg.pred, to->cfg.pred_count, from->id)) {
        to->cfg.pred = realloc(to->cfg.pred, sizeof(int) * (to->cfg.pred_count + 1));
        to->cfg.pred[to->cfg.pred_count++] = from->id;
    }
}
void dfs_postorder(IR_Function *func, const int block_id, int *visited, int *postorder, int *count) {
    if (visited[block_id]) return;
    visited[block_id] = 1;
    const IR_Block *b = get_block(func, block_id);
    for (int i = 0; i < b->cfg.succ_count; i++) {
        dfs_postorder(func, b->cfg.succ[i], visited, postorder, count);
    }
    postorder[(*count)++] = block_id;
}
void compute_reverse_postorder(IR_Function *func, int *rpo) {
    int *visited = calloc(func->blocks_array.count, sizeof(int));
    if (!visited) {
        do { log_message(LOG_ERROR, "Failed to calloc for visited\n"); exit(1); } while (0);
    }
    int *postorder = malloc(func->blocks_array.count * sizeof(int));
    if (!postorder) {
        free(visited);
        do { log_message(LOG_ERROR, "Failed to calloc for postorder\n"); exit(1); } while (0);
    }
    int count = 0;
    dfs_postorder(func, 0, visited, postorder, &count);
    for (int i = 0; i < count; i++) {
        rpo[i] = postorder[count - 1 - i];
    }
    free(postorder);
    free(visited);
}
int bitset_add_defined(const BitSet *defined, const IR_Value *v) {
    if (v->kind == IR_VREG) {
        if (!bitset_has(defined, v->vreg)) {
            bitset_add(defined, v->vreg);
            return 1;
        }
    }
    return 0;
}
void bitset_add_used(const BitSet *defined, const BitSet *used, const IR_Value *v) {
    if (v->kind == IR_VREG) {
        if (!bitset_has(defined, v->vreg)) {
            bitset_add(used, v->vreg);
        }
    }
}
void ir_init_func_cfg(const IR_Function *f) {
    for (int j = 0; j < f->blocks_array.count; j++) {
        IR_Block *b = get_block(f, j);
        b->cfg.succ = ((void *)0);
        b->cfg.succ_count = 0;
        b->cfg.pred = ((void *)0);
        b->cfg.pred_count = 0;
        bitset_init(&b->live.live_in, f->max_reg);
        bitset_init(&b->live.live_out, f->max_reg);
        bitset_init(&b->live.def, f->max_reg);
        bitset_init(&b->live.use, f->max_reg);
    }
}
void ir_compute_func_io(IR_Function *f) {
    for (int j = 0; j < f->blocks_array.count; j++) {
        IR_Block *b = get_block(f, j);
        int has_terminator = 0;
        for (int i = 0; i < b->instruction_array.count; i++) {
            const IR_Instruction *instr = get_instruction(&b->instruction_array, i);
            switch (instr->op) {
            case IR_BR:
                add_successor(f, b, instr->br.block);
                has_terminator = 1;
                break;
            case IR_BR_COND:
                if (instr->br_cond.f_block) add_successor(f, b, instr->br_cond.f_block);
                if (instr->br_cond.t_block) add_successor(f, b, instr->br_cond.t_block);
                has_terminator = instr->br_cond.f_block && instr->br_cond.t_block;
                break;
            case IR_RET:
                has_terminator = 1;
                break;
            default:
                break;
            }
            if (has_terminator) break;
        }
        if (!has_terminator && j < f->blocks_array.count - 1) add_successor(f, b, get_block(f, j + 1));
    }
}
int reg_bitset(const IR_Function *f) {
    int defined = 0;
    for (int j = 0; j < f->blocks_array.count; j++) {
        const IR_Block *b = get_block(f, j);
        for (int k = 0; k < b->instruction_array.count; k++) {
            const IR_Instruction *instr = get_instruction(&b->instruction_array, k);
            for (int i = 0; i < instr->op_count; i++) {
                if (instr->ops[i].kind != IR_VREG) continue;
                if (op_info[instr->op].def_mask & (1 << i)) {
                    if (bitset_add_defined(&b->live.def, &instr->ops[i])) defined++;
                }
                if (op_info[instr->op].use_mask & (1 << i)) {
                    bitset_add_used(&b->live.def, &b->live.use, &instr->ops[i]);
                }
            }
        }
    }
    return defined;
}
void compute_bitset(const IR_Function *f, const int *rpo) {
    BitSet old_live_out;
    BitSet old_live_in;
    BitSet tmp;
    bitset_init(&old_live_out, f->max_reg);
    bitset_init(&old_live_in, f->max_reg);
    bitset_init(&tmp, f->max_reg);
    int changed = 1;
    while (changed) {
        changed = 0;
        for (int i = 0; i < f->blocks_array.count; i++) {
            const int index = rpo[i];
            if (index == -1) continue;
            const IR_Block *b = get_block(f, index);
            bitset_clear(&old_live_out);
            bitset_clear(&old_live_in);
            bitset_clear(&tmp);
            bitset_copy(&old_live_out, &b->live.live_out);
            bitset_copy(&old_live_in, &b->live.live_in);
            bitset_clear(&b->live.live_out);
            for (int j = 0; j < b->cfg.succ_count; j++) bitset_union(&b->live.live_out, &get_block(f, b->cfg.succ[j])->live.live_in);
            bitset_copy(&tmp, &b->live.live_out);
            bitset_difference(&tmp, &b->live.def);
            bitset_copy(&b->live.live_in, &b->live.use);
            bitset_union(&b->live.live_in, &tmp);
            if (!bitset_equal(&b->live.live_out, &old_live_out) || !bitset_equal(&b->live.live_in, &old_live_in)) changed = 1;
        }
    }
    free(old_live_out.data);
    free(old_live_in.data);
    free(tmp.data);
}
int cmp_lifetime(const void *a, const void *b) { return ((Lifetime *)a)->start - ((Lifetime *)b)->start; }
void linear_stack_slot_allocation(Lifetime *lts, const int count, int *stack_size) {
    RegisterSlot *slots = ((void *)0);
    int slot_count = 0;
    for (int i = 0; i < count; i++) {
        Lifetime *l = &lts[i];
        int found_slot = 0;
        for (int j = 0; j < slot_count; j++) {
            RegisterSlot *s = &slots[j];
            if (s->free_at <= l->start) {
                s->free_at = l->end;
                l->stack_slot = j;
                l->stack_offset = s->v.phys_reg.offset;
                found_slot = 1;
                break;
            }
        }
        if (!found_slot) {
            RegisterSlot *new_slots = realloc(slots, sizeof(RegisterSlot) * (slot_count + 1));
            if (!new_slots) {
                do { log_message(LOG_ERROR, "Failed to realloc new_slots\n"); exit(1); } while (0);
            }
            slots = new_slots;
            l->stack_offset = -(*stack_size) - l->v->size;
            slots[slot_count].v = ir_stack_value(l->v->size, l->v->align, l->stack_offset);
            slots[slot_count].free_at = l->end;
            l->stack_slot = slot_count++;
            *stack_size += l->v->size;
        }
    }
    if (slots) free(slots);
}
RegSize reg_size(const int size) {
    switch (size) {
    case 1:
        return REG_8;
    case 2:
        return REG_16;
    case 4:
        return REG_32;
    case 8:
        return REG_64;
    default:
        do { log_message(LOG_ERROR, "Incompatible size %d, should have already been handled tho\n", size); exit(1); } while (0);
    }
}
const Lifetime *get_lifetime(const Lifetime *lts, const int lts_count, const int reg) {
    for (int i = 0; i < lts_count; i++) {
        if (lts[i].reg == reg) {
            return &lts[i];
        }
    }
    do { log_message(LOG_ERROR, "Failed to find lifetime of r%d\n", reg); exit(1); } while (0);
}
void ir_lower_symbol_value(IR_Value *v, const Array *symbol_slots, const Array *symbol_map) {
    const IR_Value old = *v;
    switch (old.symbol->kind) {
    case VAR:
        if (old.symbol->storage == STORAGE_NONE) {
            const int index = get_symbol_index(symbol_map, v->symbol);
            if (!(index != -1)) do { log_message(LOG_ERROR, "Tried to find symbol index of %s\n", v->symbol->name); exit(1); } while (0);
            v->kind = IR_PHYS_REG;
            *v = ((RegisterSlot *)get(symbol_slots, index))->v;
            return;
        }
    case FUNC:
        v->kind = IR_PHYS_REG;
        v->phys_reg.kind = REG_IP;
        v->phys_reg.size = REG_64;
        v->phys_reg.data_kind = REG_DATA_LABEL;
        v->phys_reg.label = old.symbol->name;
        break;
    case TYPEDEF:
    case ANY:
    case ENUM:
        do { log_message(LOG_ERROR, "ir_lower_symbol_value dont know how to handle\n"); exit(1); } while (0);
    }
}
IR_Value ir_gp_register(const GP_Reg reg) {
    return (IR_Value){.kind = IR_PHYS_REG,
                      .size = 8,
                      .align = 8,
                      .phys_reg = (PhysReg){
                          .kind = REG_GP,
                          .gp_reg = reg,
                          .data_kind = REG_DATA_NONE,
                          .size = REG_64,
                      }};
}
IR_Value ir_gp_register_value(const GP_Reg reg) {
    return (IR_Value){.kind = IR_PHYS_REG,
                      .size = 8,
                      .align = 8,
                      .phys_reg = (PhysReg){
                          .kind = REG_GP,
                          .gp_reg = reg,
                          .data_kind = REG_DATA_NONE,
                          .size = REG_64,
                      }};
}
IR_Value ir_gp_register_offset_value(const GP_Reg reg, const int offset) {
    return (IR_Value){.kind = IR_PHYS_REG,
                      .size = 8,
                      .align = 8,
                      .phys_reg = (PhysReg){
                          .kind = REG_GP,
                          .gp_reg = reg,
                          .data_kind = REG_DATA_OFFSET,
                          .size = REG_64,
                          .offset = offset,
                      }};
}
IR_Value ir_stack_value(const int size, const int align, const int offset) {
    return (IR_Value){.kind = IR_PHYS_REG,
                      .size = size,
                      .align = align,
                      .phys_reg = (PhysReg){
                          .kind = REG_GP,
                          .gp_reg = RBP,
                          .data_kind = REG_DATA_OFFSET,
                          .size = REG_64,
                          .offset = offset,
                      }};
}
void ir_lower_vreg_value(IR_Value *v, const Lifetime *lts, const int lts_count) {
    if (!(lts)) do { log_message(LOG_ERROR, "LTS is null\n"); exit(1); } while (0);
    if (!(v->kind == IR_VREG)) do { log_message(LOG_ERROR, "Expected VREG IR Value\n"); exit(1); } while (0);
    *v = ir_stack_value(v->size, v->align, get_lifetime(lts, lts_count, v->vreg)->stack_offset);
}
void verify_completion(const IR_Function *f) {
    for (int i = 0; i < f->blocks_array.count; i++) {
        const IR_Block *b = get_block(f, i);
        for (int j = 0; j < b->instruction_array.count; j++) {
            const IR_Instruction *instr = get_instruction(&b->instruction_array, j);
            const int value_count = instr->op == IR_CALL ? instr->op_count + instr->call.arg_array.count : instr->op_count;
            for (int k = 0; k < value_count; k++) {
                const IR_Value *a = k < instr->op_count ? &instr->ops[k] : &get_call_arg(instr, k - instr->op_count)->v;
                if (a->kind == IR_CONSTANT) continue;
                if (a->kind == IR_CONSTANT && instr->op != IR_CALL) continue;
                if (a->kind == IR_PHYS_REG || a->kind == IR_INT_LITERAL) continue;
                if (instr->op == IR_RET && instr->ret.type == type_void) continue;
                if (instr->op == IR_CALL && instr->call.type->_func.return_type == type_void) continue;
                log_start(LOG_ERROR);
                print_ir_value(a);
                printf(" was not converted to stack offset\n");
                exit(1);
            }
        }
    }
}
int get_symbol_index(const Array *symbol_map, const Symbol *symbol) {
    for (int i = 0; i < symbol_map->count; i++) {
        const Symbol *s = *(Symbol **)get(symbol_map, i);
        if (s == symbol) return i;
    }
    return -1;
}
void symbol_slot_allocation(const IR_Context *ctx, const IR_Function *f, int *frame_size, Array *symbol_slots, Array *symbol_map) {
    const int slot_count = f->locals_array.count + ctx->module->global_array.count;
    if (slot_count == 0) return;
    array_init(symbol_map, slot_count, sizeof(Symbol *));
    array_init(symbol_slots, slot_count, sizeof(RegisterSlot));
    for (int i = 0; i < ctx->module->global_array.count; i++) {
        Symbol *global_symbol = get_global(ctx, i)->symbol;
        append(symbol_slots, &(RegisterSlot){.v = ir_symbol_value(global_symbol), .free_at = -1});
        append(symbol_map, &global_symbol);
    }
    int stack_offset = 16;
    int offset = 0;
    int gp_count = 0;
    int fp_count = 0;
    for (int i = 0; i < f->locals_array.count; i++) {
        Symbol *local_symbol = get_local_symbol(f, i);
        const ABI_Result res = abi_classify(local_symbol->type);
        const int size = align(local_symbol->type->size, 8);
        offset = -(*frame_size) - size;
        if (local_symbol->scope_depth == 1 && res.memory) {
            offset = stack_offset;
            stack_offset += size;
        } else *frame_size += size;
        append(symbol_slots, &(RegisterSlot){.v = ir_stack_value(size, 8, offset), .free_at = -1});
        append(symbol_map, &local_symbol);
    }
}
void gen_abi_func_types() {
    const int n = typepool.count;
    for (int i = 0; i < n; i++) {
        Type *t = arena_get(&typepool, i);
        if (t->kind == T_FUNCTION) abi_func_type_gen(t);
    }
}
void lower_ir_values_to_stack(const IR_Function *f, const Lifetime *lts, const int lts_count, const Array *symbol_slots,
                              const Array *symbol_map) {
    for (int i = 0; i < f->blocks_array.count; i++) {
        const IR_Block *b = get_block(f, i);
        for (int j = 0; j < b->instruction_array.count; j++) {
            IR_Instruction *instr = get_instruction(&b->instruction_array, j);
            const int value_count = instr->op == IR_CALL ? instr->op_count + instr->call.arg_array.count : instr->op_count;
            for (int k = 0; k < value_count; k++) {
                const int is_arg_param = k >= instr->op_count;
                const int instr_index = is_arg_param ? k - instr->op_count : k;
                IR_CallArg *arg = is_arg_param ? get_call_arg(instr, instr_index) : ((void *)0);
                IR_Value *a = is_arg_param ? &arg->v : &instr->ops[instr_index];
                switch (a->kind) {
                case IR_VREG:
                    ir_lower_vreg_value(a, lts, lts_count);
                    break;
                case IR_SYMBOL:
                    ir_lower_symbol_value(a, symbol_slots, symbol_map);
                    break;
                case IR_CONSTANT:
                case IR_PHYS_REG:
                case IR_INT_LITERAL:
                    break;
                case IR_UNDEFINED:
                    if (instr->op == IR_RET && instr->ret.type == type_void) break;
                    if (instr->op == IR_CALL && instr->call.type->_func.return_type == type_void) break;
                    do { log_message(LOG_ERROR, "An undefined IR value made it to analysis!!\n"); exit(1); } while (0);
                    break;
                }
            }
        }
    }
}
void lower_ir_for_asm(IR_Function *f) {
    for (int i = 0; i < f->blocks_array.count; i++) {
        IR_Block *b = get_block(f, i);
        int param_cursor = 0;
        for (int j = 0; j < b->instruction_array.count; j++) {
            IR_Instruction *instr = get_instruction(&b->instruction_array, j);
            if (instr->op == IR_PARAM) {
                param_cursor++;
            }
        }
        int param_index = 0;
        int instrs_added = 0;
        for (int j = 0; j < b->instruction_array.count; j++) {
            IR_Instruction *instr = get_instruction(&b->instruction_array, j);
            if (instr->op == IR_RET) {
                abi_lower_ret(f, b, instr, &j);
            } else if (instr->op == IR_PARAM) {
                abi_lower_param(f, b, instr, &instrs_added, param_index++, &param_cursor);
            } else if (instr->op == IR_LOAD) {
                if (!(instr->load.type->size <= 8)) do { log_message(LOG_ERROR, " : ir_load of type sized %d is larger than 8 bytes %t\n", instr->load.type->size, instr->load.type); exit(1); } while (0);
                if (instr->load.type->kind == T_STRUCT) instr->load.type = get_integer_type(instr->load.type->size);
            } else if (instr->op == IR_STORE) {
                abi_lower_store(f, b, instr, &j);
            }
        }
    }
}
void analysis(const IR_Context *ctx) {
    for (int i = 0; i < ctx->module->functions_array.count; i++) {
        IR_Function *f = get_func(ctx->module, i);
        lower_ir_for_asm(f);
        if (has_flag(CF_DEBUG_LOWERED_IR)) {
            printf("vvvvvvvvvvvvvvvvvvvvv\n");
            print_ir_function(ctx, f);
            printf("\n^^^^^^^^^^^^^^^^^^^^^\n");
        }
        ir_init_func_cfg(f);
        ir_compute_func_io(f);
        Lifetime *lifetimes = ((void *)0);
        int *rpo = malloc(f->blocks_array.count * sizeof(int));
        for (int i = 0; i < f->blocks_array.count; i++) {
            rpo[i] = -1;
        }
        if (!rpo) {
            do { log_message(LOG_ERROR, "Failed to allocate rpo\n"); exit(1); } while (0);
        }
        int reg_count = 0;
        if (f->max_reg > 0) {
            reg_count = reg_bitset(f);
            compute_reverse_postorder(f, rpo);
            compute_bitset(f, rpo);
            lifetimes = compute_lifetimes(f, reg_count, rpo);
            qsort(lifetimes, reg_count, sizeof(Lifetime), cmp_lifetime);
            if (has_flag(CF_DEBUG_LIFETIMES)) {
                for (int j = 0; j < reg_count; j++) {
                    printf("r%d = [%d -> %d]\n", lifetimes[j].reg, lifetimes[j].start, lifetimes[j].end);
                }
                printf("\n");
            }
        }
        Array symbol_map = {};
        Array symbol_slots = {};
        const int variadic_space = f->type->_func.is_variadic ? 176 : 0;
        int frame_size = variadic_space;
        symbol_slot_allocation(ctx, f, &frame_size, &symbol_slots, &symbol_map);
        linear_stack_slot_allocation(lifetimes, reg_count, &frame_size);
        lower_ir_values_to_stack(f, lifetimes, reg_count, &symbol_slots, &symbol_map);
        verify_completion(f);
        f->stack_size = frame_size;
        free(rpo);
        free(lifetimes);
        array_free(&symbol_map);
        array_free(&symbol_slots);
    }
}
Lifetime *compute_lifetimes(const IR_Function *f, const int defined, const int *rpo) {
    Lifetime *lts = malloc(sizeof(Lifetime) * defined);
    int pc = 0;
    for (int i = 0; i < f->blocks_array.count; i++) {
        const int index = rpo[i];
        if (index == -1) continue;
        const IR_Block *b = get_block(f, rpo[i]);
        for (int j = 0; j < b->instruction_array.count; j++) {
            IR_Instruction *instr = get_instruction(&b->instruction_array, j);
            const int value_count = instr->op == IR_CALL ? instr->op_count + instr->call.arg_array.count : instr->op_count;
            for (int k = 0; k < value_count; k++) {
                const IR_Value *a = k < instr->op_count ? &instr->ops[k] : &get_call_arg(instr, k - instr->op_count)->v;
                const int is_call_arg = k >= instr->op_count;
                if (a->kind == IR_VREG) {
                    if (a->vreg < 0) continue;
                    if (op_info[instr->op].def_mask & (1 << k)) {
                        lts[instr->ops[k].vreg] = (Lifetime){instr->ops[k].vreg, pc, -1, 0, 0, .v = &instr->ops[k]};
                    }
                    if (is_call_arg || op_info[instr->op].use_mask & (1 << k)) {
                        if (lts[a->vreg].end < pc) {
                            lts[a->vreg].end = pc;
                        }
                    }
                }
            }
            pc++;
        }
    }
    return lts;
}
void print_bitset(const BitSet *bs) {
    printf("{");
    int first = 1;
    for (int i = 0; i < bs->num_bits; i++) {
        if (bitset_has(bs, i)) {
            if (!first) printf(", ");
            printf("r%d", i);
            first = 0;
        }
    }
    printf("}");
}
void print_cfg(const IR_Function *func) {
    printf("Func Analysis: %s\n", func->name);
    for (int i = 0; i < func->blocks_array.count; i++) {
        const IR_Block *b = get_block(func, i);
        printf("L%d:\n", i);
        printf("  Succ: ");
        if (b->cfg.succ_count == 0) printf("None");
        for (int j = 0; j < b->cfg.succ_count; j++) printf("L%d ", b->cfg.succ[j]);
        printf("\n");
        printf("  Pred: ");
        if (b->cfg.pred_count == 0) printf("None");
        for (int j = 0; j < b->cfg.pred_count; j++) printf("L%d ", b->cfg.pred[j]);
        printf("\n");
        printf("  Def: ");
        print_bitset(&b->live.def);
        printf("\n");
        printf("  Use: ");
        print_bitset(&b->live.use);
        printf("\n");
        printf("  Live-in: ");
        print_bitset(&b->live.live_in);
        printf("\n");
        printf("  Live-out: ");
        print_bitset(&b->live.live_out);
        printf("\n");
    }
}
