#include "compiler.c"

int main(int argc, char* argv[]){
    Compiler compiler = init_compiler(argc, argv);

    compile(&compiler);

    return 0;
}
