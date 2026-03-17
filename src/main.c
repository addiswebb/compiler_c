#include "compiler_c/log/logger.h"
#include <compiler_c/compiler.h>

int main(const int argc, char *argv[]) {
    init_logger(stdout, LOG_DEBUG);
    Compiler compiler = init_compiler(argc, argv);

    compile(&compiler);
    return 0;
}
