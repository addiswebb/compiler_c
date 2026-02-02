import sys

expected = int(sys.argv[1])
actual = int(sys.argv[2])
if actual != expected:
    print(f"FAIL: exit code {actual}, expected {expected}")
    sys.exit(1)
