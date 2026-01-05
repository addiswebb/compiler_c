#include "tokenizer.c"
#include <stdlib.h>

int main(int argc, char* argv[]){
    if (argc != 2 && argc != 4){
        printf("Improper Usage,\n  compiler [input]\n");
        return 1;
    }
    // Handle -o output filename
    char* input_filename = argv[1];
    char* output_filename = strdup(argv[1]);
    output_filename[strlen(argv[1])-1] = 'o';
    if (argc == 4){
        if (strcmp(argv[2], "-o") != 0){
            printf("Improper Usage,\n compiler [input] -o [output]");
        }else{
            output_filename = argv[3];
        }
    }
    printf("Compiling %s to %s\n", argv[1], output_filename);

    FILE *fp = fopen(argv[1], "rb");

    if (fp == NULL){
        printf("Failed to open %s", argv[1]);
        return 1;
    }

    if(fseek(fp, 0, SEEK_END) != 0){
        fclose(fp);
        printf("Failed to read file size (SEEK)");
        return 1;
    }

    long size = ftell(fp);
    if(size < 0 ){
        fclose(fp);
        printf("Failed to read file size (TELL)");
        return 1;
    }

    rewind(fp);

    char* src = malloc((size_t)size + 1);
    if(src == NULL){
        fclose(fp);
        printf("Failed to write to src buffer");
        return 1;
    }

    size_t read = fread(src, 1, (size_t)size, fp);
    fclose(fp);
    if(read != (size_t)size){
        free(src);
        printf("Inconsistent file sizes");
        return 1;
    }

    src[size] = '\0';
    t_init(src, size);

    t_tokenize();
    t_print_tokens();

    t_free();
    return 0;
}
