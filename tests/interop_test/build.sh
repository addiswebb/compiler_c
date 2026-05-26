#!/usr/bin/env bash

set -e

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <file1> <file2>"
    exit 1
fi

FILE1="$1"
FILE2="$2"

if [ ! -f "$FILE1" ]; then
    echo "File not found: $FILE1"
    exit 1
fi

if [ ! -f "$FILE2" ]; then
    echo "File not found: $FILE2"
    exit 1
fi

BASE1=$(basename "$FILE1")
BASE1="${BASE1%.*}"

BASE2=$(basename "$FILE2")
BASE2="${BASE2%.*}"

echo "[1/5] Compiling $FILE1 [compiler_c] → $BASE1.s"
compiler_c "$FILE1" -o $BASE1.o -c -I../../libc


echo "[3/5] Compiling $FILE2 [gcc] → $BASE2.o"
gcc -S "$FILE2" -o "$BASE2.s" -fno-unwind-tables -fno-asynchronous-unwind-tables
gcc -c "$BASE2.s" -o "$BASE2.o"

echo "[4/5] Linking $BASE1.o [compiler_c] + $BASE2.o [gcc] → a.out"
gcc "$BASE1.o" "$BASE2.o" -o "a.out"

echo "[5/5] Running a.out"

set +e
./a.out
rc=$?
set -e

echo "Exit Code: $rc"

echo "Done."
