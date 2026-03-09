#include "compiler_c/analyse/analysis.h"
#include "compiler_c/core/array.h"
#include "compiler_c/core/type.h"
#include "compiler_c/ir/ir_util.h"
#include <compiler_c/analyse/sema.h>
#include <compiler_c/compiler.h>
#include <compiler_c/x86/x86.h>

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
        printf("\t-ir         : Compile to IR\n");
        printf("\t-a          : Compile to assembly\n");
        printf("\t-t          : Print parse tree\n");
        printf("\t-h          : Get help\n");
        exit(0);
    }

    Compiler compiler;
    compiler.flags = 0;
    const char *input_file = argv[1];
    compiler.output_file = _strdup(input_file);
    compiler.output_file[strlen(input_file) - 1] = 's';

    // Loop and try find compile flags
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
        } else if (strcmp(argv[i], "-ir") == 0) {
            compiler.flags |= COMP_FLAG_IR;
        } else if (strcmp(argv[i], "-a") == 0) {
            compiler.flags |= COMP_FLAG_ASM;
        }
    }

    load_src_file(&compiler, input_file);

    compiler.tk = t_new_tokenizer(compiler.src, compiler.src_size);
    compiler.nm = new_node_manager();
    compiler.p = new_parser();

    printf("Compiling %s to %s ", input_file, compiler.output_file);
    if (compiler.flags != 0) {
        printf("with flags: ");
        if (compiler.flags & COMP_FLAG_AST) {
            printf("-t ");
        }
        if (compiler.flags & COMP_FLAG_IR) {
            printf("-ir ");
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

    init_parser(&compiler->p, &compiler->tk.tokens_array, compiler->tk.tokens_array.count);
    p_parse_translation_unit(&compiler->p, &compiler->nm);
    SemanticContext sema_ctx = (SemanticContext){.func = NULL, .loop = NULL, .compound = NULL};
    array_init(&sema_ctx.i_array, 4, sizeof(int));

    semantic_analysis(&sema_ctx, &compiler->p, &compiler->nm, &compiler->nm.nodes[0]);

    if (compiler->flags & COMP_FLAG_AST) print_ast(&compiler->nm);

    lower_nodes(&compiler->nm);

    if (compiler->flags & COMP_FLAG_ASM || compiler->flags & COMP_FLAG_IR) {
        IR_Context ctx = ir_init_ctx();
        IR_Module *module = ir_gen_translation_unit(&ctx, &compiler->nm.nodes[0]);

        if (compiler->flags & COMP_FLAG_IR) {
            print_ir_module(&ctx, module);
        }
        analysis(&ctx);

        if (compiler->flags & COMP_FLAG_IR) {
            for (int i = 0; i < module->functions_array.count; i++) {
                print_cfg(get_func(module, i));
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
    printf("Done.\n");

    return 1;
}

static int load_src_file(Compiler *compiler, const char *file) {
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "cpp -P -std=c11 \"%s\"", file);

    FILE *fp = _popen(cmd, "r");
    if (!fp) {
        printf("Failed to open %s\n", file);
        exit(1);
    }

    size_t capacity = 8192; // initial buffer
    size_t size = 0;
    char *src = malloc(capacity);
    if (!src) {
        _pclose(fp);
        printf("Failed to allocate buffer\n");
        exit(1);
    }

    int c;
    for (;;) {
        c = fgetc(fp);
        if (c == EOF) break;

        if (size + 1 >= capacity) {
            capacity *= 2;
            src = realloc(src, capacity);
            if (!src) {
                _pclose(fp);
                printf("Failed to realloc buffer\n");
                exit(1);
            }
        }
        src[size++] = (char)c;
    }

    _pclose(fp);
    src[size] = '\0';

    compiler->src = src;
    compiler->src_size = size;
    return 0;
}
