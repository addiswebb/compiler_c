#include "parser.c"
#include <stdlib.h>

typedef struct{
    char* input_file;
    char* output_file;
    unsigned int flags;
    char* src;
    int src_size;
}Compiler;

#define COMP_FLAG_DEBUG (1u << 0) // -d
#define COMP_FLAG_TREE (1u << 1) // -t
#define COMP_FLAG_PRINT_TOKENS (1u << 2) // -tk
#define COMP_FLAG_PRINT_NODES (1u << 3) // -n

static int load_src(Compiler* compiler){
    FILE *fp = fopen(compiler->input_file, "rb");

    if (fp == NULL){
        printf("Failed to open %s", compiler->input_file);
        exit(1);
    }

    if(fseek(fp, 0, SEEK_END) != 0){
        fclose(fp);
        printf("Failed to read file size (SEEK)");
        exit(1);
    }

    long size = ftell(fp);
    if(size < 0 ){
        fclose(fp);
        printf("Failed to read file size (TELL)");
        exit(1);
    }

    rewind(fp);

    char* src = malloc((size_t)size + 1);
    if(src == NULL){
        fclose(fp);
        printf("Failed to write to src buffer");
        exit(1);
    }

    size_t read = fread(src, 1, (size_t)size, fp);
    fclose(fp);
    if(read != (size_t)size){
        free(src);
        printf("Inconsistent file sizes");
        exit(1);
    }

    src[size] = '\0';

    compiler->src = src;
    compiler->src_size = size;

    return 0;
}

int compile(Compiler* compiler){
    t_init(compiler->src, compiler->src_size);
    t_tokenize();
    if(compiler->flags & COMP_FLAG_PRINT_TOKENS){
        t_print_tokens();
    }
    p_init();
    p_parse_translation_unit();
    if(compiler->flags & COMP_FLAG_PRINT_NODES){
        print_nodes();
    }

    if(compiler->flags & COMP_FLAG_TREE){
        print_node(&node_manager.nodes[0],0);
    }
    return 1;
}

Compiler init_compiler(int argc, char* argv[]){
    if (argc < 2){
        printf("Improper Usage,\n  compiler [input]\n");
        exit(1);
    }

    // compiler -h
    if (argc == 2 && strcmp(argv[1], "-h")==0){
        printf("compiler [input]\n");
        printf("\t-o [output] : Set output file path\n");
        printf("\t-d          : Compile in debug mode\n");
        printf("\t-t          : Print parse tree\n");
        printf("\t-h          : Get help\n");
        exit(0);
    }

    Compiler compiler;
    compiler.flags = 0;
    compiler.input_file = argv[1];
    compiler.output_file = strdup(argv[1]);
    compiler.output_file[strlen(argv[1])-1] = 'o';

    // Loop and try find -o, -t, -d
    for (int i = 1; i < argc; i++){
        if(strcmp(argv[i], "-o") == 0){
            if(argv[i+1] != NULL){
                if (strcmp(argv[i+1], "-d")==0 || strcmp(argv[i+1], "-t")==0){
                    printf("Improper Usage,\n  compiler [input] -o [output]\n");
                    exit(1);
                }
                free(compiler.output_file);
                compiler.output_file = argv[++i];
            }else{
                printf("Improper Usage,\n  compiler [input] -o [output]\n");
                exit(1);
            }
        }else  if(strcmp(argv[i], "-t") == 0){
            compiler.flags |= COMP_FLAG_TREE;
        }else  if(strcmp(argv[i], "-d") == 0){
            compiler.flags |= COMP_FLAG_DEBUG;
        }else  if(strcmp(argv[i], "-tk") == 0){
            compiler.flags |= COMP_FLAG_PRINT_TOKENS;
        }else  if(strcmp(argv[i], "-n") == 0){
            compiler.flags |= COMP_FLAG_PRINT_NODES;
        }
    }

    load_src(&compiler);

    printf("Compiling %s to %s ",compiler.input_file, compiler.output_file);
    if(compiler.flags != 0){
        printf("with flags: ");
        if(compiler.flags & COMP_FLAG_DEBUG){
            printf("-d ");
        }
        if(compiler.flags & COMP_FLAG_TREE){
            printf("-t ");
        }
        if(compiler.flags & COMP_FLAG_PRINT_TOKENS){
            printf("-tk ");
        }
        if(compiler.flags & COMP_FLAG_PRINT_NODES){
            printf("-n ");
        }
    }
    printf("\n");


    return compiler;
}

void free_compiler(Compiler* compiler){
    free(compiler->output_file);
    t_free();
    p_free();
}
