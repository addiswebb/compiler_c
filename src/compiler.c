#include "compiler_c/abi/abi.h"
#include "compiler_c/analyse/analysis.h"
#include "compiler_c/core/arena.h"
#include "compiler_c/core/array.h"
#include "compiler_c/core/type.h"
#include "compiler_c/ir/ir_module.h"
#include "compiler_c/ir/ir_util.h"
#include "compiler_c/log/logger.h"
#include "compiler_c/tokenize/tokenizer.h"
#include <compiler_c/analyse/sema.h>
#include <compiler_c/compiler.h>
#include <compiler_c/x86/x86.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Compiler init_compiler(const int argc, char *argv[]) {
    if (argc < 2) {
        PANIC("Improper Usage,\n  compiler [input]\n");
    }
    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        printf("compiler [input]\n");
        printf("\t-o [output] : Set output file path\n");
        printf("\t-run        : Build and Run Single File\n");
        printf("\t-a          : Compile to Assembly File\n");
        printf("\t-ir         : Compile and print IR\n");
        printf("\t-t          : Print parse tree\n");
        printf("\t-h          : Get help\n");
        exit(0);
    }

    Compiler compiler;
    compiler.flags = 0;
    const char *input_file = argv[1];
    compiler.output_file = strdup(input_file);
    compiler.output_file[strlen(input_file) - 1] = 's';

    // Loop and try find compile flags
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0) {
            if (argv[i + 1] != NULL) {
                if (argv[i + 1][0] == '-') {
                    PANIC("Improper Usage,\n  compiler_c [input] -o [output]\n");
                }
                free(compiler.output_file);
                compiler.output_file = strdup(argv[++i]);
            } else {
                PANIC("Improper Usage,\n  compiler_c [input] -o [output]\n");
            }
        } else if (strcmp(argv[i], "-t") == 0) {
            compiler.flags |= COMP_FLAG_AST;
        } else if (strcmp(argv[i], "-ir") == 0) {
            compiler.flags |= COMP_FLAG_IR;
        } else if (strcmp(argv[i], "-run") == 0) {
            compiler.flags |= COMP_FLAG_RUN;
            compiler.flags |= COMP_FLAG_ASM;
        } else if (strcmp(argv[i], "-a") == 0) {
            compiler.flags |= COMP_FLAG_ASM;
        }
    }

    load_src_file(&compiler, input_file);

    compiler.tk = t_new_tokenizer(compiler.src, compiler.src_size);
    compiler.nm = new_node_manager();
    compiler.p = new_parser();

    INFO("Compiling %s to %s ", input_file, compiler.output_file);
    if (compiler.flags != 0 && logger.min_level <= LOG_INFO) {
        printf("with flags: ");
        if (compiler.flags & COMP_FLAG_RUN) {
            printf("-run ");
        }
        if (compiler.flags & COMP_FLAG_AST) {
            printf("-t ");
        }
        if (compiler.flags & COMP_FLAG_IR) {
            printf("-ir ");
        }
        if (compiler.flags & COMP_FLAG_ASM) {
            printf("-a");
        }
    }
    if (logger.min_level <= LOG_INFO) printf("\n");

    return compiler;
}

void free_compiler(Compiler *compiler) {
    free_typepool();
    free_node(arena_get(&compiler->nm, 0));
    arena_free(&compiler->nm);
    free_parser(&compiler->p);
    t_free(&compiler->tk);

    free(compiler->output_file);
    compiler->output_file = NULL;
    free(compiler->src);
    compiler->src = NULL;
}

int compile(Compiler *compiler) {
    init_types();
    set_log_stage(STAGE_TOKENIZING);
    t_tokenize(&compiler->tk);

    set_log_stage(STAGE_PARSING);
    init_parser(&compiler->p, &compiler->tk.tokens_array, compiler->tk.tokens_array.count);
    p_parse_translation_unit(&compiler->p, &compiler->nm);

    set_log_stage(STAGE_SEMA_ANALYSIS);
    SemanticContext sema_ctx = (SemanticContext){.func = NULL, .loop = NULL, .compound = NULL};
    array_init(&sema_ctx.i_array, 4, sizeof(int));

    if (DEBUG_TYPEPOOL) print_typepool();
    semantic_analysis(&sema_ctx, &compiler->p, &compiler->nm, arena_get(&compiler->nm, 0));
    array_free(&sema_ctx.i_array);

    if (compiler->flags & COMP_FLAG_AST) print_ast(&compiler->nm);

    lower_nodes(&compiler->nm);

    if (compiler->flags & COMP_FLAG_ASM || compiler->flags & COMP_FLAG_IR) {
        set_log_stage(STAGE_IR);
        IR_Context ctx = ir_init_ctx();
        IR_Module *module = ir_gen_translation_unit(&ctx, arena_get(&compiler->nm, 0));

        if (compiler->flags & COMP_FLAG_IR) {
            printf("---- IR ----\n");
            print_ir_module(&ctx, module);
            printf("\n");
        }
        analysis(&ctx);

        if (compiler->flags & COMP_FLAG_IR) {
            if (DEBUG_LIFETIMES) {
                for (int i = 0; i < module->functions_array.count; i++) print_cfg(get_func(module, i));
            }
            if (DEBUG_LOWERED_IR) {
                printf("---- Lowered IR ----\n");
                print_ir_module(&ctx, module);
                printf("\n");
            }
        }

        if (compiler->flags & COMP_FLAG_ASM) {
            set_log_stage(STAGE_X86_GEN);
            FILE *fp = fopen(compiler->output_file, "w");
            x86_gen_module(fp, &ctx);
            fclose(fp);
        }
        ir_free_module(module);
        free_ir_ctx(&ctx);
    }

    set_log_stage(STAGE_COMPILER);

    INFO("Done.\n");
    if (compiler->flags & COMP_FLAG_RUN) {
        // char base[256];
        int base_len = strlen(compiler->output_file);
        char *base = malloc(sizeof(char) * base_len);
        ASSERT(base, "Failed to malloc base\n");
        strcpy(base, compiler->output_file);
        int len = strlen(base);
        while (base[len--] != '.' && len >= 0) {
        }

        if (len >= 0) base[len + 1] = '\0';
        // char cmd[512];
        int cmd_len = 15 + base_len * 2;
        char *cmd = malloc(cmd_len);
        ASSERT(cmd, "Failed to malloc cmd\n");
        INFO("Compiling %s.s to %s.exe (gcc)\n", base, base);
        snprintf(cmd, cmd_len, "gcc %s.s -o %s.exe", base, base);
        system(cmd);

        if (base[0] == '.') snprintf(cmd, cmd_len, "%s.exe", base);
        else snprintf(cmd, cmd_len, ".\\%s.exe", base);
        INFO("Running %s\n", cmd);
        system(cmd);
    }

    return 1;
}

static int load_src_file(Compiler *compiler, const char *file) {
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "cpp -P -std=c11 \"%s\"", file);

    FILE *fp = popen(cmd, "r");
    if (!fp) {
        PANIC("Failed to open %s\n", file);
    }
    Array src;
    array_init(&src, 1000, sizeof(char));

    char c;
    for (;;) {
        c = (char)fgetc(fp);
        if (c == EOF) break;
        append(&src, &c);
    }

    pclose(fp);
    append(&src, &(char){'\0'});

    compiler->src = (char *)src.data;
    compiler->src_size = src.count - 1;
    return 0;
}
