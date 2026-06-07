#ifndef COMPILER_C_COMPILER_H
#define COMPILER_C_COMPILER_H
#include "compiler_c/parse/parser.h"

#ifdef _WIN64
#define DEFAULT_OUT_PATH "a.exe"
#else
#define DEFAULT_OUT_PATH "a.out"
#endif

#define IMPROPER_USAGE "Improper Usage,\n  compiler_c [input] -o [output]\n"

/*
    The Compiler
*/
typedef struct {
    // Parsed from -o
    char *output;
    // String of the current source path
    Array current_source;
    // String of the current output path
    Array current_output;
    // Array of source files given at startup.
    Array source_files;
    // Array of non-compiler_c args passed, which will be given to preprocessor.
    Array passthrough_args;
    // Preprocessed source file
    char *src;
    int src_size;
    NodeManager nm;
    Parser p;
    Tokenizer tk;
} Compiler;

#define FLAG(f) (1u << f)
/* Flags that determine functionality of the compiler, parsed at startup.  */
typedef enum {
    // Compile and print Abstract Syntax Tree
    CF_STOP_AFTER_AST,
    // Compile and print IR Module and instructions
    CF_STOP_AFTER_IR,
    // Compile `.c` into `.s`
    CF_STOP_AFTER_COMPILE,
    // Compile `.s` into `.o`
    CF_STOP_AFTER_ASSEMBLE,
    // Print all types in the typepool
    CF_DEBUG_TYPEPOOL,
    // Print computed lifetimes of every virtual register
    CF_DEBUG_LIFETIMES,
    // Print enums with extra detail
    CF_DEBUG_ENUM,
    // Print structs with extra detail
    CF_DEBUG_STRUCT,
    // Print unions with extra detail
    CF_DEBUG_UNION,
    // Print IR after analysis and lowering
    CF_DEBUG_LOWERED_IR,
    // Print IR instructions as they are emitted
    CF_DEBUG_IR_INSTR,
    // Print Tokens when consumed by Parser
    CF_DEBUG_PARSER,
    // Print Characters when consumed by Tokenizer
    CF_DEBUG_TOKENIZER,
    // Print all tokens produced by Tokenizer
    CF_DEBUG_TOKENS,
    // Print all symbols once generated
    CF_DEBUG_SYMBOLS,
    // Number of compiler flags
    CF_COUNT,
} CompilerFlag;

/* Global compiler flags. */
extern unsigned int compiler_flags;
/* String representation of compiler_c flags. */
extern const char *flag_strings[CF_COUNT];

/* Returns true if global compiler_flags contains the given flag */
bool has_flag(CompilerFlag f);


/* Takes input from terminal and instantiates a compiler with compiler flags parsed. */
Compiler begin_compiler(int argc, char *argv[]);
/* Initialize compiler's parser, tokenizer and node manager.*/
void init_compiler(Compiler *compiler);
/* Post compile cleanup so the compiler is ready to compile another source file. */
void clear_compiler(Compiler *compiler);
void free_compiler(Compiler *compiler);

/*
    Handles whole compiler interaction.
    Correctly compiles, assembles and links input source files based of parsed compiler flags.
*/
void drive(Compiler *c);

/* Calls gcc assembler to compile `.s` to `.o` */
void assemble(Compiler *c);
/*
    Calls gcc linker on array of obj paths collected by assembler
    Output file is `compiler->current_output`
 */
void link(const Compiler *c, const Array *objs);

/*
    Takes an initialized compiler and performs compilation,
    following the set flags, on the file designated previously
*/
int compile(Compiler *compiler);

/* Loads the given source file into the Compiler with gcc preprocessor */
static int load_src_file(Compiler *compiler, const char *file);

#endif // COMPILER_C_COMPILER_H
