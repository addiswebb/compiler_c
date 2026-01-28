#include "compiler_c/parse/parse_util.h"
#include <compiler_c/compiler.h>

int main(const int argc, char *argv[]) {
    // Compiler compiler = init_compiler(argc, argv);

    // compile(&compiler);
    printf("%lf\n", parse_float("123.123", 7));
    printf("%lf\n", parse_float(".123", 4));
    printf("%lf\n", parse_float("123", 3));
    return 0;
}
