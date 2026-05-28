#include "compiler_c/log/logger.h"
#include <compiler_c/compiler.h>

int main(const int argc, char *argv[]) {
    init_logger(stdout, LOG_INFO);
    Compiler compiler = begin_compiler(argc, argv);
    drive(&compiler);

    free_compiler(&compiler);
    return 0;
}
