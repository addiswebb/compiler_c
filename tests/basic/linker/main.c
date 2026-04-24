// RUN: %cc %s %S/input/foo.c -o %t
// RUN: %check_exit 98 %t
#include "foo.h"

int main() { return foo(12, 14.5, "Hello World\n"); }
