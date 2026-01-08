gcc main.c -o compiler.exe;./compiler.exe test.c -a;gcc test.s -o test.exe;./test.exe; echo "exit code: $?"
