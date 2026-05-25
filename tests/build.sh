#!/bin/bash
set -e

if [ -z "$1" ]; then
    echo "Usage: $0 <input.c> [input2.c ...] OR $0 ./src/ OR $0 ./src/*.c"
    exit 1
fi

cmake --build build

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

# Files to compile with GCC instead of compiler_c

# Working on "ir_builder"
GCC_COMPILE=()
# GCC_COMPILE=("sysv" "ir_builder" "ir_module" "ir_gen")

# Files to skip completely (not compiled at all)
SKIP=("win64")

GCC_COMPILED=()
COMPILER_C_COMPILED=()
SKIPPED=()

for INPUT_FILE in "${INPUTS[@]}"; do
    BASENAME=$(basename "$INPUT_FILE" .c)
    OUTPUT_PATH="./build/CMakeFiles/compiler_c.dir/${INPUT_FILE}.o"

    # Check if this file should be completely skipped
    if [[ " ${SKIP[*]} " == *" $BASENAME "* ]]; then
        SKIPPED+=("$BASENAME")
        continue
    fi

    # Check if this file should be compiled with gcc
    if [[ " ${GCC_COMPILE[*]} " == *" $BASENAME "* ]]; then
        gcc -c "$INPUT_FILE" -I./include/ -o "$OUTPUT_PATH" || { echo "GCC compilation failed on $INPUT_FILE"; exit 1; }
        GCC_COMPILED+=("$BASENAME")
    else
        gcc -E -P -nostdinc -D__COMPILER_C__ -I./libc -std=c11 "$INPUT_FILE" -o ./tests/test.c -I./include/
        compiler_c ./tests/test.c -o ./tests/"$BASENAME".o -c || { echo "compiler_c failed on $INPUT_FILE"; exit 1; }
        mv ./tests/"$BASENAME".o "$OUTPUT_PATH"
        COMPILER_C_COMPILED+=("$BASENAME")
    fi
done

echo ""
echo "======== Compilation Summary ========="
echo ""
echo "Files compiled with GCC: ${#GCC_COMPILED[@]}"
# for file in "${GCC_COMPILED[@]}"; do
#     echo "  - $file"
# done
echo ""
echo "Files skipped (platform-specific): ${#SKIPPED[@]}"
for file in "${SKIPPED[@]}"; do
    echo "  - $file"
done
echo ""
echo "Files compiled with compiler_c: ${#COMPILER_C_COMPILED[@]}"
for file in "${COMPILER_C_COMPILED[@]}"; do
    echo "  - $file"
done
echo ""
echo "======================================"
echo ""

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

echo Finished Linking
