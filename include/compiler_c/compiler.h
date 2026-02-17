#ifndef COMPILER_C_COMPILER_H
#define COMPILER_C_COMPILER_H
#include "compiler_c/parse/parser.h"
#include "node.h"
#include "tokenizer.h"

/*
    Represents the whole compilation of a single file
    And all necessary structures
*/
typedef struct {
    char *output_file;
    unsigned int flags;
    char *src;
    int src_size;
    Tokenizer tk;
    NodeManager nm;
    Parser p;
} Compiler;

// Generate and print Abstract Syntax Tree
#define COMP_FLAG_AST (1u << 1)    // -t
// Generate and print IR Module and instructions
#define COMP_FLAG_IR (1u << 4)     // -ir
// Generate and save raw assembly
#define COMP_FLAG_ASM (1u << 5)    // -a


/*
    Takes input from terminal and instantiates a compiler,
    Which is ready to compile the inputted file
*/
Compiler init_compiler(int argc, char *argv[]);

/*
    Takes an initialized compiler and performes compilation,
    following the set flags, on the file designated previously
*/
int compile(Compiler *compiler);

void free_compiler(Compiler *compiler);

// Loads the given file into the Compiler
static int load_src_file(Compiler *compiler, const char *file);

#endif // COMPILER_C_COMPILER_H
