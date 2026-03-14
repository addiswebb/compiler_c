#!/usr/bin/env python3
import subprocess
import sys

if len(sys.argv) != 3:
    print("Usage: check_exit.py <expected_exit_code> <program>")
    sys.exit(2)


def normalize(code):
    # Convert POSIX 0–255 exit codes back to signed range
    return code % 256


expected = int(sys.argv[1])
program = sys.argv[2]

result = subprocess.run([program])

actual = normalize(result.returncode)
expected = normalize(result.returncode)

if actual != expected:
    print("Exit code mismatch:")
    print(f"  Expected: {expected}")
    print(f"  Actual:   {actual}")
    sys.exit(1)

sys.exit(0)
