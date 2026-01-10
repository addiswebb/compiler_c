#ifndef COMPILER_C_COMPILER_H
#define COMPILER_C_COMPILER_H
#include "node.h"
#include "parser.h"
#include "tokenizer.h"

typedef struct {
    char *input_file;
    char *output_file;
    unsigned int flags;
    char *src;
    int src_size;
    Tokenizer tk;
    NodeManager nm;
    Parser p;
} Compiler;

#define COMP_FLAG_DEBUG (1u << 0)  // -d
#define COMP_FLAG_AST (1u << 1)    // -t
#define COMP_FLAG_TOKENS (1u << 2) // -tk
#define COMP_FLAG_NODES (1u << 3)  // -n
#define COMP_FLAG_IR (1u << 4)     // -ir
#define COMP_FLAG_ASM (1u << 5)    // -a

int compile(Compiler *compiler);
Compiler init_compiler(int argc, char *argv[]);
void free_compiler(Compiler *compiler);

#endif // COMPILER_C_COMPILER_H
