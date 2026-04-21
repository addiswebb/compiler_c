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

bool is_source_file(const char *arg) {
    if (arg[0] == '-') return false;
    const char *dot = strrchr(arg, '.');
    if (!dot) return false;
    return strcmp(dot, ".c") == 0 || strcmp(dot, ".s") == 0 || strcmp(dot, ".o") == 0; // .o for linking stage
}

void read_args(Compiler *compiler, const int argc, char *argv[]) {
    // Loop and try find compile flags
    for (int i = 1; i < argc; i++) {
        char *c = argv[i];
        if (strcmp(argv[i], "-o") == 0) {
            if (argv[i + 1] != NULL && i + 1 < argc) {
                if (argv[i + 1][0] == '-') {
                    PANIC("Improper Usage,\n  compiler_c [input] -o [output]\n");
                }
                compiler->output = argv[++i];
            } else {
                PANIC("Improper Usage,\n  compiler_c [input] -o [output]\n");
            }
        } else if (strcmp(argv[i], "-t") == 0) {
            compiler->flags |= COMP_FLAG_AST;
        } else if (strcmp(argv[i], "-ir") == 0) {
            compiler->flags |= COMP_FLAG_IR;
        } else if (strcmp(argv[i], "-S") == 0) {
            compiler->flags |= COMP_STOP_AFTER_COMPILE;
        } else if (strcmp(argv[i], "-c") == 0) {
            compiler->flags |= COMP_STOP_AFTER_ASSEMBLE;
        } else if (is_source_file(argv[i])) append(&compiler->source_files, &argv[i]);
        else append(&compiler->passthrough_args, &argv[i]);
    }
}

void assemble(Compiler *c) {
    set_log_stage(STAGE_ASSEMBLER);
    ASSERT(c->current_source && c->current_output, "Source or output is not set for assemble\n");
    INFO("Assembling %s to %s\n", c->current_source, c->current_output);
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "gcc -c %s -o %s", c->current_source, c->current_output);
    int ret = system(cmd);
    ASSERT(ret == 0, "Failed to assemble %s to %s\n", c->current_source, c->current_output);
}
void link(Compiler *c, Array *objs) {
    set_log_stage(STAGE_LINKER);
    INFO("Linking ");
    char cmd[512] = {};
    int cmd_len = snprintf(cmd, sizeof(cmd), "gcc -lc -lm ");
    for (int i = 0; i < objs->count; i++) {
        char *src = *(char **)get(objs, i);
        printf("%s", src);
        cmd_len += snprintf(cmd + cmd_len, sizeof(cmd) - cmd_len, "%s ", src);
        if (i < objs->count - 1) putchar(' ');
    }
    printf(" to %s\n", c->current_output);
    snprintf(cmd + cmd_len, sizeof(cmd) - cmd_len, "-o %s ", c->current_output);
    int ret = system(cmd);
    ASSERT(ret == 0, "Failed to link %d objs to %s\n", objs->count, c->current_output);
}

void drive(Compiler *c) {
    if (c->flags & (COMP_STOP_AFTER_COMPILE | COMP_STOP_AFTER_ASSEMBLE) && c->source_files.count > 1 && c->output) {
        WARN("-o ignored with multiple inputs\n");
        c->output = NULL;
    }
    Array objs;
    array_init(&objs, c->source_files.count, sizeof(char *));
    for (int i = 0; i < c->source_files.count; i++) {
        char *src = *(char **)get(&c->source_files, i);
        c->current_source = src;
        c->current_output = c->output && c->flags & COMP_STOP_AFTER_COMPILE ? c->output : replace_extension(src, ".s");
        compile(c);
        if (c->flags & COMP_FLAG_IR) return;
        if (c->flags & COMP_STOP_AFTER_COMPILE) continue;
        c->current_source = c->current_output;
        c->current_output = c->output && c->flags & COMP_STOP_AFTER_ASSEMBLE ? c->output : replace_extension(src, ".o");
        assemble(c);
        if (c->flags & COMP_STOP_AFTER_ASSEMBLE) continue;
        append(&objs, &c->current_output);
    }
    if (c->flags & (COMP_STOP_AFTER_COMPILE | COMP_STOP_AFTER_ASSEMBLE)) return;
    c->current_source = NULL;
    if (c->output) c->current_output = c->output;

    else {
        c->current_output =
#ifdef _WIN64
            "a.exe";
#else
            "a.out";
#endif
    }
    ASSERT(objs.count > 0, "No object files to link\n");
    link(c, &objs);
    set_log_stage(STAGE_COMPILER);
    INFO("Done.\n");
}

Compiler init_compiler(const int argc, char *argv[]) {
    if (argc < 2) {
        PANIC("Improper Usage,\n  compiler [input]\n");
    }
    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        printf("compiler [input]\n");
        printf("\t-o [output] : Set output file path\n");
        printf("\t-S          : Compile to Assembly File\n");
        printf("\t-c          : Compile to Object File\n");
        printf("\t-ir         : Compile and print IR\n");
        printf("\t-t          : Print parse tree\n");
        printf("\t-h          : Get help\n");
        exit(0);
    }

    Compiler compiler;
    compiler.flags = 0;
    compiler.current_output = NULL;
    compiler.current_source = NULL;
    compiler.output = NULL;
    array_init(&compiler.passthrough_args, 4, sizeof(char *));
    array_init(&compiler.source_files, 4, sizeof(char *));

    read_args(&compiler, argc, argv);

    return compiler;
}

void free_compiler(Compiler *compiler) {
    free_typepool();
    free_node(arena_get(&compiler->nm, 0));
    arena_free(&compiler->nm);
    free_parser(&compiler->p);
    t_free(&compiler->tk);

    // free(compiler->current_output);
    compiler->current_output = NULL;
    free(compiler->src);
    compiler->src = NULL;
}

char *replace_extension(const char *path, const char *ext) {
    char *out = strdup(path);
    char *dot = strrchr(out, '.');
    if (dot) strcpy(dot, ext);
    return out;
}
void handle_output_file(Compiler *compiler, const char *ext) {
    ASSERT(compiler->current_source,
           "Tried handling output file while current_source is NULL. Make sure compiler was initialized correctly.\n");
    if (compiler->current_output == NULL) {
        compiler->current_output = replace_extension(compiler->current_source, ext);
    }
}

int compile(Compiler *compiler) {
    set_log_stage(STAGE_COMPILER);
    ASSERT(compiler->current_source && compiler->current_output, "Source or output is not set for compile\n");
    INFO("Compiling %s to %s\n", compiler->current_source, compiler->current_output);

    load_src_file(compiler, compiler->current_source);

    compiler->tk = t_new_tokenizer(compiler->src, compiler->src_size);
    compiler->nm = new_node_manager();
    compiler->p = new_parser();

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

    generate_types();
    lower_nodes(&compiler->nm);

    set_log_stage(STAGE_IR);
    IR_Context ctx = ir_init_ctx(&compiler->p);
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
        return 1;
    }

    set_log_stage(STAGE_X86_GEN);
    FILE *fp = fopen(compiler->current_output, "w");
    x86_gen_module(fp, &ctx);
    fclose(fp);

    ir_free_module(module);
    free_ir_ctx(&ctx);

    return 1;
}

static int load_src_file(Compiler *compiler, const char *file) {
    char cmd[512];
    int cmd_len = snprintf(cmd, sizeof(cmd), "gcc -E -P -nostdinc -I./libc -std=c11 %s ", file);
    for (int i = 0; i < compiler->passthrough_args.count; i++) {
        cmd_len += snprintf(cmd + cmd_len, sizeof(cmd) - cmd_len, "%s ", *(char **)get(&compiler->passthrough_args, i));
    }

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
