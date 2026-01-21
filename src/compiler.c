#include "compiler_c/ir/ir_analysis.h"
#include "compiler_c/ir/ir_util.h"
#include "compiler_c/parser.h"
#include <compiler_c/compiler.h>
#include <compiler_c/sema.h>
#include <compiler_c/tokenizer.h>
#include <compiler_c/x86.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Compiler init_compiler(const int argc, char *argv[]) {
    if (argc < 2) {
        printf("Improper Usage,\n  compiler [input]\n");
        exit(1);
    }

    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        printf("compiler [input]\n");
        printf("\t-o [output] : Set output file path\n");
        printf("\t-d          : Compile in debug mode\n");
        printf("\t-ir         : Compile to IR\n");
        printf("\t-ira        : Compile to IR and print IR analysis\n");
        printf("\t-a          : Compile to assembly\n");
        printf("\t-t          : Print parse tree\n");
        printf("\t-h          : Get help\n");
        exit(0);
    }

    Compiler compiler;
    compiler.flags = 0;
    compiler.input_file = argv[1];
    compiler.output_file = strdup(argv[1]);
    compiler.output_file[strlen(argv[1]) - 1] = 's';

    // Loop and try find compile flags: [-o, -t, -d]
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0) {
            if (argv[i + 1] != NULL) {
                if (strcmp(argv[i + 1], "-d") == 0 || strcmp(argv[i + 1], "-t") == 0) {
                    printf("Improper Usage,\n  compiler [input] -o [output]\n");
                    exit(1);
                }
                free(compiler.output_file);
                compiler.output_file = argv[++i];
            } else {
                printf("Improper Usage,\n  compiler [input] -o [output]\n");
                exit(1);
            }
        } else if (strcmp(argv[i], "-t") == 0) {
            compiler.flags |= COMP_FLAG_AST;
        } else if (strcmp(argv[i], "-d") == 0) {
            compiler.flags |= COMP_FLAG_DEBUG;
        } else if (strcmp(argv[i], "-tk") == 0) {
            compiler.flags |= COMP_FLAG_TOKENS;
        } else if (strcmp(argv[i], "-n") == 0) {
            compiler.flags |= COMP_FLAG_NODES;
        } else if (strcmp(argv[i], "-ira") == 0) {
            compiler.flags |= COMP_FLAG_IR;
            compiler.flags |= COMP_FLAG_IR_ANALYSIS;
        } else if (strcmp(argv[i], "-ir") == 0) {
            compiler.flags |= COMP_FLAG_IR;
        } else if (strcmp(argv[i], "-a") == 0) {
            compiler.flags |= COMP_FLAG_ASM;
        }
    }

    load_src_file(&compiler);

    compiler.tk = t_new_tokenizer(compiler.src, compiler.src_size);
    compiler.nm = new_node_manager();
    compiler.p = new_parser();

    printf("Compiling %s to %s ", compiler.input_file, compiler.output_file);
    if (compiler.flags != 0) {
        printf("with flags: ");
        if (compiler.flags & COMP_FLAG_DEBUG) {
            printf("-d ");
        }
        if (compiler.flags & COMP_FLAG_AST) {
            printf("-t ");
        }
        if (compiler.flags & COMP_FLAG_TOKENS) {
            printf("-tk ");
        }
        if (compiler.flags & COMP_FLAG_NODES) {
            printf("-n ");
        }
        if (compiler.flags & COMP_FLAG_IR) {
            printf("-ir ");
        }
        if (compiler.flags & COMP_FLAG_IR_ANALYSIS) {
            printf("-ira ");
        }
        if (compiler.flags & COMP_FLAG_ASM) {
            printf("-a ");
        }
    }
    printf("\n");

    return compiler;
}

void free_compiler(Compiler *compiler) {
    t_free(&compiler->tk);
    free_node_manager(&compiler->nm);
    free(compiler->output_file);
    free(compiler->src);
    compiler->src = NULL;
    compiler->output_file = NULL;
    compiler->src = NULL;
}

int compile(Compiler *compiler) {
    init_types();
    t_tokenize(&compiler->tk);

    if (compiler->flags & COMP_FLAG_TOKENS) {
        t_print_tokens(&compiler->tk);
    }
    init_parser(&compiler->p, &compiler->tk.tokens, compiler->tk.tokens.size);
    p_parse_translation_unit(&compiler->p, &compiler->nm);

    semantic_analysis(&compiler->p, &compiler->nm, &compiler->nm.nodes[0]);

    if (compiler->flags & COMP_FLAG_AST) print_ast(&compiler->nm);

    if (compiler->flags & COMP_FLAG_ASM || compiler->flags & COMP_FLAG_IR) {
        IR_Context ctx = {NULL, NULL, NULL};
        IR_Module *module = ir_gen_translation_unit(&ctx, &compiler->nm.nodes[0]);

        if (compiler->flags & COMP_FLAG_IR) {
            print_ir_module(&ctx, module);
        }
        ir_analysis(&ctx);

        if (compiler->flags & COMP_FLAG_IR) {
            for (int i = 0; i < module->func_count; i++) {
                print_cfg(module->functions[i]);
            }
            print_ir_module(&ctx, module);
        }

        if (compiler->flags & COMP_FLAG_ASM) {
            FILE *fp = fopen(compiler->output_file, "w");
            x86_gen_module(fp, &ctx);
            fclose(fp);
        }
        ir_free_module(module);
    }
    printf("Finished.\n");

    return 1;
}

static int load_src_file(Compiler *compiler) {
    FILE *fp = fopen(compiler->input_file, "rb");

    if (fp == NULL) {
        printf("Failed to open %s", compiler->input_file);
        exit(1);
    }

    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        printf("Failed to read file size (SEEK)");
        exit(1);
    }

    const long size = ftell(fp);
    if (size < 0) {
        fclose(fp);
        printf("Failed to read file size (TELL)");
        exit(1);
    }

    rewind(fp);
    char *src = malloc((size_t)size + 1);
    if (src == NULL) {
        fclose(fp);
        printf("Failed to write to src buffer");
        exit(1);
    }

    const size_t read = fread(src, 1, (size_t)size, fp);
    fclose(fp);
    if (read != (size_t)size) {
        free(src);
        printf("Inconsistent file sizes");
        exit(1);
    }

    src[size] = '\0';

    compiler->src = src;
    compiler->src_size = size;

    return 0;
}
