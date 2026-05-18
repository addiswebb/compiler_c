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
typedef int bool;
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
typedef long int64_t;
typedef unsigned long size_t;
typedef short int8_t;
typedef unsigned short uint8_t;
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
void *memcpy(void *, const void *, size_t);
void *memmove(void *, const void *, size_t);
void *memset(void *, int, size_t);
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
bool is_alpha(char c);
bool is_num(char c);
bool is_alpha_num(char c);
bool is_alpha_numeric_str(const char *c);
char to_lower_case(const char c);
bool is_hex(const char c);
bool is_oct(const char c);
bool is_binary(const char c);
bool is_whitespace(char c);
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
    }
    fprintf(logger.file, "[%s] %s: ", level_str, stage_str);
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
    if (lvl == LOG_PANIC) ; exit(1);;
}
const char *KEYWORDS[33] = {
    [TK_AUTO] = "auto", [TK_BREAK] = "break", [TK_CASE] = "case", [TK_CHAR] = "char",
    [TK_CONST] = "const", [TK_CONTINUE] = "continue", [TK_DEFAULT] = "default", [TK_DO] = "do",
    [TK_DOUBLE] = "double", [TK_ELSE] = "else", [TK_ENUM] = "enum", [TK_EXTERN] = "extern",
    [TK_FLOAT] = "float", [TK_FOR] = "for", [TK_GOTO] = "goto", [TK_IF] = "if",
    [TK_INLINE] = "inline", [TK_INT] = "int", [TK_LONG] = "long", [TK_REGISTER] = "register",
    [TK_RETURN] = "return", [TK_SHORT] = "short", [TK_SIGNED] = "signed", [TK_SIZEOF] = "sizeof",
    [TK_STATIC] = "static", [TK_STRUCT] = "struct", [TK_SWITCH] = "switch", [TK_TYPEDEF] = "typedef",
    [TK_UNION] = "union", [TK_UNSIGNED] = "unsigned", [TK_VOID] = "void", [TK_VOLATILE] = "volatile",
    [TK_WHILE] = "while",
};
static void t_buffer_reset(Tokenizer *tk) {
    tk->buf.size = 0;
    memset(tk->buf.buf, 0, sizeof(tk->buf.buf));
}
Tokenizer t_new_tokenizer(const char *src, const int src_size) {
    Tokenizer tokenizer = {};
    tokenizer.size = src_size;
    tokenizer.src = src;
    tokenizer.line_n = 1;
    t_buffer_reset(&tokenizer);
    array_init(&tokenizer.tokens_array, 16, sizeof(Token));
    return tokenizer;
}
void free_token(Token *token) {
    free(token->value);
    token->value = ((void *)0);
}
void t_free(Tokenizer *tokenizer) {
    for (int i = 0; i < tokenizer->tokens_array.count; i++) {
        free_token(get_token(&tokenizer->tokens_array, i));
    }
    tokenizer->src = ((void *)0);
    tokenizer->index = 0;
    tokenizer->size = 0;
    array_free(&tokenizer->tokens_array);
}
static bool t_is_eof(const Tokenizer *tk) { return tk->index >= tk->size; }
static char t_peek(const Tokenizer *tk) {
    if (!t_is_eof(tk)) {
        return tk->src[tk->index];
    }
    printf("T_peek Tried peeking past eof\n");
    return '\0';
}
static char t_peek_n(const Tokenizer *tk, const int n) {
    if (tk->index + n > tk->size) {
        printf("t_peek_n Tried peeking past eof\n");
        return '\0';
    }
    return tk->src[tk->index + n];
}
static char t_peek_next(const Tokenizer *tk) { return t_peek_n(tk, 1); }
static void t_skip(Tokenizer *tk) {
    if (!(tk->index + 1 <= tk->size)) do { log_message(LOG_ERROR, "T_Skip Reached end of file\n"); ; exit(1);; } while (0);
    if (t_peek(tk) == '\n') {
        tk->line_n++;
        tk->char_n = 0;
    }
    tk->index += 1;
    tk->char_n += 1;
}
static void t_consume(Tokenizer *tk) {
    tk->buf.buf[tk->buf.size++] = t_peek(tk);
    if (has_flag(CF_DEBUG_TOKENIZER)) {
        printf("%c", tk->src[tk->index]);
    }
    t_skip(tk);
}
static void t_consume_n(Tokenizer *tk, const int n) {
    for (int i = 0; i < n; i++) {
        t_consume(tk);
    }
}
static void t_consume_a(Tokenizer *tk, const char c) { tk->buf.buf[tk->buf.size++] = c; }
static void t_skip_n(Tokenizer *tk, int n) {
    for (int i = 0; i < n; i++) t_skip(tk);
}
static void t_push_buffer(Tokenizer *tk, const TokenType type) {
    if (tk->buf.size == 0 && type != TK_STRING_LITERAL) {
        log_message(LOG_WARN, "Tried to push an empty buffer to TokenArray, skipping.\n");
        return;
    }
    char *buf_dupe = malloc(sizeof(char) * tk->buf.size);
    if (!buf_dupe) {
        do { log_message(LOG_ERROR, "Failed to allocate for buffer duplicate\n"); ; exit(1);; } while (0);
    }
    memcpy(buf_dupe, tk->buf.buf, sizeof(char) * tk->buf.size);
    append(&tk->tokens_array, &(Token){.type = type, .value = buf_dupe, .size = tk->buf.size, .line_n = tk->line_n, .char_n = tk->char_n});
    t_buffer_reset(tk);
}
static void t_parse_and_push_buffer(Tokenizer *tk) {
    if (tk->buf.size == 0) return;
    Token token = {.type = TK_VOID, .value = ((void *)0), .size = 0, .line_n = tk->line_n, .char_n = tk->char_n};
    bool is_keyword = 0;
    for (int i = 0; i < 33; i++) {
        if (strcmp(tk->buf.buf, KEYWORDS[i]) == 0) {
            token.type = (TokenType)i;
            is_keyword = 1;
            break;
        }
    }
    if (!is_keyword) {
        token.type = TK_IDENTIFIER;
        token.value = strndup(tk->buf.buf, tk->buf.size);
        token.size = tk->buf.size;
    }
    append(&tk->tokens_array, &token);
}
static void t_skip_comments(Tokenizer *tk) {
    t_skip(tk);
    if (t_peek(tk) == '/') {
        t_skip(tk);
        while (t_peek(tk) != '\n') {
            t_skip(tk);
        }
        t_skip(tk);
    } else if (t_peek(tk) == '*') {
        t_skip(tk);
        while (t_peek(tk) != '*' && t_peek_next(tk) != '/') {
            t_skip(tk);
        }
        t_skip(tk);
        t_skip(tk);
    }
}
static int is_op_start(const char c) {
    switch (c) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case '=':
    case '!':
    case '~':
    case '<':
    case '>':
    case '|':
    case '&':
    case '^':
    case '.':
        return 1;
    default:
        return 0;
    }
}
static TokenMatch t_match_operator(const Tokenizer *tk) {
    const char next = t_peek_next(tk);
    const int eq = next == '=';
    switch (t_peek(tk)) {
    case '+':
        if (next == '+') return (TokenMatch){TK_INCR, 2};
        return eq ? (TokenMatch){TK_PLUS_EQ, 2} : (TokenMatch){TK_PLUS, 1};
    case '-':
        if (next == '-') return (TokenMatch){TK_DECR, 2};
        if (next == '>') return (TokenMatch){TK_ARROW, 2};
        return eq ? (TokenMatch){TK_MINUS_EQ, 2} : (TokenMatch){TK_MINUS, 1};
    case '*':
        return eq ? (TokenMatch){TK_MULTIPLY_EQ, 2} : (TokenMatch){TK_MULTIPLY, 1};
    case '/':
        return eq ? (TokenMatch){TK_DIVIDE_EQ, 2} : (TokenMatch){TK_DIVIDE, 1};
    case '=':
        return eq ? (TokenMatch){TK_EQ_EQ, 2} : (TokenMatch){TK_EQ, 1};
    case '^':
        return eq ? (TokenMatch){TK_XOR_EQ, 2} : (TokenMatch){TK_XOR, 1};
    case '%':
        return eq ? (TokenMatch){TK_MOD_EQ, 2} : (TokenMatch){TK_MOD, 1};
    case '!':
        return eq ? (TokenMatch){TK_NEQ, 2} : (TokenMatch){TK_L_NOT, 1};
    case '~':
        return (TokenMatch){TK_BW_NOT, 1};
    case '&':
        return next == '&' ? (TokenMatch){TK_AND_AND, 2} : (eq) ? (TokenMatch){TK_AND_EQ, 2} : (TokenMatch){TK_AND, 1};
    case '|':
        return next == '|' ? (TokenMatch){TK_OR_OR, 2} : (eq) ? (TokenMatch){TK_OR_EQ, 2} : (TokenMatch){TK_OR, 1};
    case '<':
        if (next == '<') {
            return t_peek_n(tk, 2) == '=' ? (TokenMatch){TK_SHL_EQ, 3} : (TokenMatch){TK_SHL, 2};
        }
        return eq ? (TokenMatch){TK_LE, 2} : (TokenMatch){TK_LT, 1};
    case '>':
        if (next == '>') {
            return t_peek_n(tk, 2) == '=' ? (TokenMatch){TK_SHR_EQ, 3} : (TokenMatch){TK_SHR, 2};
        }
        return eq ? (TokenMatch){TK_GE, 2} : (TokenMatch){TK_GT, 1};
    case '.':
        return (TokenMatch){TK_DOT, 1};
    default:
        do { log_message(LOG_ERROR, "Unknown operator"); ; exit(1);; } while (0);
    }
}
static void t_consume_operator(Tokenizer *tk) {
    const TokenMatch m = t_match_operator(tk);
    t_consume_n(tk, m.n_chars);
    t_push_buffer(tk, m.type);
}
static void t_consume_special_char(Tokenizer *tk) {
    TokenType type;
    switch (t_peek(tk)) {
    case '(':
        type = TK_OPEN_PAREN;
        break;
    case ')':
        type = TK_CLOSE_PAREN;
        break;
    case '{':
        type = TK_OPEN_CURLY;
        break;
    case '}':
        type = TK_CLOSE_CURLY;
        break;
    case '[':
        type = TK_OPEN_SQUARE;
        break;
    case ']':
        type = TK_CLOSE_SQUARE;
        break;
    case ';':
        type = TK_SEMI;
        break;
    case ',':
        type = TK_COMMA;
        break;
    case ':':
        type = TK_COLON;
        break;
    case '?':
        type = TK_TERNARY;
        break;
    default:
        do { log_message(LOG_ERROR, "Unexpected '%c'\n", t_peek(tk)); ; exit(1);; } while (0);
    }
    t_consume(tk);
    t_consume_a(tk, '\0');
    t_push_buffer(tk, type);
}
static char t_parse_escape_sequence(Tokenizer *tk, int *length) {
    t_skip(tk);
    char c = t_peek(tk);
    *length = 1;
    switch (c) {
    case 'n':
        return '\n';
    case 't':
        return '\t';
    case 'f':
        return '\f';
    case 'r':
        return '\r';
    case 'v':
        return '\v';
    case '\\':
        return '\\';
    case '"':
        return '\"';
    case '\'':
        return '\'';
    case 'x':
        t_skip(tk);
        Array hexal;
        array_init(&hexal, 4, sizeof(char));
        for (;;) {
            char x = t_peek(tk);
            if (is_hex(x)) {
                append(&hexal, &x);
                t_skip(tk);
            } else break;
        }
        *length = 0;
        int64_t res = parse_hex(hexal.data, hexal.count);
        array_free(&hexal);
        return res;
    default:
        if (is_oct(t_peek(tk))) {
            char octal[3] = {};
            int o_i = 0;
            while (o_i < 3) {
                char o = t_peek(tk);
                if (is_oct(o)) {
                    octal[o_i++] = o;
                    t_skip(tk);
                } else break;
            }
            *length = 0;
            return parse_oct(octal, o_i);
        }
        do { log_message(LOG_ERROR, "Invalid escape sequence\n"); ; exit(1);; } while (0);
    }
}
static void t_consume_char_literal(Tokenizer *tk) {
    t_skip(tk);
    for (int i = 0; i < 4; i++) {
        if (t_peek(tk) == '\'') break;
        if (t_peek(tk) == '\\') {
            int n = 0;
            t_consume_a(tk, t_parse_escape_sequence(tk, &n));
            t_skip_n(tk, n);
        } else t_consume(tk);
    }
    while (t_peek(tk) != '\'') t_skip(tk);
    t_skip(tk);
    t_push_buffer(tk, TK_CHAR_LITERAL);
}
static void t_consume_string_literal(Tokenizer *tk) {
    t_skip(tk);
    for (;;) {
        char c = t_peek(tk);
        if (c == '\n') {
            do { log_message(LOG_ERROR, "Found '\\n' in string literal."); ; exit(1);; } while (0);
        }
        if (c == '\"') {
            t_skip(tk);
            break;
        }
        if (c == '\\') {
            int n = 0;
            t_consume_a(tk, t_parse_escape_sequence(tk, &n));
            t_skip_n(tk, n);
        } else t_consume(tk);
    }
    t_push_buffer(tk, TK_STRING_LITERAL);
}
void t_tokenize(Tokenizer *tk) {
    while (!t_is_eof(tk)) {
        const char c = t_peek(tk);
        if (c == '.' && !is_num(t_peek_next(tk))) {
            t_consume(tk);
            if (t_peek(tk) == '.' && t_peek_next(tk) == '.') {
                t_consume_n(tk, 2);
                t_push_buffer(tk, TK_ELLIPSES);
            } else t_push_buffer(tk, TK_DOT);
        } else if (is_num(c) || c == '.' || c == '-' && is_num(t_peek_next(tk))) {
            int is_float = 0;
            t_consume(tk);
            if (c == '0') {
                switch (t_peek(tk)) {
                case 'x':
                    t_consume(tk);
                    while (is_hex(t_peek(tk))) {
                        t_consume(tk);
                    }
                    break;
                case 'b':
                    t_consume(tk);
                    while (is_binary(t_peek(tk))) {
                        t_consume(tk);
                    }
                    break;
                case '.':
                    is_float = 1;
                    t_consume(tk);
                    while (is_num(t_peek(tk))) {
                        t_consume(tk);
                    }
                    break;
                default:
                    while (is_oct(t_peek(tk))) {
                        t_consume(tk);
                    }
                    break;
                }
            } else {
                while (is_num(t_peek(tk))) {
                    t_consume(tk);
                }
                if (t_peek(tk) == '.') {
                    t_consume(tk);
                    is_float = 1;
                    while (is_num(t_peek(tk))) {
                        t_consume(tk);
                    }
                }
            }
            int l_count = 0;
            int u_count = 0;
            int f_count = 0;
            for (;;) {
                char c = t_peek(tk);
                if (c == 'u' || c == 'U') {
                    if (u_count >= 1) break;
                    u_count++;
                    t_consume(tk);
                } else if (c == 'l' || c == 'L') {
                    if (l_count >= 2) break;
                    l_count++;
                    t_consume(tk);
                } else if (c == 'f' || c == 'F') {
                    if (f_count >= 1) break;
                    f_count++;
                    is_float = 1;
                    t_consume(tk);
                } else break;
            }
            t_push_buffer(tk, is_float ? TK_FLT_LITERAL : TK_INT_LITERAL);
        } else if (is_alpha(c) || c == '_') {
            t_consume(tk);
            while (is_alpha_num(t_peek(tk))) {
                t_consume(tk);
            }
            t_parse_and_push_buffer(tk);
            t_buffer_reset(tk);
        } else if (is_whitespace(c)) t_skip(tk);
        else if (t_peek(tk) == '/' && (t_peek_next(tk) == '/' || t_peek_next(tk) == '*')) t_skip_comments(tk);
        else if (t_peek(tk) == '\'') t_consume_char_literal(tk);
        else if (t_peek(tk) == '\"') t_consume_string_literal(tk);
        else if (is_op_start(c)) t_consume_operator(tk);
        else t_consume_special_char(tk);
    }
}
bool is_unary_operator(const TokenType type) {
    switch (type) {
    case TK_PLUS:
    case TK_MINUS:
    case TK_INCR:
    case TK_DECR:
    case TK_L_NOT:
    case TK_BW_NOT:
    case TK_AND:
    case TK_MULTIPLY:
    case TK_SIZEOF:
        return 1;
    default:
        return 0;
    }
}
bool is_postfix_operator(const TokenType type) {
    switch (type) {
    case TK_INCR:
    case TK_DECR:
    case TK_OPEN_PAREN:
    case TK_OPEN_SQUARE:
    case TK_DOT:
    case TK_ARROW:
        return 1;
    default:
        return 0;
    }
}
bool is_binary_operator(const TokenType type) {
    switch (type) {
    case TK_PLUS:
    case TK_MINUS:
    case TK_MULTIPLY:
    case TK_DIVIDE:
    case TK_XOR:
    case TK_EQ:
    case TK_AND_AND:
    case TK_AND_EQ:
    case TK_EQ_EQ:
    case TK_GE:
    case TK_GT:
    case TK_LE:
    case TK_LT:
    case TK_DIVIDE_EQ:
    case TK_MINUS_EQ:
    case TK_MULTIPLY_EQ:
    case TK_OR_EQ:
    case TK_OR_OR:
    case TK_NEQ:
    case TK_PLUS_EQ:
    case TK_SHL_EQ:
    case TK_SHR_EQ:
    case TK_XOR_EQ:
    case TK_SHL:
    case TK_SHR:
    case TK_OR:
    case TK_AND:
    case TK_MOD:
    case TK_MOD_EQ:
        return 1;
    default:
        return 0;
    }
}
bool is_assignment_op(const TokenType type) {
    switch (type) {
    case TK_EQ:
    case TK_PLUS_EQ:
    case TK_MINUS_EQ:
    case TK_MULTIPLY_EQ:
    case TK_DIVIDE_EQ:
    case TK_MOD_EQ:
    case TK_OR_EQ:
    case TK_AND_EQ:
    case TK_XOR_EQ:
    case TK_SHL_EQ:
    case TK_SHR_EQ:
        return 1;
    default:
        return 0;
    }
}
bool is_arithmetic_op(const TokenType type) {
    switch (type) {
    case TK_PLUS:
    case TK_PLUS_EQ:
    case TK_MINUS:
    case TK_MINUS_EQ:
    case TK_MULTIPLY:
    case TK_MULTIPLY_EQ:
    case TK_DIVIDE:
    case TK_DIVIDE_EQ:
    case TK_MOD:
    case TK_MOD_EQ:
    case TK_INCR:
    case TK_DECR:
        return 1;
    default:
        return 0;
    }
}
bool is_bitwise_op(const TokenType type) {
    switch (type) {
    case TK_OR:
    case TK_AND:
    case TK_SHL:
    case TK_SHR:
    case TK_XOR:
    case TK_OR_EQ:
    case TK_AND_EQ:
    case TK_SHL_EQ:
    case TK_SHR_EQ:
    case TK_XOR_EQ:
    case TK_BW_NOT:
        return 1;
    default:
        return 0;
    }
}
bool is_comparison_op(const TokenType type) {
    switch (type) {
    case TK_EQ_EQ:
    case TK_NEQ:
    case TK_GT:
    case TK_GE:
    case TK_LT:
    case TK_LE:
        return 1;
    default:
        return 0;
    }
}
bool is_logical_op(const TokenType type) {
    switch (type) {
    case TK_AND_AND:
    case TK_EQ_EQ:
    case TK_OR_OR:
    case TK_L_NOT:
        return 1;
    default:
        return 0;
    }
}
TokenType get_underlying_op(const TokenType type) {
    switch (type) {
    case TK_PLUS_EQ:
        return TK_PLUS;
    case TK_MINUS_EQ:
        return TK_MINUS;
    case TK_MULTIPLY_EQ:
        return TK_MULTIPLY;
    case TK_DIVIDE_EQ:
        return TK_DIVIDE;
    case TK_MOD_EQ:
        return TK_MOD;
    case TK_OR_EQ:
        return TK_OR;
    case TK_AND_EQ:
        return TK_AND;
    case TK_XOR_EQ:
        return TK_XOR;
    case TK_SHL_EQ:
        return TK_SHL;
    case TK_SHR_EQ:
        return TK_SHR;
    case TK_EQ:
        return TK_EQ;
    default:
        log_start(LOG_ERROR);
        printf("Tried to get the underlying operator of a non-eq operator\n");
        print_token_type(type);
        exit(1);
    }
}
int op_associativity(const TokenType type) {
    switch (type) {
    case TK_MULTIPLY:
    case TK_DIVIDE:
    case TK_MOD:
    case TK_PLUS:
    case TK_MINUS:
    case TK_SHR:
    case TK_SHL:
    case TK_LT:
    case TK_LE:
    case TK_GT:
    case TK_GE:
    case TK_EQ_EQ:
    case TK_NEQ:
    case TK_AND:
    case TK_XOR:
    case TK_OR:
    case TK_AND_AND:
    case TK_OR_OR:
        return 1;
    case TK_SHR_EQ:
    case TK_SHL_EQ:
    case TK_AND_EQ:
    case TK_XOR_EQ:
    case TK_OR_EQ:
    case TK_EQ:
    case TK_PLUS_EQ:
    case TK_MINUS_EQ:
    case TK_MULTIPLY_EQ:
    case TK_DIVIDE_EQ:
    case TK_MOD_EQ:
        return 0;
    default:
        do { log_message(LOG_ERROR, "Tried to get the associativity of a token which is not a binary " "operator"); ; exit(1);; } while (0);
    }
}
int op_precedence(const TokenType type) {
    switch (type) {
    case TK_EQ:
    case TK_PLUS_EQ:
    case TK_MINUS_EQ:
    case TK_MULTIPLY_EQ:
    case TK_DIVIDE_EQ:
    case TK_MOD_EQ:
        return 0;
    case TK_TERNARY:
        return 1;
    case TK_SHR_EQ:
    case TK_SHL_EQ:
    case TK_AND_EQ:
    case TK_XOR_EQ:
    case TK_OR_EQ:
        return 2;
    case TK_OR_OR:
        return 3;
    case TK_AND_AND:
        return 4;
    case TK_OR:
        return 5;
    case TK_XOR:
        return 6;
    case TK_AND:
        return 7;
    case TK_EQ_EQ:
    case TK_NEQ:
        return 8;
    case TK_LT:
    case TK_LE:
    case TK_GT:
    case TK_GE:
        return 9;
    case TK_SHR:
    case TK_SHL:
        return 10;
    case TK_PLUS:
    case TK_MINUS:
        return 11;
    case TK_MULTIPLY:
    case TK_DIVIDE:
    case TK_MOD:
        return 12;
    default:
        log_start(LOG_ERROR);
        printf("Tried to get the precedence of a token which is not a binary operator");
        print_token_type(type);
        exit(1);
    }
}
const char *token_type_str(const TokenType type) {
    switch (type) {
    case TK_INT_LITERAL:
        return "Int Literal";
    case TK_FLT_LITERAL:
        return "Float Literal";
    case TK_CHAR_LITERAL:
        return "Char Literal";
    case TK_STRING_LITERAL:
        return "String Literal";
    case TK_SEMI:
        return "';'";
    case TK_PLUS:
        return "'+'";
    case TK_MINUS:
        return "'-'";
    case TK_MULTIPLY:
        return "'*'";
    case TK_DIVIDE:
        return "'/'";
    case TK_XOR:
        return "'^'";
    case TK_EXPR:
        return "Expr";
    case TK_EQ:
        return "'='";
    case TK_OPEN_PAREN:
        return "'('";
    case TK_CLOSE_PAREN:
        return "')'";
    case TK_OPEN_CURLY:
        return "'{'";
    case TK_CLOSE_CURLY:
        return "'}'";
    case TK_COMMA:
        return "','";
    case TK_IDENTIFIER:
        return "Identifier";
    case TK_MOD:
        return "'%'";
    case TK_EQ_EQ:
        return "'=='";
    case TK_PLUS_EQ:
        return "'+='";
    case TK_MINUS_EQ:
        return "'-='";
    case TK_MULTIPLY_EQ:
        return "'*='";
    case TK_DIVIDE_EQ:
        return "'/='";
    case TK_MOD_EQ:
        return "'%='";
    case TK_NEQ:
        return "'!='";
    case TK_LT:
        return "'<'";
    case TK_LE:
        return "'<='";
    case TK_GT:
        return "'>'";
    case TK_GE:
        return "'>='";
    case TK_SHL:
        return "'<<'";
    case TK_SHR:
        return "'>>'";
    case TK_SHL_EQ:
        return "'<<='";
    case TK_SHR_EQ:
        return "'>>='";
    case TK_AND:
        return "'&'";
    case TK_AND_AND:
        return "'&&'";
    case TK_AND_EQ:
        return "'&='";
    case TK_OR:
        return "'|'";
    case TK_OR_OR:
        return "'||'";
    case TK_OR_EQ:
        return "'|='";
    case TK_XOR_EQ:
        return "'^='";
    case TK_TERNARY:
        return "'?'";
    case TK_L_NOT:
        return "'!'";
    case TK_BW_NOT:
        return "'~'";
    case TK_INCR:
        return "'++'";
    case TK_DECR:
        return "'--'";
    case TK_OPEN_SQUARE:
        return "'['";
    case TK_CLOSE_SQUARE:
        return "']'";
    case TK_DOT:
        return "'.'";
    case TK_ARROW:
        return "'->'";
    case TK_COLON:
        return "':'";
    case TK_ELLIPSES:
        return "'...'";
    case TK_AUTO:
    case TK_BREAK:
    case TK_CASE:
    case TK_CHAR:
    case TK_CONST:
    case TK_CONTINUE:
    case TK_DEFAULT:
    case TK_DO:
    case TK_DOUBLE:
    case TK_ELSE:
    case TK_ENUM:
    case TK_EXTERN:
    case TK_FLOAT:
    case TK_FOR:
    case TK_GOTO:
    case TK_IF:
    case TK_INLINE:
    case TK_INT:
    case TK_LONG:
    case TK_REGISTER:
    case TK_RETURN:
    case TK_SHORT:
    case TK_SIGNED:
    case TK_SIZEOF:
    case TK_STATIC:
    case TK_STRUCT:
    case TK_SWITCH:
    case TK_TYPEDEF:
    case TK_UNION:
    case TK_UNSIGNED:
    case TK_VOID:
    case TK_VOLATILE:
    case TK_WHILE:
        return KEYWORDS[type];
    }
}
void print_token_type(const TokenType type) { printf("%s", token_type_str(type)); }
void print_token(const Token *token) {
    print_token_type(token->type);
    printf(":[%d:%d] ", token->line_n, token->char_n);
    if (token->value != ((void *)0)) {
        printf(": ");
        if (token->value[0] == '\0') {
            printf("\\0");
        } else if (token->value[0] == '\n') {
            printf("\\n");
        } else printf("%.*s", token->size, token->value);
    }
    printf("\n");
}
