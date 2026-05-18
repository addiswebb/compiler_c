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
    bool is_resolved;
    bool printing;
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
bool is_func_ptr(const Type *t);
bool is_scalar_type(const Type *t);
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
bool is_start_of_type(const Parser *p, const Token *tk);
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
Type *type_i8;
Type *type_i16;
Type *type_i32;
Type *type_i64;
Type *type_u8;
Type *type_u16;
Type *type_u32;
Type *type_u64;
Type *type_f32;
Type *type_f64;
Type *type_void;
Type *type_void_ptr;
Type *type_invalid;
Arena typepool;
void init_typepool() {
    arena_init(&typepool, 2, sizeof(Type));
    type_i8 = init_global_type(T_INT, sizeof(char), QUAL_NONE, 1);
    type_i16 = init_global_type(T_INT, sizeof(short), QUAL_NONE, 1);
    type_i32 = init_global_type(T_INT, sizeof(int), QUAL_NONE, 1);
    type_i64 = init_global_type(T_INT, sizeof(int64_t), QUAL_NONE, 1);
    type_u8 = init_global_type(T_INT, sizeof(char), QUAL_NONE, 0);
    type_u16 = init_global_type(T_INT, sizeof(short), QUAL_NONE, 0);
    type_u32 = init_global_type(T_INT, sizeof(int), QUAL_NONE, 0);
    type_u64 = init_global_type(T_INT, sizeof(int64_t), QUAL_NONE, 0);
    type_f32 = init_global_type(T_FLOAT, sizeof(float), QUAL_NONE, 1);
    type_f64 = init_global_type(T_FLOAT, sizeof(double), QUAL_NONE, 1);
    type_void = init_global_type(T_VOID, sizeof(void), QUAL_NONE, 1);
    type_void_ptr = get_pointer_type(type_void);
    type_invalid = init_global_type(T_INVALID, -1, QUAL_NONE, 1);
}
void free_typepool() {
    for (int i = 0; i < typepool.count; i++) {
        Type *t = arena_get(&typepool, i);
        switch (t->kind) {
        case T_STRUCT:
            array_free(&t->_struct.members_array);
            break;
        case T_ENUM:
            array_free(&t->_enum.fields_array);
            break;
        case T_UNION:
            array_free(&t->_union.members_array);
            break;
        case T_FUNCTION:
            array_free(&t->_func.params);
            break;
        default:
            break;
        }
    }
    arena_free(&typepool);
}
Type *init_global_type(TypeKind type, int size, unsigned int qualifiers, bool is_signed) {
    Type *t = new_type();
    t->kind = type;
    t->size = size;
    t->align = size;
    t->base = ((void *)0);
    t->qualifiers = qualifiers;
    t->is_signed = is_signed;
    t->is_resolved = 1;
    return t;
}
Type *new_type() { return arena_append(&typepool, &(Type){0}); }
Type *new_incomplete_array_type(Type *type, Node *const_expr) {
    Type *arr_type = new_type();
    arr_type->kind = T_ARRAY;
    arr_type->size = type->size;
    arr_type->align = type->align;
    arr_type->base = type;
    arr_type->_array.const_expr = const_expr;
    arr_type->is_signed = 1;
    arr_type->qualifiers = QUAL_NONE;
    arr_type->is_resolved = 0;
    return arr_type;
}
Type *new_array_type(Type *type, int len) {
    Type *arr_type = new_type();
    arr_type->kind = T_ARRAY;
    arr_type->size = type->size * len;
    arr_type->align = type->align;
    arr_type->base = type;
    arr_type->_array.array_len = len;
    arr_type->is_signed = 1;
    arr_type->qualifiers = QUAL_NONE;
    arr_type->is_resolved = 1;
    return arr_type;
}
Type *infer_array_length(Type *arr_type, int len) {
    arr_type->_array.array_len = len;
    arr_type->size = len * arr_type->base->size;
    arr_type->is_resolved = 1;
    return arr_type;
}
Type *new_function_type(Type *type, Array params, bool is_variadic) {
    Type *fn_type = new_type();
    fn_type->kind = T_FUNCTION;
    fn_type->size = sizeof(void);
    fn_type->align = sizeof(void);
    fn_type->abi.type = ((void *)0);
    fn_type->abi.gp_count = 0;
    fn_type->abi.fp_count = 0;
    fn_type->_func.return_type = type;
    array_init(&fn_type->_func.params, params.capacity, params.element_size);
    memcpy(fn_type->_func.params.data, params.data, params.count * params.element_size);
    fn_type->_func.params.count = params.count;
    fn_type->_func.is_variadic = is_variadic;
    return fn_type;
}
Type *new_pointer_type(Type *type) {
    Type *ptr_type = new_type();
    ptr_type->kind = T_POINTER;
    ptr_type->size = sizeof(void *);
    ptr_type->align = ptr_type->size;
    ptr_type->base = type;
    ptr_type->is_signed = 1;
    ptr_type->qualifiers = QUAL_NONE;
    ptr_type->is_resolved = type->is_resolved;
    return ptr_type;
}
Type *new_qualified_type(Type *type, unsigned int qualifiers) {
    Type *qual_type = new_type();
    *qual_type = *type;
    switch (type->kind) {
    case T_ENUM:
    case T_STRUCT:
        array_init(&qual_type->_struct.members_array, type->_struct.members_array.count, sizeof(StructMember));
        memcpy(qual_type->_struct.members_array.data, type->_struct.members_array.data,
               type->_struct.members_array.count * sizeof(StructMember));
        qual_type->_struct.members_array.count = type->_struct.members_array.count;
        break;
    case T_UNION:
        array_init(&qual_type->_enum.fields_array, type->_enum.fields_array.count, sizeof(EnumField));
        memcpy(qual_type->_enum.fields_array.data, type->_enum.fields_array.data, type->_enum.fields_array.count * sizeof(EnumField));
        qual_type->_enum.fields_array.count = type->_enum.fields_array.count;
        break;
    case T_FUNCTION:
        array_init(&qual_type->_func.params, type->_func.params.count, sizeof(ParamDecl));
        memcpy(qual_type->_func.params.data, type->_func.params.data, type->_func.params.count * sizeof(ParamDecl));
        qual_type->_func.params.count = type->_func.params.count;
        break;
    default:
        break;
    }
    qual_type->qualifiers = qualifiers;
    qual_type->is_resolved = type->is_resolved;
    return qual_type;
}
Type *new_unsigned_type(Type *type) {
    log_message(LOG_WARN, "Creating new unsigned type\n");
    Type *unsigned_type = new_type();
    unsigned_type->kind = type->kind;
    unsigned_type->size = type->size;
    unsigned_type->align = type->align;
    unsigned_type->is_signed = 0;
    unsigned_type->qualifiers = type->qualifiers;
    unsigned_type->base = type;
    unsigned_type->is_resolved = type->is_resolved;
    return unsigned_type;
}
Type *get_float_type(int size) {
    if (!(size > 0 && size > 8)) do { log_message(LOG_ERROR, "Size must be between [1-8]"); exit(1); } while (0);
    if (size <= 4) return type_f32;
    else return type_f64;
}
Type *get_integer_type(int size) {
    switch (size) {
    case 1:
        return type_i8;
    case 2:
        return type_i16;
    case 3:
    case 4:
        return type_i32;
    case 5:
    case 6:
    case 7:
    case 8:
        return type_i64;
    default:
        do { log_message(LOG_ERROR, "Invalid integer size\n"); exit(1); } while (0);
    }
}
Type *promote_integer(Type *from, Type *to) { return from->is_signed ? to : get_unsigned_type(to); }
Type *get_pointer_type(Type *type) {
    for (int i = 0; i < typepool.count; i++) {
        Type *t = arena_get(&typepool, i);
        if (t->kind == T_POINTER && t->base == type) return t;
    }
    return new_pointer_type(type);
}
Type *get_array_type(Type *type, int len) {
    for (int i = 0; i < typepool.count; i++) {
        Type *t = arena_get(&typepool, i);
        if (!t->is_resolved) continue;
        if (t->base == type && t->kind == T_ARRAY && t->_array.array_len == len) return t;
    }
    return new_array_type(type, len);
}
Type *get_function_type(Type *type, Array params, bool is_variadic) {
    if (!(type->kind != T_ARRAY)) do { log_message(LOG_ERROR, "Functions cannot return arrays\n"); exit(1); } while (0);
    for (int i = 0; i < typepool.count; i++) {
        Type *t = arena_get(&typepool, i);
        if (t->kind == T_FUNCTION && t->_func.return_type == type && t->_func.is_variadic == is_variadic) {
            if (t->_func.params.count != params.count) continue;
            bool match = 1;
            for (int j = 0; j < params.count; j++) {
                ParamDecl *p_a = get(&params, j);
                ParamDecl *p_b = get(&t->_func.params, j);
                if (p_a->type != p_b->type) {
                    match = 0;
                    break;
                }
                if (p_a->name != p_b->name) {
                    match = 0;
                    break;
                }
            }
            if (match) {
                return t;
            }
        }
    }
    return new_function_type(type, params, is_variadic);
}
Type *get_modified_type(Type *type, Declarator *decl) {
    if (decl->modifiers.count == 0) return type;
    for (int i = decl->modifiers.count - 1; i >= 0; i--) {
        Modifier *mod = (Modifier *)get(&decl->modifiers, i);
        if (mod->kind == MOD_POINTER) {
            type = get_pointer_type(type);
        } else if (mod->kind == MOD_ARRAY) {
            type = new_incomplete_array_type(type, mod->array_bounds);
        } else if (mod->kind == MOD_FUNCTION) {
            type = get_function_type(type, mod->function.params, mod->function.is_variadic);
        }
    }
    return type;
}
Type *get_qualified_type(Type *type, unsigned int qualifiers) {
    for (int i = 0; i < typepool.count; i++) {
        Type *t = arena_get(&typepool, i);
        if (t->base == type->base && t->kind == type->kind && t->size == type->size && t->qualifiers == qualifiers &&
            t->is_signed == type->is_signed && t->is_resolved) {
            return t;
        }
    }
    return new_qualified_type(type, qualifiers);
}
bool cmp_func_types(const Type *a, const Type *b) {
    if (!(a->kind == T_FUNCTION)) do { log_message(LOG_ERROR, "Can only compare function types\n"); exit(1); } while (0);
    if (!(b->kind == T_FUNCTION)) do { log_message(LOG_ERROR, "Can only compare function types\n"); exit(1); } while (0);
    if (a->_func.is_variadic != b->_func.is_variadic) return 0;
    if (a->_func.return_type != b->_func.return_type) return 0;
    if (a->_func.params.data != b->_func.params.data) {
        if (a->_func.params.count != b->_func.params.count) return 0;
        for (int i = 0; i < a->_func.params.count; i++) {
            ParamDecl *a_p = get(&a->_func.params, i);
            ParamDecl *b_p = get(&b->_func.params, i);
            if (a_p->type != b_p->type) return 0;
        }
    }
    return 1;
}
Type *get_unsigned_type(Type *type) {
    if (type->kind != T_INT) {
        do { log_message(LOG_ERROR, "Cannot retrieve signed/unsigned variant of non-integer type\n"); exit(1); } while (0);
    }
    if (type->is_signed == 0) return type;
    for (int i = 0; i < typepool.count; i++) {
        Type *t = arena_get(&typepool, i);
        if (t->kind == type->kind && t->size == type->size && t->is_signed == 0 && t->qualifiers == type->qualifiers) {
            return t;
        }
    }
    return new_unsigned_type(type);
}
Type *get_enum_type(const char *name) {
    if (name == ((void *)0)) return ((void *)0);
    for (int i = 0; i < typepool.count; i++) {
        Type *type = arena_get(&typepool, i);
        if (type->kind == T_ENUM && type->_enum.name != ((void *)0) && strcmp(name, type->_enum.name) == 0) return type;
    }
    return ((void *)0);
}
Type *get_union_type(const char *name) {
    if (name == ((void *)0)) return ((void *)0);
    for (int i = 0; i < typepool.count; i++) {
        Type *type = arena_get(&typepool, i);
        if (type->kind == T_UNION && type->_union.name != ((void *)0) && strcmp(name, type->_union.name) == 0) return type;
    }
    return ((void *)0);
}
Type *get_struct_type(const char *name) {
    if (name == ((void *)0)) return ((void *)0);
    for (int i = 0; i < typepool.count; i++) {
        Type *type = arena_get(&typepool, i);
        if (type->kind == T_STRUCT && type->_struct.name != ((void *)0) && strcmp(name, type->_struct.name) == 0) return type;
    }
    return ((void *)0);
}
void append_enum_field(Type *e, EnumField *f) { append(&e->_enum.fields_array, f); }
void append_union_member(Type *u, UnionMember *m) {
    if (m->type->size > u->size) u->size = align(m->type->size, m->type->align);
    append(&u->_union.members_array, m);
}
void append_struct_member(Type *s, StructMember *m) {
    if (m->type->align > s->align) s->align = m->type->align;
    s->size = align(s->size, m->type->align);
    m->offset = s->size;
    append(&s->_struct.members_array, m);
    s->size += m->type->size;
}
Type union_type() {
    Type u = {};
    u.kind = T_UNION;
    u.base = ((void *)0);
    u.align = 0;
    u.size = 0;
    u.is_signed = 1;
    u.qualifiers = QUAL_NONE;
    u._union.complete = 0;
    u._union.name = ((void *)0);
    u._union.members_array.capacity = 0;
    u._union.members_array.count = 0;
    u._union.members_array.element_size = -1;
    u._union.members_array.data = ((void *)0);
    u.is_resolved = 0;
    return u;
}
Type struct_type() {
    Type s = {};
    s.kind = T_STRUCT;
    s.base = ((void *)0);
    s.align = 0;
    s.size = 0;
    s.is_signed = 1;
    s.qualifiers = QUAL_NONE;
    s._struct.complete = 0;
    s._struct.name = ((void *)0);
    s._struct.members_array.capacity = 0;
    s._struct.members_array.count = 0;
    s._struct.members_array.element_size = -1;
    s._struct.members_array.data = ((void *)0);
    s.is_resolved = 0;
    return s;
}
Type enum_type() {
    Type e = {};
    e.kind = T_ENUM;
    e.base = type_i32;
    e.align = 4;
    e.size = 4;
    e.is_signed = 1;
    e.qualifiers = NONE;
    e._enum.complete = 0;
    e._enum.name = ((void *)0);
    e._enum.fields_array.capacity = 0;
    e._enum.fields_array.count = 0;
    e._enum.fields_array.element_size = -1;
    e._enum.fields_array.data = ((void *)0);
    e.is_resolved = 0;
    return e;
}
AggrMember *get_member(Type *struct_t, const char *name, bool is_root) {
    for (int i = 0; i < struct_t->_struct.members_array.count; i++) {
        AggrMember *member = get_struct_member(struct_t, i);
        if (member->name) {
            if (strcmp(name, member->name) == 0) return member;
        } else if (member->type->kind == T_STRUCT || member->type->kind == T_UNION) {
            AggrMember *x = get_member(member->type, name, 0);
            if (x) return x;
        }
    }
    if (is_root) do { log_message(LOG_ERROR, "No member named \"%s\" in struct %s\n", name, struct_t->_struct.name); exit(1); } while (0);
    else return ((void *)0);
}
bool is_func_ptr(const Type *t) { return t->kind == T_POINTER && t->base->kind == T_FUNCTION; }
bool is_scalar_type(const Type *t) { return t->kind == T_INT || t->kind == T_FLOAT || t->kind == T_ENUM || t->kind == T_POINTER; }
void print_type(Type *type) {
    if (!type) {
        printf("NULL");
        return;
    }
    if (type->printing) {
        printf("<recursive>\n");
        return;
    }
    type->printing = 1;
    if (type->qualifiers & QUAL_CONST) printf("%s ", KEYWORDS[TK_CONST]);
    if (type->qualifiers & QUAL_VOLATILE) printf("%s ", KEYWORDS[TK_VOLATILE]);
    if (type->kind == T_INT && !type->is_signed) printf("%s ", KEYWORDS[TK_UNSIGNED]);
    switch (type->kind) {
    case T_INVALID:
        printf("[#]");
        break;
    case T_ARRAY:
        print_type(type->base);
        if (type->is_resolved) printf("[%" "ld" "]", type->_array.array_len);
        else printf("[!!!]");
        break;
    case T_INT:
        switch (type->size) {
        case 1:
            print_token_type(TK_CHAR);
            break;
        case 2:
            print_token_type(TK_SHORT);
            break;
        case 4:
            print_token_type(TK_INT);
            break;
        case 8:
            print_token_type(TK_LONG);
            break;
        default:
            do { log_message(LOG_ERROR, "Tried to type of int, with invalid size\n"); exit(1); } while (0);
        }
        break;
    case T_FLOAT:
        switch (type->size) {
        case 4:
            print_token_type(TK_FLOAT);
            break;
        case 8:
            print_token_type(TK_DOUBLE);
            break;
        default:
            do { log_message(LOG_ERROR, "Tried to type of float, with invalid size\n"); exit(1); } while (0);
        }
        break;
    case T_POINTER:
        print_type(type->base);
        printf("*");
        break;
    case T_STRUCT:
        printf("struct %s ", type->_struct.name);
        if (has_flag(CF_DEBUG_STRUCT)) {
            printf("{");
            for (int i = 0; i < type->_struct.members_array.count; i++) {
                StructMember *member = get_struct_member(type, i);
                print_type(member->type);
                printf(" %s:[%d@%d], ", member->name, member->offset, member->type->size);
            }
            printf("}");
        }
        break;
    case T_ENUM:
        printf("enum %s ", type->_enum.name);
        if (has_flag(CF_DEBUG_ENUM)) {
            printf("{");
            for (int i = 0; i < type->_enum.fields_array.count; i++) {
                EnumField *field = get_enum_field(type, i);
                printf(" %s = %d, ", field->name, field->value);
            }
            printf("}");
        }
        break;
    case T_UNION:
        printf("union %s ", type->_union.name);
        if (has_flag(CF_DEBUG_UNION)) {
            printf("{");
            for (int i = 0; i < type->_union.members_array.count; i++) {
                UnionMember *member = get_union_member(type, i);
                print_type(member->type);
                printf(" %s:[%d] ", member->name, member->type->size);
                if (i < type->_union.members_array.count - 1) printf(", ");
            }
            printf("}");
        }
        break;
    case T_VOID:
        print_token_type(TK_VOID);
        break;
    case T_FUNCTION:
        print_type(type->_func.return_type);
        printf("(");
        if (type->_func.params.count == 0) printf("void");
        for (int i = 0; i < type->_func.params.count; i++) {
            ParamDecl *param = (ParamDecl *)get(&type->_func.params, i);
            print_param_decl(param);
            if (i < type->_func.params.count - 1) printf(", ");
        }
        if (type->_func.is_variadic) printf(", ... ");
        printf(")");
        break;
    }
    type->printing = 0;
}
void print_struct_type(Type *s) {
    printf("struct");
    if (s->_struct.name != ((void *)0)) {
        printf("%s", s->_struct.name);
    }
    if (s->_struct.complete) {
        printf(" {\n");
        for (int i = 0; i < s->_struct.members_array.count; i++) {
            StructMember *member = get_struct_member(s, i);
            printf("    ");
            print_type(member->type);
            printf("; [%d]\n", member->offset);
        }
        printf("}\n");
    }
}
void print_typepool() {
    printf("---- Type Pool -----\n");
    for (int i = 0; i < typepool.count; i++) {
        print_type(arena_get(&typepool, i));
        printf("\n");
    }
}
void print_param_decl(ParamDecl *decl) {
    print_type(decl->type);
    printf(" ");
    if (decl->name) printf("%s", decl->name);
}
