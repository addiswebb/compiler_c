#include "compiler.h"

int main(const int argc, char *argv[]) {
    Compiler compiler = init_compiler(argc, argv);

    compile(&compiler);

    return 0;
}
