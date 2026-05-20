#!/bin/bash

# Track results in temp files
PASSED_FILE=$(mktemp)
FAILED_FILE=$(mktemp)

# Files to skip (just the base name without extension)
SKIP_FILES=()
#SKIP_FILES=("sysv" "analysis" "const_expr" "sema" "type" "node" "ir_builder" "ir_module" "ir_gen" "parser" "x86_emit")

# Find all .c files recursively in ./src/
find ./src/ -name "*.c" | while read -r src_file; do
    # Get basename without extension
    base_name=$(basename "$src_file" .c)

    # Check if file should be skipped
    skip=0
    for skip_file in "${SKIP_FILES[@]}"; do
        if [ "$base_name" = "$skip_file" ]; then
            skip=1
            break
        fi
    done

    if [ $skip -eq 1 ]; then
        echo "Skipping: $src_file"
        continue
    fi

    echo "=========================================="
    echo "Testing: $src_file"
    echo "=========================================="

    if ./tests/build.sh "$src_file"; then
        echo "✓ Build succeeded: $src_file"
        echo "Running lit tests..."

        if lit -s ./tests/basic/ ./tests/complex/; then
            echo "✓ Tests passed for: $src_file"
            echo "$src_file" >> "$PASSED_FILE"
        else
            echo "✗ Tests failed for: $src_file"
            echo "$src_file" >> "$FAILED_FILE"
        fi
    else
        echo "✗ Build failed: $src_file"
        echo "$src_file" >> "$FAILED_FILE"
    fi
    echo ""
done

# Print summary
echo "=========================================="
echo "SUMMARY"
echo "=========================================="
echo "Passed: $(wc -l < "$PASSED_FILE" | tr -d ' ')"
while read -r f; do
    echo "  ✓ $f"
done < "$PASSED_FILE"

echo ""
echo "Failed: $(wc -l < "$FAILED_FILE" | tr -d ' ')"
while read -r f; do
    echo "  ✗ $f"
done < "$FAILED_FILE"

# Cleanup
rm -f "$PASSED_FILE" "$FAILED_FILE"
