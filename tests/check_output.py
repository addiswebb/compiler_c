#!/usr/bin/env python3
import sys

if len(sys.argv) != 2:
    print(f"Usage: {sys.argv[0]} <pattern>", file=sys.stderr)
    sys.exit(1)

pattern = sys.argv[1]
data = sys.stdin.read()

if pattern in data:
    sys.exit(0)  # match
else:
    print(
        f"CHECK FAILED: '{pattern}' not found in input\nFound :{data}", file=sys.stderr
    )
    sys.exit(1)
