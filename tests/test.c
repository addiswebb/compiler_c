#include "stdarg.h"
typedef long int64_t;
typedef unsigned long size_t;
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
void free(void *);
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
    const char *stage_str;
    switch (logger.stage) {
    case STAGE_COMPILER:
        stage_str = "Compiler";
        break;
    case STAGE_TOKENIZING:
        stage_str = "Tokenizer";
        break;
    case STAGE_PARSING:
        stage_str = "Parser";
        break;
    case STAGE_IR:
        stage_str = "IR";
        break;
    case STAGE_X86_GEN:
        stage_str = "x86 Gen";
        break;
    case STAGE_SEMA_ANALYSIS:
        stage_str = "Semantic Analysis";
        break;
    case STAGE_ASSEMBLER:
        stage_str = "Assembler";
        break;
    case STAGE_LINKER:
        stage_str = "Linker";
        break;
    default:
        break;
    }
    fprintf(logger.file, "[%s] %s: ", level_str, stage_str);
}
static inline void log_message(LogLevel lvl, const char *fmt, ...) {
    if (lvl < logger.min_level) return;
    log_start(lvl);
    va_list args;
    va_start(args, fmt);
    vfprintf(logger.file, fmt, args);
    va_end(args);
    fflush(logger.file);
    if (lvl == LOG_PANIC) exit(1);
}
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
typedef int bool;
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
            bool is_variadic;
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
    bool is_signed;
    unsigned int qualifiers;
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
            bool is_complete;
            union {
                int64_t array_len;
                Node *const_expr;
            };
        } _array;
        struct {
            bool complete;
            char *name;
            Array members_array;
        } _struct;
        struct {
            bool complete;
            char *name;
            Array fields_array;
        } _enum;
        struct {
            bool complete;
            char *name;
            Array members_array;
        } _union;
        struct {
            Type *return_type;
            Array params;
            bool is_variadic;
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
Type *init_global_type(TypeKind type, int size, unsigned int qualifiers, bool is_signed);
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
Type *get_function_type(Type *type, Array params, bool is_variadic);
Type *get_modified_type(Type *type, Declarator *decl);
Type *get_unsigned_type(Type *type);
bool cmp_func_types(const Type *a, const Type *b);
Type *promote_integer(Type *from, Type *to);
Type *infer_array_length(Type *arr_type, int len);
void append_union_member(Type *u, UnionMember *m);
void append_enum_field(Type *e, EnumField *f);
void append_struct_member(Type *s, StructMember *f);
Type union_type();
Type struct_type();
Type enum_type();
StructMember *get_member(Type *struct_t, const char *name, bool is_root);
bool is_func_ptr(Type *t);
void print_type(const Type *type);
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
bool is_postfix_operator(const TokenType type);
bool is_unary_operator(const TokenType type);
bool is_binary_operator(const TokenType type);
bool is_assignment_op(const TokenType type);
bool is_arithmetic_op(const TokenType type);
bool is_bitwise_op(const TokenType type);
bool is_comparison_op(const TokenType type);
bool is_logical_op(const TokenType type);
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
char *strcpy(char *, const char *);
char *strncpy(char *, const char *, size_t);
int strcmp(const char *, const char *);
int strncmp(const char *, const char *, size_t);
size_t strlen(const char *);
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
            bool is_global;
            bool is_defined;
            bool is_inline;
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
            bool is_do_while;
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
            bool is_global;
            bool is_defined;
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
                    bool is_complete;
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
void init_typepool();
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
bool is_valid_cast(const Type *from, const Type *to);
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
    bool expect_semi;
    Array scopes_array;
    Arena symbols_arena;
} Parser;
Parser new_parser();
void init_parser(Parser *p, Array *src, int size);
void free_parser(Parser *p);
bool p_is_last_token(const Parser *p);
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
bool is_type_token(const Parser *p, const Token *t);
bool is_storage_classifier(const TokenType type);
bool is_qualifier_token(const TokenType type);
bool is_start_of_type(const Parser *p, const Token *t);
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
Node *p_parse_function(Parser *p, NodeManager *nm, Type *type, const char *name, const StorageClass storage_class, bool is_inline);
Modifier p_parse_parameter_list(Parser *p, NodeManager *nm);
Node *p_parse_external_declaration(Parser *p, NodeManager *nm);
Node *p_parse_block_declaration(Parser *p, NodeManager *nm);
Node *p_parse_decl_identifier(Parser *p, NodeManager *nm);
Node *p_parse_declaration(Parser *p, NodeManager *nm, Type *type, const char *name, const StorageClass storage_class, const bool global);
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
Symbol *p_get_symbol(const Parser *p, const char *name, const SymbolKind kind, const bool same_depth);
Node *p_get_func_def(const Parser *p, const char *name);
Typedef *p_get_typedef(const Parser *p, const char *name);
Node *p_get_var_decl(const Parser *p, const char *name);
const EnumField *p_get_enum_const(const Parser *p, const char *name);
Symbol *p_new_symbol(Parser *p, const Symbol *s);
void p_append_call_param(Node *func_call, Node *param);
void p_append_param(Node *func, Node *param);
void p_append_enum_const(Parser *p, const EnumField *e);
Symbol *p_append_var_decl_symbol(Parser *p, Node *v);
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
UnionMember *get_union_member_named(const Type *union_t, const char *name);
StructMember *get_struct_member_named(const Type *struct_t, const char *name, int *index);
typedef struct {
    char *output;
    Array current_source;
    Array current_output;
    Array source_files;
    Array passthrough_args;
    char *src;
    int src_size;
    Tokenizer tk;
    NodeManager nm;
    Parser p;
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
    CF_COUNT,
} CompilerFlag;
extern unsigned int compiler_flags;
extern const char *flag_strings[CF_COUNT];
bool has_flag(CompilerFlag f);
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
int main(const int argc, char *argv[]) {
    printf("COMPILER_C compiled this oMG\n");
    init_logger(stdout, LOG_DEBUG);
    Compiler compiler = begin_compiler(argc, argv);
    drive(&compiler);
    free_compiler(&compiler);
    return 0;
}
