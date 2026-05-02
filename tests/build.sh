#!/bin/bash
set -e

if [ -z "$1" ]; then
    echo "Usage: $0 <input.c>"
    exit 1
fi

INPUT_FILE="$1"
BASENAME=$(basename "$INPUT_FILE" .c)

gcc -E -P -nostdinc -D__COMPILER_C__ -I./libc -std=c11 ./src/"$INPUT_FILE" -o ./tests/"$INPUT_FILE" -I./include/

# cmake --build build

compiler_c ./tests/"$INPUT_FILE" -o ./tests/"$INPUT_FILE".o -c || { echo "compiler_c failed"; exit 1; }

mv ./tests/compiler.c.o ./build/CMakeFiles/compiler_c.dir/src/

cd ./build/CMakeFiles/compiler_c.dir/

gcc \
    src/main.c.o \
    src/compiler.c.o \
    src/core/node.c.o \
    src/core/util.c.o \
    src/core/array.c.o \
    src/core/type.c.o \
    src/core/arena.c.o \
    src/tokenize/tokenizer.c.o \
    src/parse/parser.c.o \
    src/x86/x86_gen.c.o \
    src/x86/x86_emit.c.o \
    src/ir/ir_gen.c.o \
    src/ir/ir_util.c.o \
    src/ir/ir_builder.c.o \
    src/ir/ir_module.c.o \
    src/analyse/sema.c.o \
    src/analyse/analysis.c.o \
    src/analyse/const_expr.c.o \
    src/abi/sysv.c.o \
    src/log/logger.c.o \
    -o ../../compiler_c -lm -lc

cd ../../..
