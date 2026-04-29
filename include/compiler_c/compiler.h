#ifndef COMPILER_C_COMPILER_H
#define COMPILER_C_COMPILER_H
#include "compiler_c/parse/parser.h"

#ifdef _WIN64
#define DEFAULT_OUT_PATH "a.exe"
#else
#define DEFAULT_OUT_PATH "a.out"
#endif

/*
    Represents the whole compilation of a single file
    And all necessary structures
*/
typedef struct {
    /* Parsed from -o */
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

#define FLAG(f) (1u << f)

typedef enum {
    // Compile and print Abstract Syntax Tree
    CF_STOP_AFTER_AST,
    // Compile and print IR Module and instructions
    CF_STOP_AFTER_IR,
    // Compile .c into .s
    CF_STOP_AFTER_COMPILE,
    // Compile .s into .o
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

#define IMPROPER_USAGE "Improper Usage,\n  compiler_c [input] -o [output]\n"

/*
    Takes input from terminal and instantiates a compiler,
    Which is ready to compile the inputted file
*/
Compiler begin_compiler(int argc, char *argv[]);
void init_compiler(Compiler *compiler);
void clear_compiler(Compiler *compiler);
void free_compiler(Compiler *compiler);

void drive(Compiler *c);

void assemble(Compiler *c);
void link(Compiler *c, Array *objs);

/*
    Takes an initialized compiler and performes compilation,
    following the set flags, on the file designated previously
*/
int compile(Compiler *compiler);

void free_compiler(Compiler *compiler);

// Loads the given file into the Compiler
static int load_src_file(Compiler *compiler, const char *file);
char *replace_extension(const char *path, const char *ext);

#endif // COMPILER_C_COMPILER_H
