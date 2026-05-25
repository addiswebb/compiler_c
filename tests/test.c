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
void link(Compiler *c, Array *objs);
int compile(Compiler *compiler);
void free_compiler(Compiler *compiler);
static int load_src_file(Compiler *compiler, const char *file);
char *replace_extension(const char *path, const char *ext);
int is_alpha(char c);
int is_num(char c);
int is_alpha_num(char c);
int is_alpha_numeric_str(const char *c);
char to_lower_case(const char c);
int is_hex(const char c);
int is_oct(const char c);
int is_binary(const char c);
int is_whitespace(char c);
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
Parser new_parser() { return (Parser){0}; }
void init_parser(Parser *p, Array *src, const int size) {
    p->size = size;
    p->src = src;
    p->index = 0;
    p->expect_semi = 1;
    array_init(&p->scopes_array, 4, sizeof(Array));
    arena_init(&p->symbols_arena, 64, sizeof(Symbol));
    p_append_symbol_table(p);
}
void free_parser(Parser *p) {
    for (int i = 0; i < p->scopes_array.count; i++) {
        array_free(get_symbol_table(p, i));
    }
    array_free(&p->scopes_array);
    arena_free(&p->symbols_arena);
}
void p_append_symbol_table(Parser *p) {
    Array symbol_table;
    array_init(&symbol_table, 4, sizeof(Symbol *));
    append(&p->scopes_array, &symbol_table);
}
int current_scope_depth;
int p_is_last_token(const Parser *p) { return p->index >= p->size; }
Token *p_peek_n(const Parser *p, const int n) {
    if (p->index + n > p->src->count) {
        printf("P_peek_n Tried peeking past eof\n");
        return ((void *)0);
    }
    return get_token(p->src, p->index + n);
}
Token *p_peek(const Parser *p) { return p_peek_n(p, 0); }
Token *p_peek_next(const Parser *p) { return p_peek_n(p, 1); }
Token *p_consume_n(Parser *p, const int n) {
    if (n == 0) return ((void *)0);
    if (p->index + n > p->src->count) {
        printf("P_consume_n %d Reached the end of the token list %d/%d\n", n, p->index, p->src->count);
        return ((void *)0);
    }
    Token *token = get_token(p->src, p->index);
    p->index += n;
    if (has_flag(CF_DEBUG_PARSER)) print_token(token);
    return token;
}
Token *p_consume(Parser *p) { return p_consume_n(p, 1); }
void p_skip_n(Parser *p, const int n) { p_consume_n(p, n); }
void p_skip(Parser *p) { p_consume_n(p, 1); }
void p_expect(const Parser *p, const TokenType expected_type) {
    if (!p_is_last_token(p)) {
        const Token *token = get_token(p->src, p->index);
        if (token->type != expected_type) {
            log_start(LOG_ERROR);
            printf("[%d:%d]: ", token->line_n, token->char_n);
            printf("Expected ");
            print_token_type(expected_type);
            printf(" got ");
            print_token_type(token->type);
            printf("\n");
            exit(1);
        }
    }
}
Token *p_consume_a(Parser *p, const TokenType type) {
    p_expect(p, type);
    return p_consume(p);
}
Token *p_consume_semi(Parser *p) {
    if (p->expect_semi) {
        return p_consume_a(p, TK_SEMI);
    }
    return ((void *)0);
}
Node *init_translation_unit(NodeManager *nm) {
    Node *node = new_node(nm, N_TRANSLATION_UNIT);
    array_init(&node->translation_unit.declarations_array, 8, sizeof(Node **));
    return node;
}
Node *new_compound_node(NodeManager *nm) {
    Node *node = new_node(nm, N_COMPOUND);
    array_init(&node->compound.items_array, 8, sizeof(Node **));
    return node;
}
Node *p_parse_builtin(Parser *p, NodeManager *nm, BuiltinKind kind) {
    Node *b = new_node(nm, N_BUILTIN);
    b->_builtin.kind = kind;
    p_consume_a(p, TK_OPEN_PAREN);
    switch (kind) {
    case BUILTIN_MEMCPY:
        array_init(&b->_builtin.params, 3, sizeof(Node *));
        append(&b->_builtin.params, &(Node *){p_parse_expression(p, nm, 0)});
        p_consume_a(p, TK_COMMA);
        append(&b->_builtin.params, &(Node *){p_parse_expression(p, nm, 0)});
        p_consume_a(p, TK_COMMA);
        append(&b->_builtin.params, &(Node *){p_parse_expression(p, nm, 0)});
        break;
    case BUILTIN_VA_START:
        array_init(&b->_builtin.params, 2, sizeof(Node *));
        append(&b->_builtin.params, &(Node *){p_parse_expression(p, nm, 0)});
        p_consume_a(p, TK_COMMA);
        append(&b->_builtin.params, &(Node *){p_parse_expression(p, nm, 0)});
        break;
    case BUILTIN_VA_ARG:
        array_init(&b->_builtin.params, 2, sizeof(Node *));
        append(&b->_builtin.params, &(Node *){p_parse_expression(p, nm, 0)});
        p_consume_a(p, TK_COMMA);
        Node *va_arg_type = new_node(nm, N_TYPE);
        va_arg_type->type = p_parse_abstract_type(p, nm);
        append(&b->_builtin.params, &va_arg_type);
        break;
    case BUILTIN_VA_END:
        array_init(&b->_builtin.params, 1, sizeof(Node *));
        append(&b->_builtin.params, &(Node *){p_parse_expression(p, nm, 0)});
        break;
    case BUILTIN_NONE:
        do { log_message(LOG_ERROR, "builtin reached but not builtin\n"); exit(1); } while (0);
        break;
    }
    p_consume_a(p, TK_CLOSE_PAREN);
    return b;
}
Node *p_parse_postfix_expression(Parser *p, NodeManager *nm) {
    Node *expr = p_parse_primary_expression(p, nm);
    for (;;) {
        switch (p_peek(p)->type) {
        case TK_OPEN_PAREN:
            if (expr->kind == N_IDENTIFIER) {
                BuiltinKind kind = get_builtin_kind(expr->identifier.name);
                if (kind != BUILTIN_NONE) {
                    expr = p_parse_builtin(p, nm, kind);
                    break;
                }
            }
            p_consume(p);
            Node *func_call = new_function_call_node(nm, expr);
            while (p_peek(p)->type != TK_CLOSE_PAREN) {
                p_append_call_param(func_call, p_parse_expression(p, nm, 0));
                if (p_peek(p)->type == TK_COMMA) p_consume(p);
                else break;
            }
            p_consume_a(p, TK_CLOSE_PAREN);
            expr = func_call;
            break;
        case TK_OPEN_SQUARE:
            p_consume(p);
            if (!is_lvalue(expr)) {
                log_start(LOG_ERROR);
                print_node_type(expr->kind);
                printf(" is not a an lvalue, needed for indexing\n");
                exit(1);
            }
            Node *node = new_node(nm, N_INDEX);
            node->index.index = p_parse_expression(p, nm, 0);
            node->index.identifier = expr;
            expr = node;
            p_consume_a(p, TK_CLOSE_SQUARE);
            break;
        case TK_DOT:
        case TK_ARROW:
            TokenType op = p_consume(p)->type;
            Token *t = p_consume_a(p, TK_IDENTIFIER);
            Node *member = new_node(nm, N_IDENTIFIER);
            member->identifier.name = t->value;
            member->identifier.len = t->size;
            Node *access = new_node(nm, N_MEMBER_ACCESS);
            access->member_access.op = op;
            access->member_access.identifier = expr;
            access->member_access.member = member;
            expr = access;
            break;
        case TK_INCR:
        case TK_DECR:
            Node *unary = new_node(nm, N_UNARY);
            unary->unary.op = p_consume(p)->type;
            unary->unary.associativity = 1;
            unary->unary.expr = expr;
            expr = unary;
            break;
        default:
            return expr;
        }
    }
}
Node *p_parse_prefix(Parser *p, NodeManager *nm) {
    if (is_unary_operator(p_peek(p)->type)) {
        Node *node = new_node(nm, N_UNARY);
        node->unary.op = p_consume(p)->type;
        node->unary.associativity = 0;
        if (node->unary.op == TK_SIZEOF && p_peek(p)->type == TK_OPEN_PAREN && is_start_of_type(p, p_peek_next(p))) {
            node->unary.expr = new_node(nm, N_TYPE);
            p_consume(p);
            node->unary.expr->type = p_parse_abstract_type(p, nm);
            p_consume_a(p, TK_CLOSE_PAREN);
        } else node->unary.expr = p_parse_cast(p, nm);
        return node;
    }
    return p_parse_postfix_expression(p, nm);
}
Node *p_parse_primary_expression(Parser *p, NodeManager *nm) {
    Node *primary = ((void *)0);
    Token *tk;
    switch (p_peek(p)->type) {
    case TK_INT_LITERAL:
    case TK_FLT_LITERAL:
    case TK_CHAR_LITERAL:
    case TK_STRING_LITERAL:
        primary = new_node(nm, N_LITERAL);
        tk = p_consume(p);
        primary->literal.kind = literal_kind(tk->type);
        primary->literal.raw_rata = tk->value;
        primary->literal.len = tk->size;
        if (primary->literal.kind == L_STRING) {
            Array cat_str;
            array_init(&cat_str, primary->literal.len + tk->size + 1, sizeof(char));
            memcpy(cat_str.data, primary->literal.raw_rata, primary->literal.len);
            cat_str.count += primary->literal.len;
            while (p_peek(p)->type == TK_STRING_LITERAL) {
                tk = p_consume(p);
                array_str_catn(&cat_str, tk->value, tk->size);
            }
            primary->literal.raw_rata = cat_str.data;
            primary->literal.len = cat_str.count;
        }
        return primary;
    case TK_IDENTIFIER:
        primary = new_node(nm, N_IDENTIFIER);
        tk = p_consume(p);
        primary->identifier.name = tk->value;
        primary->identifier.len = tk->size;
        break;
    case TK_OPEN_PAREN:
        p_consume_a(p, TK_OPEN_PAREN);
        primary = p_parse_expression(p, nm, 0);
        p_consume_a(p, TK_CLOSE_PAREN);
        return primary;
    case TK_OPEN_CURLY:
        return p_parse_init_list(p, nm);
    default:
        tk = p_consume(p);
        do { log_message(LOG_ERROR, "[%d:%d]: Expected primary expression got %tk\n", tk->line_n, tk->char_n, tk->type); exit(1); } while (0);
    }
    return primary;
}
Node *p_parse_init_list(Parser *p, NodeManager *nm) {
    Node *node = new_init_list_node(nm);
    p_consume(p);
    while (p_peek(p)->type != TK_CLOSE_CURLY) {
        if (p_peek(p)->type == TK_DOT) {
            p_consume(p);
            Token *token = p_consume_a(p, TK_IDENTIFIER);
            p_consume_a(p, TK_EQ);
            Node *member_assign = new_node(nm, N_DESIGNATOR);
            member_assign->designated_init.kind = T_STRUCT;
            member_assign->designated_init._struct.name = token->value;
            member_assign->designated_init.value = p_parse_expression(p, nm, 0);
            p_append_element(node, member_assign);
        } else if (p_peek(p)->type == TK_OPEN_SQUARE) {
            p_consume(p);
            Node *index_expr = p_parse_expression(p, nm, 0);
            p_consume_a(p, TK_CLOSE_SQUARE);
            p_consume_a(p, TK_EQ);
            Node *element_assign = new_node(nm, N_DESIGNATOR);
            element_assign->designated_init._array.const_expr = index_expr;
            element_assign->designated_init.value = p_parse_expression(p, nm, 0);
            p_append_element(node, element_assign);
        } else p_append_element(node, p_parse_expression(p, nm, 0));
        if (p_peek(p)->type == TK_COMMA) p_consume(p);
        else break;
    }
    p_consume_a(p, TK_CLOSE_CURLY);
    return node;
}
Node *new_init_list_node(NodeManager *nm) {
    Node *node = new_node(nm, N_INIT_LIST);
    array_init(&node->init_list.elements_array, 4, sizeof(Node *));
    return node;
}
Node *new_function_node(NodeManager *nm) { return new_node(nm, N_FUNCTION); }
Node *new_function_call_node(NodeManager *nm, Node *identifier) {
    Node *node = new_node(nm, N_FUNCTION_CALL);
    node->func_call.callee = identifier;
    array_init(&node->func_call.params_array, 4, sizeof(Node *));
    return node;
}
Node *p_parse_goto_statement(Parser *p, NodeManager *nm) {
    p_consume(p);
    Node *identifier = new_node(nm, N_IDENTIFIER);
    const Token *t = p_consume_a(p, TK_IDENTIFIER);
    identifier->identifier.name = t->value;
    identifier->identifier.len = t->size;
    Node *node = new_node(nm, N_GOTO);
    node->_goto.identifier = identifier;
    p_consume_semi(p);
    return node;
}
Node *p_parse_label(Parser *p, NodeManager *nm) {
    Node *identifier = new_node(nm, N_IDENTIFIER);
    const Token *t = p_consume_a(p, TK_IDENTIFIER);
    identifier->identifier.name = t->value;
    identifier->identifier.len = t->size;
    Node *node = new_node(nm, N_LABEL);
    node->_goto.identifier = identifier;
    p_consume_a(p, TK_COLON);
    return node;
}
Node *p_parse_cast(Parser *p, NodeManager *nm) {
    if (p_peek(p)->type == TK_OPEN_PAREN && is_start_of_type(p, p_peek_next(p))) {
        p_consume_a(p, TK_OPEN_PAREN);
        Type *type = p_parse_abstract_type(p, nm);
        p_consume_a(p, TK_CLOSE_PAREN);
        if (p_peek(p)->type == TK_OPEN_CURLY) {
            Node *comp_node = new_node(nm, N_COMPOUND_LITERAL);
            comp_node->type = type;
            comp_node->compound_literal.value = p_parse_init_list(p, nm);
            return comp_node;
        }
        Node *cast_node = new_node(nm, N_CAST);
        cast_node->type = type;
        cast_node->cast.to = type;
        cast_node->cast.expr = p_parse_cast(p, nm);
        return cast_node;
    }
    return p_parse_prefix(p, nm);
}
Node *p_parse_binary(Parser *p, NodeManager *nm, Node *lhs, const int min_prec) {
    Node *b = new_node(nm, N_BINARY);
    b->binary.op = p_consume(p)->type;
    b->binary.rhs = p_parse_expression(p, nm, op_precedence(b->binary.op) + op_associativity(b->binary.op));
    b->binary.lhs = lhs;
    return b;
}
Node *p_parse_ternary(Parser *p, NodeManager *nm, Node *cond) {
    p_consume(p);
    Node *t = new_node(nm, N_TERNARY);
    t->ternary.cond = cond;
    t->ternary.if_true = p_parse_expression(p, nm, 0);
    p_consume_a(p, TK_COLON);
    t->ternary.if_false = p_parse_expression(p, nm, op_precedence(TK_TERNARY));
    return t;
}
Node *p_parse_expression(Parser *p, NodeManager *nm, const int min_prec) {
    if (p_peek(p)->type == TK_SEMI) {
        if (p->expect_semi) p_consume(p);
        return new_node(nm, N_NULL);
    }
    Node *primary = p_parse_cast(p, nm);
    while (is_postfix_operator(p_peek(p)->type)) primary = p_parse_postfix_expression(p, nm);
    for (;;) {
        if (is_binary_operator(p_peek(p)->type) && op_precedence(p_peek(p)->type) >= min_prec) {
            primary = p_parse_binary(p, nm, primary, min_prec);
        } else if (p_peek(p)->type == TK_TERNARY && op_precedence(TK_TERNARY) >= min_prec) {
            primary = p_parse_ternary(p, nm, primary);
        } else break;
    }
    return primary;
}
Node *p_parse_block_item(Parser *p, NodeManager *nm) {
    if (is_start_of_type(p, p_peek(p))) return p_parse_block_declaration(p, nm);
    else return p_parse_statement(p, nm);
}
Type *p_parse_abstract_type(Parser *p, NodeManager *nm) {
    const char *name = ((void *)0);
    Type *type = p_parse_type(p, nm, &name);
    if (!(name == ((void *)0))) do { log_message(LOG_ERROR, "Unexpected identifier in parsing abstract type.\n"); exit(1); } while (0);
    return type;
}
void free_declarator(Declarator *decl) {
    for (int i = 0; i < decl->modifiers.count; i++) {
        Modifier *mod = get(&decl->modifiers, i);
        if (mod->kind == MOD_FUNCTION) array_free(&mod->function.params);
    }
    array_free(&decl->modifiers);
}
Type *p_parse_type(Parser *p, NodeManager *nm, const char **name) {
    Type *type;
    unsigned int qualifiers = QUAL_NONE;
    int is_signed = 1;
    for (;;) {
        if (p_peek(p)->type == TK_CONST) qualifiers |= QUAL_CONST;
        else if (p_peek(p)->type == TK_VOLATILE) qualifiers |= QUAL_VOLATILE;
        else if (p_peek(p)->type == TK_UNSIGNED) is_signed = 0;
        else if (p_peek(p)->type == TK_SIGNED) is_signed = 1;
        else break;
        p_consume(p);
    }
    if (p_peek(p)->type == TK_STRUCT) type = p_parse_struct(p, nm);
    else if (p_peek(p)->type == TK_ENUM) type = p_parse_enum(p, nm);
    else if (p_peek(p)->type == TK_UNION) type = p_parse_union(p, nm);
    else type = token_to_type(p, p_consume(p));
    if (!(type != type_invalid)) do { log_message(LOG_ERROR, "Got type_invalid in p_parse_base_type\n"); exit(1); } while (0);
    if (type->kind == T_INT && !is_signed && is_signed != type->is_signed) type = get_unsigned_type(type);
    Declarator decl = p_parse_declarator(p, nm);
    *name = decl.name;
    type = get_modified_type(type, &decl);
    if (qualifiers != QUAL_NONE) type = get_qualified_type(type, qualifiers);
    free_declarator(&decl);
    return type;
}
Declarator p_parse_declarator(Parser *p, NodeManager *nm) {
    Declarator d = {.name = ((void *)0)};
    array_init(&d.modifiers, 4, sizeof(Modifier));
    int ptrs = 0;
    while (p_peek(p)->type == TK_MULTIPLY) {
        p_consume(p);
        ptrs++;
    }
    if (p_peek(p)->type == TK_OPEN_PAREN) {
        p_consume(p);
        free_declarator(&d);
        d = p_parse_declarator(p, nm);
        p_consume_a(p, TK_CLOSE_PAREN);
    } else if (p_peek(p)->type == TK_IDENTIFIER) d.name = p_consume_a(p, TK_IDENTIFIER)->value;
    for (;;) {
        if (p_peek(p)->type == TK_OPEN_SQUARE) {
            p_consume(p);
            Node *expr = ((void *)0);
            if (p_peek(p)->type != TK_CLOSE_SQUARE) expr = p_parse_expression(p, nm, 0);
            p_consume_a(p, TK_CLOSE_SQUARE);
            append(&d.modifiers, &(Modifier){.kind = MOD_ARRAY, .array_bounds = expr});
        } else if (p_peek(p)->type == TK_OPEN_PAREN) {
            Modifier func_modifier = p_parse_parameter_list(p, nm);
            append(&d.modifiers, &func_modifier);
        } else break;
    }
    for (int i = 0; i < ptrs; i++) {
        append(&d.modifiers, &(Modifier){.kind = MOD_POINTER});
    }
    return d;
}
Type *p_parse_enum(Parser *p, NodeManager *nm) {
    Type enum_t = enum_type();
    p_consume_a(p, TK_ENUM);
    if (p_peek(p)->type == TK_IDENTIFIER) {
        enum_t._enum.name = p_consume(p)->value;
    }
    if (p_peek(p)->type == TK_OPEN_CURLY) {
        array_init(&enum_t._enum.fields_array, 4, sizeof(EnumField));
        p_consume(p);
        int val = 0;
        while (p_peek(p)->type != TK_CLOSE_CURLY) {
            EnumField f = {};
            f.name = p_consume_a(p, TK_IDENTIFIER)->value;
            if (p_peek(p)->type == TK_EQ) {
                p_consume(p);
                f.const_expr = p_parse_expression(p, nm, 0);
            }
            f._enum_t = ((void *)0);
            append_enum_field(&enum_t, &f);
            if (p_peek(p)->type == TK_COMMA) p_consume(p);
            else break;
        }
        p_consume(p);
        enum_t._enum.complete = 1;
    }
    Type *s = get_enum_type(enum_t._enum.name);
    if (s) {
        if (enum_t._enum.complete) {
            if (s->_enum.complete) {
                do { log_message(LOG_ERROR, "Redefinition of enum %s\n", enum_t._enum.name); exit(1); } while (0);
            } else *s = enum_t;
        }
        return s;
    } else {
        Type *t = new_type();
        for (int i = 0; i < enum_t._enum.fields_array.count; i++) {
            EnumField *f = get_enum_field(&enum_t, i);
            f->_enum_t = t;
        }
        *t = enum_t;
        t->is_resolved = 0;
        return t;
    }
}
Type *p_parse_union(Parser *p, NodeManager *nm) {
    Type union_t = union_type();
    p_consume_a(p, TK_UNION);
    if (p_peek(p)->type == TK_IDENTIFIER) {
        union_t._union.name = p_consume(p)->value;
    }
    if (p_peek(p)->type == TK_OPEN_CURLY) {
        array_init(&union_t._union.members_array, 4, sizeof(UnionMember));
        p_consume(p);
        while (p_peek(p)->type != TK_CLOSE_CURLY) {
            UnionMember m;
            m.name = ((void *)0);
            Type *t = p_parse_type(p, nm, &m.name);
            if (!(t->kind == T_STRUCT || t->kind == T_UNION)) if (!(m.name)) do { log_message(LOG_ERROR, "Scalar Union member must be named\n"); exit(1); } while (0);
            m.type = t;
            m.offset = 0;
            append_union_member(&union_t, &m);
            p_consume_semi(p);
        }
        p_consume(p);
        union_t._union.complete = 1;
    }
    Type *u = get_union_type(union_t._union.name);
    if (u) {
        if (union_t._union.complete) {
            if (u->_union.complete) {
                do { log_message(LOG_ERROR, "Redefinition of union %s\n", union_t._union.name); exit(1); } while (0);
            } else *u = union_t;
        }
        return u;
    } else {
        Type *t = new_type();
        *t = union_t;
        return t;
    }
}
Type *p_parse_struct(Parser *p, NodeManager *nm) {
    Type struct_t = struct_type();
    p_consume_a(p, TK_STRUCT);
    if (p_peek(p)->type == TK_IDENTIFIER) {
        struct_t._struct.name = p_consume(p)->value;
    }
    if (p_peek(p)->type == TK_OPEN_CURLY) {
        array_init(&struct_t._struct.members_array, 4, sizeof(StructMember));
        p_consume(p);
        while (p_peek(p)->type != TK_CLOSE_CURLY) {
            StructMember f;
            f.name = ((void *)0);
            Type *t = p_parse_type(p, nm, &f.name);
            if (!(t->kind == T_STRUCT || t->kind == T_UNION)) if (!(f.name)) do { log_message(LOG_ERROR, "Scalar Struct member must be named\n"); exit(1); } while (0);
            f.type = t;
            append_struct_member(&struct_t, &f);
            p_consume_semi(p);
        }
        p_consume(p);
        struct_t.size = align(struct_t.size, struct_t.align);
        struct_t._struct.complete = 1;
    }
    Type *s = get_struct_type(struct_t._struct.name);
    if (s) {
        if (struct_t._struct.complete) {
            if (s->_struct.complete) {
                do { log_message(LOG_ERROR, "Redefinition of struct %s\n", struct_t._struct.name); exit(1); } while (0);
            } else *s = struct_t;
        }
        return s;
    } else {
        Type *t = new_type();
        *t = struct_t;
        return t;
    }
}
void p_append_block_item(Node *root, Node *item) {
    if (item != ((void *)0)) append(&root->compound.items_array, &item);
    else printf("Skipping empty node\n");
}
void p_append_param(Node *func, Node *param) {
    if (param != ((void *)0)) {
        append(&func->type->_func.params, &param);
    } else {
        do { log_message(LOG_ERROR, "Recieved a NULL param node to append\n"); exit(1); } while (0);
    }
}
void p_append_call_param(Node *func_call, Node *param) { append(&func_call->func_call.params_array, &param); }
Symbol *p_append_symbol(Array *st, const Symbol *s) { return *(Symbol **)append(st, &s); }
Symbol *p_get_symbol(const Parser *p, const char *name, const SymbolKind kind, const int same_depth) {
    for (int i = p->scopes_array.count - 1; i >= 0; i--) {
        Array *st = get_symbol_table(p, i);
        for (int j = 0; j < st->count; j++) {
            Symbol *symbol = get_symbol(st, j);
            if (same_depth && symbol->scope_depth != p->current_scope_depth) continue;
            if (symbol->scope_depth <= p->current_scope_depth && (kind == ANY || symbol->kind == kind) && strcmp(symbol->name, name) == 0) {
                return symbol;
            }
        }
    }
    return ((void *)0);
}
Typedef *p_get_typedef(const Parser *p, const char *name) {
    Symbol *s = p_get_symbol(p, name, TYPEDEF, 0);
    if (s) return &s->_typedef;
    do { log_message(LOG_ERROR, "Tried to get the typedef of %s, which does not exist\n", name); exit(1); } while (0);
}
Node *p_get_func_def(const Parser *p, const char *name) { do { log_message(LOG_ERROR, "Tried to get function definition for '%s' which does not exist\n", name); exit(1); } while (0); }
Symbol *p_new_symbol(Parser *p, const Symbol *s) { return arena_append(&p->symbols_arena, s); }
void p_append_typedef(Parser *p, const Typedef *t) {
    p_append_symbol(get_current_symbol_table(p), p_new_symbol(p, &(Symbol){.name = t->new_def,
                                                                           .kind = TYPEDEF,
                                                                           .linkage = LINK_NONE,
                                                                           .storage = STORAGE_NONE,
                                                                           ._typedef = *t,
                                                                           .type = t->type,
                                                                           .scope_depth = p->current_scope_depth}));
}
Symbol *p_append_func_def(Parser *p, Node *f) {
    if (p->scopes_array.count > 2) {
        do { log_message(LOG_ERROR, "Declaring function inside a function???\n"); exit(1); } while (0);
    }
    Linkage linkage = f->func.storage_class == STATIC ? LINK_INTERNAL : LINK_EXTERNAL;
    Storage storage = STORAGE_TEXT;
    return p_append_symbol(get_current_symbol_table(p), p_new_symbol(p, &(Symbol){.name = f->func.name,
                                                                                  .kind = FUNC,
                                                                                  .linkage = linkage,
                                                                                  .storage = storage,
                                                                                  .func_def = f,
                                                                                  .type = f->type,
                                                                                  .scope_depth = p->current_scope_depth}));
}
void update_linkage_storage(Symbol *s, Node *v) {
    if (!(v->kind == N_VAR_DECL)) do { log_message(LOG_ERROR, "Expected Var decl node to update symbol linkage and storage\n"); exit(1); } while (0);
    Linkage linkage = LINK_NONE;
    Storage storage = STORAGE_NONE;
    if (v->var_decl.is_global) {
        storage = v->var_decl.is_defined ? STORAGE_DATA : STORAGE_BSS;
        linkage = v->var_decl.storage_class == STATIC ? LINK_INTERNAL : LINK_EXTERNAL;
    } else {
        storage = STORAGE_NONE;
        if (v->var_decl.storage_class == NONE) linkage = LINK_NONE;
        if (v->var_decl.storage_class == EXTERN) linkage = LINK_EXTERNAL;
        if (v->var_decl.storage_class == STATIC) linkage = LINK_INTERNAL;
    }
    s->linkage = linkage;
    s->storage = storage;
}
Symbol *p_append_var_decl_symbol(Parser *p, Node *v) {
    Symbol *s = p_new_symbol(p, &(Symbol){.name = v->var_decl.identifier->identifier.name,
                                          .kind = VAR,
                                          .var_decl = v,
                                          .type = v->type,
                                          .scope_depth = p->current_scope_depth});
    update_linkage_storage(s, v);
    return p_append_symbol(get_current_symbol_table(p), s);
}
Symbol *p_append_param_decl_symbol(Parser *p, ParamDecl *param) {
    if (!(param->name)) do { log_message(LOG_ERROR, "Function parameter must be named\n"); exit(1); } while (0);
    return p_append_symbol(get_current_symbol_table(p), p_new_symbol(p, &(Symbol){.name = param->name,
                                                                                  .kind = VAR,
                                                                                  .linkage = LINK_NONE,
                                                                                  .storage = STORAGE_NONE,
                                                                                  .var_decl = ((void *)0),
                                                                                  .type = param->type,
                                                                                  .scope_depth = p->current_scope_depth}));
}
void p_append_enum_const(Parser *p, const EnumField *e) {
    p_append_symbol(get_current_symbol_table(p), p_new_symbol(p, &(Symbol){.name = e->name,
                                                                           .kind = ENUM,
                                                                           .linkage = LINK_NONE,
                                                                           .storage = STORAGE_NONE,
                                                                           .enum_field = e,
                                                                           .type = type_i32,
                                                                           .scope_depth = p->current_scope_depth}));
}
void p_append_element(Node *init_list, Node *element) { append(&init_list->init_list.elements_array, &element); }
Node *p_get_var_decl(const Parser *p, const char *name) {
    const Symbol *s = p_get_symbol(p, name, VAR, 0);
    if (s) return s->var_decl;
    do { log_message(LOG_ERROR, "Tried to find variable %s which does not exist\n", name); exit(1); } while (0);
}
const EnumField *p_get_enum_const(const Parser *p, const char *name) {
    Symbol *s = p_get_symbol(p, name, ENUM, 0);
    if (s) return s->enum_field;
    do { log_message(LOG_ERROR, "Tried to find enum constant %s which does not exist\n", name); exit(1); } while (0);
}
Node *p_parse_if_statement(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_IF);
    p_consume_a(p, TK_IF);
    p_consume_a(p, TK_OPEN_PAREN);
    node->_if.cond = p_parse_expression(p, nm, 0);
    p_consume_a(p, TK_CLOSE_PAREN);
    node->_if.if_true = p_parse_statement(p, nm);
    if (p_peek(p)->type == TK_ELSE) {
        p_consume(p);
        if (p_peek(p)->type == TK_IF) {
            node->_if.if_false = p_parse_if_statement(p, nm);
        } else {
            node->_if.if_false = p_parse_statement(p, nm);
        }
    } else {
        node->_if.if_false = ((void *)0);
    }
    return node;
}
Node *p_parse_do_while_loop(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_WHILE);
    node->_while.is_do_while = 1;
    p_consume_a(p, TK_DO);
    node->_while.block = p_parse_statement(p, nm);
    p_consume_a(p, TK_WHILE);
    p_consume_a(p, TK_OPEN_PAREN);
    node->_while.cond = p_parse_expression(p, nm, 0);
    p_consume_a(p, TK_CLOSE_PAREN);
    p_consume_semi(p);
    return node;
}
Node *p_parse_while_loop(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_WHILE);
    node->_while.is_do_while = 0;
    p_consume_a(p, TK_WHILE);
    p_consume_a(p, TK_OPEN_PAREN);
    node->_while.cond = p_parse_expression(p, nm, 0);
    p_consume_a(p, TK_CLOSE_PAREN);
    node->_while.block = p_parse_statement(p, nm);
    return node;
}
Node *p_parse_case(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_CASE);
    if (p_peek(p)->type == TK_CASE) {
        p_consume_a(p, TK_CASE);
        node->_case.const_expr = p_parse_expression(p, nm, 0);
    } else {
        p_consume_a(p, TK_DEFAULT);
        node->_case.const_expr = ((void *)0);
    }
    p_consume_a(p, TK_COLON);
    return node;
}
void p_append_case(Node *s, Node *c) {
    c->_case.i = s->_switch.block->compound.items_array.count;
    append(&s->_switch.cases_array, &c);
}
UnionMember *get_union_member_named(Type *union_t, const char *name) {
    int found_member = 0;
    for (int j = 0; j < union_t->_union.members_array.count; j++) {
        UnionMember *member = get_union_member(union_t, j);
        if (strcmp(member->name, name) == 0) {
            found_member = 1;
            return member;
        }
    }
    if (!found_member) {
        log_start(LOG_ERROR);
        printf("No such member '%s' on ", name);
        print_type(union_t);
        printf("\n");
        exit(1);
    }
    return ((void *)0);
}
StructMember *get_struct_member_named(Type *struct_t, const char *name, int *index) {
    int found_member = 0;
    for (int j = 0; j < struct_t->_struct.members_array.count; j++) {
        StructMember *member = get_struct_member(struct_t, j);
        if (member->name) {
            if (strcmp(member->name, name) == 0) {
                found_member = 1;
                *index = j;
                return member;
            }
        } else {
            StructMember *m = get_member(member->type, name, 0, 0,0);
            if (m) {
                *index = j;
                return m;
            }
        }
    }
    if (!found_member) {
        log_start(LOG_ERROR);
        printf("No such member '%s' on ", name);
        print_type(struct_t);
        printf("\n");
        exit(1);
    }
    return ((void *)0);
}
Node *p_parse_switch_statement(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_SWITCH);
    array_init(&node->_switch.cases_array, 4, sizeof(Node *));
    p_consume_a(p, TK_SWITCH);
    p_consume_a(p, TK_OPEN_PAREN);
    node->_switch.test = p_parse_expression(p, nm, 0);
    p_consume_a(p, TK_CLOSE_PAREN);
    node->_switch.block = new_compound_node(nm);
    p_consume_a(p, TK_OPEN_CURLY);
    while (p_peek(p)->type != TK_CLOSE_CURLY) {
        Node *item;
        if (p_peek(p)->type == TK_CASE || p_peek(p)->type == TK_DEFAULT) {
            item = p_parse_case(p, nm);
            p_append_case(node, item);
        } else {
            item = p_parse_block_item(p, nm);
        }
        p_append_block_item(node->_switch.block, item);
    }
    p_consume_a(p, TK_CLOSE_CURLY);
    return node;
}
Node *p_parse_for_loop(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_FOR);
    p_consume_a(p, TK_FOR);
    p_consume_a(p, TK_OPEN_PAREN);
    p->expect_semi = 0;
    if (p_peek(p)->type != TK_SEMI) node->_for.init = p_parse_block_item(p, nm);
    p_consume_a(p, TK_SEMI);
    if (p_peek(p)->type != TK_SEMI) node->_for.cond = p_parse_expression(p, nm, 0);
    p_consume_a(p, TK_SEMI);
    if (p_peek(p)->type != TK_CLOSE_PAREN) node->_for.iter = p_parse_expression(p, nm, 0);
    p->expect_semi = 1;
    p_consume_a(p, TK_CLOSE_PAREN);
    node->_for.block = p_parse_statement(p, nm);
    return node;
}
Node *p_parse_return(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_RETURN);
    p_consume(p);
    if (p_peek(p)->type != TK_SEMI) {
        node->_return.expr = p_parse_expression(p, nm, 0);
        node->type = type_invalid;
    }
    p_consume_semi(p);
    return node;
}
Node *p_parse_continue(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_CONTINUE);
    p_consume(p);
    p_consume_semi(p);
    return node;
}
Node *p_parse_break(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_BREAK);
    p_consume(p);
    p_consume_semi(p);
    return node;
}
Node *p_parse_statement(Parser *p, NodeManager *nm) {
    switch (p_peek(p)->type) {
    case TK_IF:
        return p_parse_if_statement(p, nm);
    case TK_DO:
        return p_parse_do_while_loop(p, nm);
    case TK_WHILE:
        return p_parse_while_loop(p, nm);
    case TK_FOR:
        return p_parse_for_loop(p, nm);
    case TK_SWITCH:
        return p_parse_switch_statement(p, nm);
    case TK_CONTINUE:
        return p_parse_continue(p, nm);
    case TK_BREAK:
        return p_parse_break(p, nm);
    case TK_RETURN:
        return p_parse_return(p, nm);
    case TK_MULTIPLY:
    case TK_IDENTIFIER:
        if (p_peek_next(p)->type == TK_COLON) return p_parse_label(p, nm);
        Node *n = p_parse_expression(p, nm, 0);
        p_consume_semi(p);
        return n;
    case TK_OPEN_CURLY:
        return p_parse_compound(p, nm);
    case TK_GOTO:
        return p_parse_goto_statement(p, nm);
    default:
        return p_parse_expression(p, nm, 0);
    }
}
void p_push_scope(Parser *p) {
    p->current_scope_depth++;
    p_append_symbol_table(p);
}
void p_pop_scope(Parser *p) {
    p->current_scope_depth--;
    array_free(get_current_symbol_table(p));
    pop(&p->scopes_array);
}
Node *p_parse_compound(Parser *p, NodeManager *nm) {
    Node *node = new_compound_node(nm);
    p_consume_a(p, TK_OPEN_CURLY);
    while (p_peek(p)->type != TK_CLOSE_CURLY && !p_is_last_token(p)) {
        p_append_block_item(node, p_parse_block_item(p, nm));
    }
    p_consume_a(p, TK_CLOSE_CURLY);
    return node;
}
Type *decay_array_type(Type *t) {
    if (t->kind == T_ARRAY) return get_pointer_type(decay_array_type(t->base));
    return t;
}
Modifier p_parse_parameter_list(Parser *p, NodeManager *nm) {
    Modifier mod = {.kind = MOD_FUNCTION, .function = {.is_variadic = 0}};
    array_init(&mod.function.params, 4, sizeof(ParamDecl));
    p_consume_a(p, TK_OPEN_PAREN);
    while (p_peek(p)->type != TK_CLOSE_PAREN && !p_is_last_token(p)) {
        if (p_peek(p)->type == TK_ELLIPSES) {
            p_consume(p);
            mod.function.is_variadic = 1;
            break;
        }
        const char *name = ((void *)0);
        append(&mod.function.params, &(ParamDecl){.type = decay_array_type(p_parse_type(p, nm, &name)), .name = name});
        if (p_peek(p)->type == TK_COMMA) p_consume(p);
        else break;
    }
    p_consume_a(p, TK_CLOSE_PAREN);
    return mod;
}
Node *p_parse_function(Parser *p, NodeManager *nm, Type *type, const char *name, const StorageClass storage_class, int is_inline) {
    Node *node = new_function_node(nm);
    if (!(name != ((void *)0))) do { log_message(LOG_ERROR, "Expected nonnull name of function\n"); exit(1); } while (0);
    node->func.name = name;
    node->func.is_inline = is_inline;
    node->type = type;
    if (p_peek(p)->type == TK_SEMI) {
        p_consume(p);
        node->func.is_defined = 0;
    } else {
        node->func.is_defined = 1;
    }
    node->func.storage_class = storage_class;
    if (node->func.is_defined) node->func.body = p_parse_compound(p, nm);
    return node;
}
Node *p_parse_decl_identifier(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_IDENTIFIER);
    const int expect_closing_paren = p_peek(p)->type == TK_OPEN_PAREN;
    if (expect_closing_paren) p_consume(p);
    const Token *t = p_consume_a(p, TK_IDENTIFIER);
    node->identifier.name = t->value;
    node->identifier.len = t->size;
    if (expect_closing_paren) p_consume_a(p, TK_CLOSE_PAREN);
    return node;
}
Node *p_parse_declaration(Parser *p, NodeManager *nm, Type *type, const char *name, const StorageClass storage_class, const int global) {
    if (type->kind == T_STRUCT || type->kind == T_ENUM || type->kind == T_UNION) {
        if (name == ((void *)0)) {
            p_consume_semi(p);
            Node *type_decl = new_node(nm, N_TYPE);
            type_decl->type = type;
            return type_decl;
        }
        if (!(type->_struct.complete)) do { log_message(LOG_ERROR, "Cannot instantiate an incomplete type\n"); exit(1); } while (0);
    }
    if (!(name != ((void *)0))) do { log_message(LOG_ERROR, "Expected non null name of variable\n"); exit(1); } while (0);
    Node *ident = new_node(nm, N_IDENTIFIER);
    ident->identifier.name = name;
    Node *var_decl = new_node(nm, N_VAR_DECL);
    var_decl->var_decl.identifier = ident;
    var_decl->var_decl.is_global = global;
    var_decl->type = type;
    if (p_peek(p)->type == TK_EQ) {
        p_consume(p);
        var_decl->var_decl.expr = p_parse_expression(p, nm, 0);
        var_decl->var_decl.is_defined = 1;
    } else {
        var_decl->var_decl.is_defined = 0;
        var_decl->var_decl.expr = ((void *)0);
    }
    var_decl->var_decl.storage_class = storage_class;
    p_consume_semi(p);
    return var_decl;
}
Node *p_parse_external_declaration(Parser *p, NodeManager *nm) {
    if (p_peek(p)->type == TK_TYPEDEF) return p_parse_typedef(p, nm);
    StorageClass storage_class = NONE;
    int is_inline = 0;
    for (;;) {
        if (p_peek(p)->type == TK_STATIC) storage_class = STATIC;
        else if (p_peek(p)->type == TK_EXTERN) storage_class = EXTERN;
        else if (p_peek(p)->type == TK_INLINE) is_inline = 1;
        else break;
        p_consume(p);
    }
    const char *name = ((void *)0);
    Type *type = p_parse_type(p, nm, &name);
    if (type->kind == T_FUNCTION) return p_parse_function(p, nm, type, name, storage_class, is_inline);
    else return p_parse_declaration(p, nm, type, name, storage_class, 1);
}
Node *p_parse_typedef(Parser *p, NodeManager *nm) {
    p_consume_a(p, TK_TYPEDEF);
    Node *node = new_node(nm, N_TYPEDEF);
    node->_typedef.name = ((void *)0);
    node->type = p_parse_type(p, nm, &node->_typedef.name);
    if (!(node->_typedef.name)) do { log_message(LOG_ERROR, "Missing typedef name\n"); exit(1); } while (0);
    p_consume_semi(p);
    p_append_typedef(p, &(Typedef){.type = node->type, .new_def = node->_typedef.name});
    return node;
}
StorageClass p_parse_storage_classifier(Parser *p, NodeManager *nm) {
    switch (p_peek(p)->type) {
    case TK_EXTERN:
        p_consume(p);
        return EXTERN;
    case TK_STATIC:
        p_consume(p);
        return STATIC;
    default:
        return NONE;
    }
}
Node *p_parse_block_declaration(Parser *p, NodeManager *nm) {
    if (p_peek(p)->type == TK_TYPEDEF) return p_parse_typedef(p, nm);
    StorageClass storage_class = p_parse_storage_classifier(p, nm);
    const char *name = ((void *)0);
    Type *type = p_parse_type(p, nm, &name);
    if (type->kind == T_FUNCTION) {
        do { log_message(LOG_ERROR, "Function prototypes within block scope is unsupported\n"); exit(1); } while (0);
    } else return p_parse_declaration(p, nm, type, name, storage_class, 0);
}
Node *p_parse_translation_unit(Parser *p, NodeManager *nm) {
    Node *root = init_translation_unit(nm);
    if (!(p->size > 0)) do { log_message(LOG_ERROR, "Tried to generate AST with empty token array."); exit(1); } while (0);
    while (!p_is_last_token(p)) {
        Node *decl = p_parse_external_declaration(p, nm);
        append(&root->translation_unit.declarations_array, &decl);
    }
    return root;
}
int is_storage_classifier(const TokenType type) {
    switch (type) {
    case TK_STATIC:
    case TK_EXTERN:
        return 1;
    default:
        return 0;
    }
}
int is_qualifier_token(const TokenType type) {
    switch (type) {
    case TK_CONST:
    case TK_VOLATILE:
        return 1;
    default:
        return 0;
    }
}
int is_start_of_type(const Parser *p, const Token *tk) {
    return tk->type == TK_UNSIGNED || tk->type == TK_SIGNED || is_type_token(p, tk) || is_qualifier_token(tk->type);
}
int is_type_token(const Parser *p, const Token *t) {
    switch (t->type) {
    case TK_CHAR:
    case TK_SHORT:
    case TK_INT:
    case TK_LONG:
    case TK_FLOAT:
    case TK_DOUBLE:
    case TK_VOID:
    case TK_STRUCT:
    case TK_ENUM:
        return 1;
    case TK_IDENTIFIER:
        return p_get_symbol(p, t->value, TYPEDEF, 0) != ((void *)0);
    default:
        return 0;
    }
}
Type *token_to_type(Parser *p, const Token *t) {
    switch (t->type) {
    case TK_CHAR:
        return type_i8;
    case TK_SHORT:
        return type_i16;
    case TK_INT:
        return type_i32;
    case TK_LONG:
        return type_i64;
    case TK_FLOAT:
        return type_f32;
    case TK_DOUBLE:
        return type_f64;
    case TK_VOID:
        return type_void;
    case TK_IDENTIFIER:
        return p_get_typedef(p, t->value)->type;
    default:
        return type_invalid;
    }
}
Type *parse_int_suffix(const char *raw, int *len) {
    int i = *len - 1;
    int l_count = 0;
    int is_unsigned = 0;
    for (;;) {
        if (i <= 0) break;
        char c = raw[i];
        if (c == 'u' || c == 'U') is_unsigned = 1;
        else if (c == 'l' || c == 'L') l_count++;
        else break;
        i--;
    }
    Type *type = type_i32;
    *len -= *len - i - 1;
    if (l_count > 0) type = type_i64;
    if (is_unsigned) return get_unsigned_type(type);
    return type;
}
Type *parse_float_suffix(const char *raw, int *len) {
    Type *type = type_f64;
    switch (raw[*len - 1]) {
    case 'f':
    case 'F':
        type = type_f32;
    case 'l':
    case 'L':
        *len -= 1;
    default:
        return type;
    }
}
int64_t parse_int(const char *raw, int len) {
    if (len > 20) {
        do { log_message(LOG_ERROR, "Cannot parse an integer larger than 64 bytes\n"); exit(1); } while (0);
    }
    if (raw[0] == '-') return -parse_int(raw + 1, len - 1);
    if (raw[0] == '0' && len > 1) {
        switch (raw[1]) {
        case 'x':
        case 'X':
            return parse_hex(raw + 2, len - 2);
        case 'b':
        case 'B':
            return parse_binary(raw + 2, len - 2);
        default:
            return parse_oct(raw + 1, len - 1);
        }
    }
    return parse_dec(raw, len);
}
int64_t parse_dec(const char *raw, int len) {
    int64_t res = 0;
    const char *start = raw;
    while (raw < start + len) {
        res = res * 10 + (*raw - '0');
        raw++;
    }
    return res;
}
int64_t parse_binary(const char *raw, int len) {
    int64_t res = 0;
    const char *start = raw;
    while (raw < start + len) {
        int value = (*raw - '0');
        if (value > 1) {
            do { log_message(LOG_ERROR, "Parse Binary Failed: digit cannot be larger than 1\n"); exit(1); } while (0);
        }
        res = res * 2 + value;
        raw++;
    }
    return res;
}
int64_t parse_oct(const char *raw, int len) {
    int64_t res = 0;
    const char *start = raw;
    while (raw < start + len) {
        int value = (*raw - '0');
        if (value > 7) {
            do { log_message(LOG_ERROR, "Parse Octal Failed: digit cannot be larger than 7\n"); exit(1); } while (0);
        }
        res = res * 8 + value;
        raw++;
    }
    return res;
}
int64_t parse_hex(const char *raw, int len) {
    int64_t res = 0;
    const char *start = raw;
    while (raw < start + len) {
        int value;
        if (is_num(*raw)) value = *raw - '0';
        char c = *raw | 0x20;
        if (c <= 'f' && c >= 'a') value = c - 'a' + 10;
        if (c <= 'F' && c >= 'A') value = c - 'A' + 10;
        res = res * 16 + value;
        raw++;
    }
    return res;
}
int parse_multi_character(const char *raw, int len) {
    int c = 0;
    for (int i = 0; i < len && i < 4; i++) {
        c |= (unsigned char)raw[i] << (i * 8);
    }
    return c;
}
double parse_float(const char *raw, int len) {
    if (raw[0] == '-') return -parse_float(raw + 1, len - 1);
    double res = 0;
    const char *end = raw + len;
    double m = 0;
    while (raw < end) {
        if (*raw == '.') {
            m = 0.1;
            raw++;
            continue;
        }
        int digit = *raw - '0';
        if (m) {
            res += digit * m;
            m *= 0.1;
        } else {
            res = res * 10.0 + digit;
        }
        raw++;
    }
    return res;
}
