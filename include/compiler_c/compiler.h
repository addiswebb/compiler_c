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
    unsigned int flags;
    Array passthrough_args;
    char *src;
    int src_size;
    Tokenizer tk;
    NodeManager nm;
    Parser p;
} Compiler;

// Compile and print Abstract Syntax Tree
#define COMP_FLAG_AST (1u << 0) // -t
// Compile and print IR Module and instructions
#define COMP_FLAG_IR (1u << 1) // -ir
// Compile .c into .s
#define COMP_STOP_AFTER_COMPILE (1u << 2) // -S
// Compile .s into .o
#define COMP_STOP_AFTER_ASSEMBLE (1u << 3) // -c

/*
    Takes input from terminal and instantiates a compiler,
    Which is ready to compile the inputted file
*/
Compiler init_compiler(int argc, char *argv[]);
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
