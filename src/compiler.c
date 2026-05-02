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

unsigned int compiler_flags = 0u;
const char *flag_strings[CF_COUNT] = {[CF_STOP_AFTER_AST] = "ast",        [CF_STOP_AFTER_IR] = "ir",
                                      [CF_STOP_AFTER_COMPILE] = "S",      [CF_STOP_AFTER_ASSEMBLE] = "c",
                                      [CF_DEBUG_TYPEPOOL] = "gtypepool",  [CF_DEBUG_LIFETIMES] = "glifetimes",
                                      [CF_DEBUG_ENUM] = "genum",          [CF_DEBUG_STRUCT] = "gstruct",
                                      [CF_DEBUG_UNION] = "gunion",        [CF_DEBUG_LOWERED_IR] = "glowered-ir",
                                      [CF_DEBUG_IR_INSTR] = "gir-instr",  [CF_DEBUG_PARSER] = "gparser",
                                      [CF_DEBUG_TOKENIZER] = "gtokenizer"};

bool has_flag(CompilerFlag f) { return compiler_flags & FLAG(f); }

bool is_source_file(const char *arg) {
    if (arg[0] == '-') return false;
    const char *dot = strrchr(arg, '.');
    if (!dot) return false;
    return strcmp(dot, ".c") == 0 || strcmp(dot, ".s") == 0 || strcmp(dot, ".o") == 0; // .o for linking stage
}

void read_args(Compiler *compiler, const int argc, char *argv[]) {
    // Loop and try find compile flags
    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];
        if (arg[0] == '-') {
            arg++;
            if (strcmp(arg, "o") == 0) {
                if (i + 1 < argc && argv[i + 1] != NULL) {
                    ASSERT(argv[i + 1][0] != '-', IMPROPER_USAGE);
                    compiler->output = argv[++i];
                    ASSERT(compiler->output, "Failed to strdup output\n");
                    continue;
                }
                PANIC(IMPROPER_USAGE);
            }
            bool consumed_flag = false;

            for (int j = 0; j < CF_COUNT; j++) {
                if (strcmp(arg, flag_strings[j]) == 0) {
                    compiler_flags |= FLAG(j);
                    consumed_flag = true;
                    break;
                }
            }
            if (consumed_flag) continue;
            append(&compiler->passthrough_args, &argv[i]);
        } else if (is_source_file(argv[i])) append(&compiler->source_files, &argv[i]);
    }
}

void assemble(Compiler *c) {
    set_log_stage(STAGE_ASSEMBLER);
    ASSERT(c->current_source.count && c->current_output.count, "Source or output is not set for assemble\n");
    INFO("Assembling %s to %s\n", (char *)c->current_source.data, (char *)c->current_output.data);
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "gcc -c %s -o %s", (char *)c->current_source.data, (char *)c->current_output.data);
    int ret = system(cmd);
    ASSERT(ret == 0, "Failed to assemble %s to %s\n", (char *)c->current_source.data, (char *)c->current_output.data);
}
void link(Compiler *c, Array *objs) {
    set_log_stage(STAGE_LINKER);
    INFO("Linking ");
    char cmd[512] = {};
    int cmd_len = snprintf(cmd, sizeof(cmd), "gcc -lm ");
    for (int i = 0; i < objs->count; i++) {
        char *src = *(char **)get(objs, i);
        printf("%s", src);
        cmd_len += snprintf(cmd + cmd_len, sizeof(cmd) - cmd_len, "%s ", src);
        if (i < objs->count - 1) putchar(' ');
    }
    printf(" to %s\n", (char *)c->current_output.data);
    snprintf(cmd + cmd_len, sizeof(cmd) - cmd_len, "-o %s ", (char *)c->current_output.data);
    int ret = system(cmd);
    ASSERT(ret == 0, "Failed to link %d objs to %s\n", objs->count, (char *)c->current_output.data);
}

void update_current_output(Compiler *c, bool cond, char *path, const char *ext) {
    if (cond && c->output) {
        array_str_cpy(&c->current_output, c->output);
    } else {
        char *out = strdup(path);
        char *dot = strrchr(out, '.');
        if (dot) strcpy(dot, ext);
        else PANIC("Failed to append '%s' to %s\n", ext, path);
        array_str_cpy(&c->current_output, out);
        free(out);
    }
}

void drive(Compiler *c) {
    if ((has_flag(CF_STOP_AFTER_COMPILE) || has_flag(CF_STOP_AFTER_ASSEMBLE)) && c->source_files.count > 1 && c->output) {
        WARN("-o ignored with multiple inputs %d\n", c->source_files.count);
        c->output = NULL;
    }
    Array objs;
    array_init(&objs, c->source_files.count, sizeof(char *));
    for (int i = 0; i < c->source_files.count; i++) {
        char *src_path = *(char **)get(&c->source_files, i);
        array_str_cpy(&c->current_source, src_path);
        update_current_output(c, has_flag(CF_STOP_AFTER_COMPILE), src_path, ".s");

        load_src_file(c, (char *)c->current_source.data);
        init_compiler(c);
        compile(c);
        clear_compiler(c);

        if (has_flag(CF_STOP_AFTER_AST) || has_flag(CF_STOP_AFTER_IR)) return;
        if (has_flag(CF_STOP_AFTER_COMPILE)) continue;

        array_str_cpy(&c->current_source, c->current_output.data);
        update_current_output(c, has_flag(CF_STOP_AFTER_ASSEMBLE), src_path, ".o");
        assemble(c);
        if (has_flag(CF_STOP_AFTER_ASSEMBLE)) continue;

        char *obj_path = strdup((char *)c->current_output.data);
        append(&objs, &obj_path);
    }
    if (has_flag(CF_STOP_AFTER_COMPILE) || has_flag(CF_STOP_AFTER_ASSEMBLE)) return;

    array_str_cpy(&c->current_output, c->output ? c->output : DEFAULT_OUT_PATH);

    ASSERT(objs.count > 0, "No object files to link\n");
    link(c, &objs);
    ptr_array_free(&objs);
    set_log_stage(STAGE_COMPILER);
    INFO("Done.\n");
}

void init_compiler(Compiler *compiler) {
    compiler->tk = t_new_tokenizer(compiler->src, compiler->src_size);
    compiler->nm = new_node_manager();
    compiler->p = new_parser();
    init_typepool();
}
Compiler begin_compiler(const int argc, char *argv[]) {
    if (argc < 2) {
        PANIC(IMPROPER_USAGE);
    }
    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        printf("compiler [input]\n");
        printf("\t-o [output] : Set output file path\n");
        printf("\t-S          : Compile to Assembly File\n");
        printf("\t-c          : Compile to Object File\n");
        printf("\t-ir         : Compile and print IR\n");
        printf("\t-ast          : Print parse tree\n");
        printf("\t-h          : Get help\n");
        exit(0);
    }

    Compiler compiler = {};

    compiler.output = NULL;
    compiler_flags = 0;
    array_init(&compiler.passthrough_args, 4, sizeof(char *));
    array_init(&compiler.source_files, 4, sizeof(char *));
    array_init(&compiler.current_output, 4, sizeof(char));
    array_init(&compiler.current_source, 4, sizeof(char));

    read_args(&compiler, argc, argv);

    return compiler;
}

void clear_compiler(Compiler *compiler) {
    free_typepool();
    free_node(arena_get(&compiler->nm, 0));
    arena_free(&compiler->nm);
    free_parser(&compiler->p);
    t_free(&compiler->tk);
    free(compiler->src);
}

void free_compiler(Compiler *compiler) {
    array_free(&compiler->passthrough_args);
    array_free(&compiler->source_files);
    array_free(&compiler->current_source);
    array_free(&compiler->current_output);
    compiler->src = NULL;
}

int compile(Compiler *compiler) {
    set_log_stage(STAGE_COMPILER);
    ASSERT(compiler->current_source.count && compiler->current_output.count, "Source or output is not set for compile\n");
    INFO("Compiling %s to %s\n", (char *)compiler->current_source.data, (char *)compiler->current_output.data);

    set_log_stage(STAGE_TOKENIZING);
    t_tokenize(&compiler->tk);

    set_log_stage(STAGE_PARSING);
    init_parser(&compiler->p, &compiler->tk.tokens_array, compiler->tk.tokens_array.count);

    p_parse_translation_unit(&compiler->p, &compiler->nm);

    set_log_stage(STAGE_SEMA_ANALYSIS);
    SemanticContext sema_ctx = (SemanticContext){
        .func = NULL,
    };
    array_init(&sema_ctx.i_array, 4, sizeof(int));
    array_init(&sema_ctx.compound_stack, 4, sizeof(Node *));
    array_init(&sema_ctx.loop_stack, 4, sizeof(Node *));

    semantic_analysis(&sema_ctx, &compiler->p, &compiler->nm, arena_get(&compiler->nm, 0));

    if (has_flag(CF_DEBUG_TYPEPOOL)) print_typepool();

    if (has_flag(CF_STOP_AFTER_AST)) {
        print_ast(&compiler->nm);
        return 1;
    }

    array_free(&sema_ctx.i_array);

    set_log_stage(STAGE_IR);
    generate_types();
    lower_nodes(&compiler->nm);

    IR_Context ctx = ir_init_ctx(&compiler->p);
    IR_Module *module = ir_gen_translation_unit(&ctx, arena_get(&compiler->nm, 0));

    if (has_flag(CF_STOP_AFTER_IR)) {
        printf("---- IR ----\n");
        print_ir_module(&ctx, module);
        printf("\n");
    }
    analysis(&ctx);

    if (has_flag(CF_STOP_AFTER_IR)) {
        if (has_flag(CF_DEBUG_LIFETIMES)) {
            for (int i = 0; i < module->functions_array.count; i++) print_cfg(get_func(module, i));
        }
        if (has_flag(CF_DEBUG_LOWERED_IR)) {
            printf("---- Lowered IR ----\n");
            print_ir_module(&ctx, module);
            printf("\n");
        }
        return 1;
    }

    set_log_stage(STAGE_X86_GEN);
    FILE *fp = fopen((char *)compiler->current_output.data, "w");
    x86_gen_module(fp, &ctx);
    fclose(fp);

    ir_free_module(module);
    free_ir_ctx(&ctx);

    return 1;
}

static int load_src_file(Compiler *compiler, const char *file) {
    char cmd[512];
    int cmd_len = snprintf(cmd, sizeof(cmd), "gcc -E -P -nostdinc -D__COMPILER_C__ -I./libc -std=c11 %s ", file);
    for (int i = 0; i < compiler->passthrough_args.count; i++) {
        cmd_len += snprintf(cmd + cmd_len, sizeof(cmd) - cmd_len, "%s ", *(char **)get(&compiler->passthrough_args, i));
    }
    printf("Running: %s\n", cmd);

    FILE *fp = popen(cmd, "r");
    ASSERT(fp, "Failed to open %s\n", file);

    Array src;
    array_init(&src, 1024, sizeof(char));

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
    if (compiler->src_size == 0) WARN("Loaded an empty source file\n");
    return 0;
}
