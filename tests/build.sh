#!/bin/bash
set -e
if [ -z "$1" ]; then
    echo "Usage: $0 <input.c> [input2.c ...] OR $0 ./src/ OR $0 ./src/*.c"
    exit 1
fi

# Expand directory to .c files recursively
INPUTS=()
for ARG in "$@"; do
    if [ -d "$ARG" ]; then
        while IFS= read -r FILE; do
            INPUTS+=("$FILE")
        done < <(find "$ARG" -name "*.c" -type f)
        if [ ${#INPUTS[@]} -eq 0 ]; then
            echo "No .c files found in directory: $ARG"
            exit 1
        fi
    else
        INPUTS+=("$ARG")
    fi
done

cmake --build build

# IGNORE=("win64" "sysv" "analysis" "const_expr" "sema" "ir_util" "ir_builder" "ir_module" "ir_gen" "parser" "tokenizer" "x86_emit" "x86_gen")
IGNORE=("win64" "ir_gen" "x86_emit" "x86_gen")

for INPUT_FILE in "${INPUTS[@]}"; do
    BASENAME=$(basename "$INPUT_FILE" .c)

    # Skip ignored files
    if [[ " ${IGNORE[*]} " == *" $BASENAME "* ]]; then
        echo "Skipping $INPUT_FILE"
        continue
    fi

    gcc -E -P -nostdinc -D__COMPILER_C__ -I./libc -std=c11 "$INPUT_FILE" -o ./tests/test.c -I./include/
    compiler_c ./tests/test.c -o ./tests/"$BASENAME".o -c || { echo "compiler_c failed on $INPUT_FILE"; exit 1; }
    mv ./tests/"$BASENAME".o ./build/CMakeFiles/compiler_c.dir/src/"$BASENAME".c.o
done

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
