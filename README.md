# Compiler C
An unoptimised C compiler written in C supporting C89 following Win64 MS ABI Conventions.
With the goal of eventual self compilation.

## Table of Contents
* [Compiler C](#compiler-c)
* [Grammar](#grammar)
* [Architecture](#architecture)
  * [The Tokenizer](#the-tokenizer)
  * [The Parser](#the-parser)
  * [Semantic Analysis Pass](#semantic-analysis-pass)
  * [IR Gen](#ir-gen)
  * [Analysis Pass](#analysis-pass)
  * [x86 Gen](#x86-gen)
  * [ABI](#abi)
* [Compiler Features Implemented](#compiler-features-implemented)
  * [1. Types](#1-types)
  * [2. Literals](#2-literals)
  * [3. Variables & Storage](#3-variables--storage)
  * [4. Expressions](#4-expressions)
  * [5. Control Flow](#5-control-flow)
  * [6. Functions](#6-functions)
  * [7. Pointers](#7-pointers)
  * [8. Completeness](#8-completeness)
* [To be Implemented](#to-be-implemented-ordered-from-next-to-never)

# Grammar
The grammar supported by the compiler is as defined in the [GNU C Language Manual](gnu-c-language-manual.pdf). Some liberties have been taken in places where I felt supporting such grammar would be redundant in the short term. Below is an example of equivalent code where only the former is supported.
```c
char a[] = "Hello World";
```

```c
char b[] = "Hello " "World";
```
Both of these are parsed to be identical strings `"Hello World"`, as option `a` is perfectly fine, I do not plan to support option `b` until absolutely necessary. While the goal is to fully support C89, some useful features from future standards are also supported. For example designated initliazers for `struct` and `array` types,
```c
int x[] = { [1] =5, 4, 5, [5]= 6 };
```

```c
struct Point{ int x; int y;  };
struct Point p = {.x = 10, .y=10};
```
Or compound literals
```c
void foo(int *a);
foo(&(int){1});
```

GNU also features several compiler extensions, from which I have chosen the useful or interesting ones to implement aswell,
* Binary Constants `0b101 = 5`

Additionally digraphs and trigraphs will not be supported. Things like implicit `int` return types are also ignored.

A good way to understand what is supported is as follows,
> "Every language feature used in developing the compiler will be implemented, such that the compiler can compiler itself." 

# Architecture
The compiler is comprised of 6 major sections. Each one has a clear task. 
The compiler manages all sections and hands the work of one onto the next. It also handles loading the file from disc into memory and parsing compile flags given at runtime which change the compiler`s behaviour.

### The Tokenizer. 
Its job is to take in the loaded file (called a translation unit), as a string of characters and convert it into an array of tokens. These tokens represent core parts of the grammar defined below, e.g. **TK_INT_LITERAL** is simply a raw number "1234", or **TK_RETURN** represents the the `return` keyword. It performs this by loading tokens sequentially into a buffer until it finds a whitespace character. It then parses the buffer into a token. Essentially looking at each word and deciding what kind of token it is. There is more hidden complexity to this in skipping comments, or handling special characters but this is a good overview. Once it reaches the EOF (end of the file) it stops and returns the Token Array back to the compiler.

### The Parser
The parser recieves the linear array of tokens and is tasked with converting it into a AST (abstract syntax tree). The tree is structured as follows:

* **N_TRANSLATION_UNIT**: Array of declarations of either a function or variable
* **N_FUNCTION**: Name, Params, and an **N_COMPOUND** block.
* **N_COMPOUND**: Array of statements.

With more fundamental nodes like **N_BINARY** and **N_UNARY** representing the most basic C constructs.

The parser, starting from the very first token, uses context and grammar to decide what node is next. It also naturally handles syntax errors. E.g., if the current token is a **TK_RETURN**, given the grammar below, we can expect an `expr` to follow. The parser then tries to parse an `expr` and if it succeeds, we combine the **TK_RETURN** token and parsed `expr` into a single Node, **N_RETURN**. This node is then appended to the **N_COMPOUND** parent node. Another example, if the current token is **TK_OPEN_CURLY**, given the grammar, this can only be the start of a **N_COMPOUND**. The parser knows to parse the following tokens as an array of statements and append them to the compound nodes list, and only stops when **TK_CLOSE_CURLY** is found. Same as the **Tokenizer**, when we reach the end of the token array, we return the parsed **AST** to the compiler.
Whilst parsing, a typedef token might be parsed, this happens the typedef entry is added to the symbol pool. This is later used by the parser to parse types.

### Semantic Analysis Pass
The **AST** constructed by the parser is mostly typeless. Only explictly typed expressions have type information. It is the job of the semantic analysis pass to give every node a correct type. While doing so, it also ensures correctness through type checking. E.g, in the majority of cases a binary operation requires two operands of the same type. If the operands are found to have different types, we try to promote them to the same common type. If this is not possible and their types are completely incompatible, a type error is thrown. Semantic Analysis also handles converting constructs which are simply syntactic sugar into their literal underlying forms. E.g `a[5]` is just `*(a + sizeof(element) * 5)`. Similarily `a->b` is just `*(a).b`. These conversions are handled in this pass.

During semantic analysis, the symbol table is added to for all declarations. Symbols should be unique so for every declaration we check if its identifier is already within the symbol table. If it is, we ensure it is already defined, in the case where it is, the current declaration cannot also contain a definiton. This case produces a redefinition error. If the symbol is not defined and the declaration is, we update the symbol with the definition.

### IR Gen
This step involves taking the **AST** and converting it from the recursive structured tree (where nodes link to nodes linking to other nodes), into a more linear and assembly like "Intermediate Representation", **IR**. The **IR** generator, traverses the tree according to our definition, and when it reaches a statement or expression, it converts it to corresponding **IR**  instructions. 

IR takes two forms, an intitial generic representation and a final ABI specific representation. The conversion happens in the **Analysis** pass, the goal is to have a more human readable generic version and a more literal platform specific, later version which closer reflects the final assembly code.

E.g., We reach a **N_RETURN** node. It has an `expr`, which we need to generate **IR** for before we can call **IR_RET**. First, we call `ir_gen_expression`, which creates the **IR** for the return expression and stores the result in some virtual register. `ir_gen_expression` then returns the register, which we can give to our **IR_RET** instruction. Now the **IR_RET** has only the register where its return value is. This makes converting to assembly very easy later on. Its similar to assembly but it only represents what the code will do, where assembly represents exactly what the computer will do (which as a human, is not always as clear). 

The **IR_Module** makes use of pools to store symbols and their definitons. There is a const pool which stores all literals and can be referenced to using an index. The globals pool acts similarly, storing a literal but also a identifier. Finally the var pool which tracks defined locals. It exists its self as a stack of pools, one for each scope entered.

This simplicity is vital for easily translating **IR** into actual assembly, which takes place in the final step.

### Analysis Pass
When **IR** is generated, the instructions mostly use Virtual Registers. A virtual register is defined once and its value is read from many times. A virtual register does not get redefined nor does it actually exist in memory. The job of the Analysis pass is to replace these virtual registers with physical stack slots and registers. To achieve this, first a Control Flow Graph is generated from the **IR Blocks**. Each block is read to determine where it can branch to. The result is that every block has a list of successor blocks and predecessor blocks. The next step is to define what virtual registers a block defines/creates and what registers it uses but did not create. This is done by looping over every instruction within the block, it is known for each instruction which operand is a definition and which operands are just used. With this information we populate the blocks `used` and `defined` arrays.

The next step is to compute for each block, the virtual registers which come from outside the block and which exist after the block ends. These are called the `live_in` and `live_out` arrays. The details of this are complex and will be skipped for brevity. At this point it can be concluded that for every block, where a virtual register is in its `live_in` but not the `live_out`, the register dies within the block. For such instances we search for the last occurence of it and set that as the registers end instruction. The start instruction of every virtual register is simply found by looping over all instructions and recording when it is defined.

With the liveness/lifetimes of every virtual register computed, we can now allocate them to reusable stack slots. This process is simple, given the virtual registers sorted from first defined to last defined, we check if their is a suitable stack slot availiable, if so we assign the register to it. Otherwise we create a new stack slot with the correct size and set its `next_free` variable to the registers `last_used`. This results in stack slots being reused once they are free.

These slots can then be allocated on the stack as physical slots. Later, the first n compatible stack slots can be assigned to registers instead for optimisations. 

### x86 Gen
Here we lower the **IR** to raw assembly, replacing simplified **IR_BINARYOP**, or other instruction with hardware specific instructions. Loading memory to and from registers, handling bit flags etc. While the assembly can be tricky to come to terms with, converting the generated **IR** to x86-64 assembly is actually the easiest step. This is thanks to the work done by the Tokenizer, Parser and **IR** gen.

E.g., given an **IR_RET** instruction, we first take the given virtual register (Which promises to store a return value) and load it into the `%rax` CPU register. This register is used for return values. Then we reset the `%rsp` stack pointer back to the start of the function's stack frame, which was stored in `%rbp`. Then we pop the return address off the stack (Which is at the top of the stack, when we are at the start of the functions stack frame) and finally call `ret`. Which jumps back to the return address in `%rbp` and "brings" our return value along in `%rax`.

You can see how such a simple **IR** instruction can expand into a much more complex set of assembly instructions. This highlights the purpose of the intermediate representation, to hide away this overwhelming complexity so we can later focus on optimizaton and stuff.
### ABI
As the compiler supports both Win64 and SysV platforms it is neccary to have differing implementations which correspond to the correct Application Binary Interface. This is done by selectively compiling either `win64.c` or `sysv.c`, which both implement the functions found in `abi.h` and are ABI conformant. 

Any function whose implementation changes by platform is prefixed with `abi_`.  Much of the differences in ABI standards are found in IR or x86 lowering. My goal was to have a high level version of a translation unit's IR, which is the same regardless of platform. This then gets "lowered" to be ABI specific. Things like `call` or `ret` instructions. This mimics how `IR_Value`s go from virtual concepts to physical registers and defined stack offsets. Both take place together after IR has been fully generated.

As both platforms use registers for different things and in different orders, the corresponding static arrays which define this are intialized in their respective implementation source file, so that anywhere else `int_param_reg[0]` gives the correct register.

### Architecure Components

Below is a list of features implemented to enable easier implentation and maintainance of other comiler features, while preventing code duplication and increasing memory safety.

#### Array
A generic dynamic Array struct which is used throught the code base where variable sized lists of objects must be stored.
Works by reallocating memory with `capacity * 2` size when full.

#### Arena
A large scale allocator used in place of an Array for instances where the memory cannot be moved/reallocated, but must also support dynamic sizing. Works by allocating blocks of a set size one by one as needed. Appending elements into each until it is full before appending another block. Insertions are not allowed only append and set operations.
When a block is full, instead of reallocating the whole block somewhere else at a greater size, another block is created elsewhere so any references to elements in the arena are valid until the arena is freed. This is the major difference compared to an Array.

# Compiler Features Implemented

## 1. Types

* Primitive Types
  * `int`, `float`, `char`, `double`, `short`, `long`
  * Unsigned integer variants: `unsigned int`, `signed int`
* Type Qualifiers
  * `const` (Parsed but ignored)
  * `volatile` (Parsed but ignored)
* Type Conversion
  * Casts: `(int)x`, `(float*)y`
* Compound Types
  * Arrays
    * Declaration: `type A[n]`
    * C strings `"Hello World\n"`
      * Auto null terminating `\0`
    * Indexing with `[i]`
    * Array initialization: `= {}`
    * **Size inference** `int[]` (C99)
    * **Designated initializers** `[i] = value` (C99)
  * Structs/Unions
    * Declaration: `struct A { ... }`
    * Nested structs/unions
    * Member access: `.` and `->`
    * Padding rules
    * **Designated initializers** `.member = value` (C99)
  * Enums
    * Explicit values: `enum C { ONE = 1, TWO, }`
* Typedef
  * Aliases: `typedef type name`
* Function Pointers

## 2. Literals

* Integer Literals
  * Decimal, Octal, Hexadecimal, Binary (`0bNN`) (C99) (C11/GNU Extension for Binary)
  * Unsigned variants
  * Literal Suffixes `u`, `U`, `l`, `L`
* Floating-Point Literals
  * Decimal floating literals
  * Literal Suffixes `f`, `F`, `l`, `L`
* Character Literals
  * Single character `'a'`
  * Multi-character literals `'abcd'` (implementation-defined)
  * Escape sequences: `\n`, `\t`, etc.
* String Literals
  * `"..."`  
* Compound Literals
  * `(Type){ ... }` (C99)

## 3. Variables & Storage

* Declaration & Scoping
  * Local and global variables
  * Shadowing
* Storage Specifiers
  * `extern`
  * `static`

## 4. Expressions

* Unary Operators
  * Arithmetic: `+`, `-`, `!`, `++`, `--`
  * Bitwise: `~`
  * `sizeof`
* Binary Operators
  * Arithmetic: `+`, `-`, `*`, `/`
  * Bitwise: `&`, `|`, `^`, `<<`, `>>`
  * Comparison: `<`, `>`, `<=`, `>=`, `==`, `!=`
  * Logical: `&&`, `||`
  * Assignment: `=`, `+=`, `-=`, `*=`, `/=`, `&=`, `|=`, `^=`, `<<=`, `>>=`
* Pointer & Address Operators
  * `&`, `*`
  * Pointer arithmetic

## 5. Control Flow

* Conditional
  * `if` / `else`
  * Nested conditionals
* Loops
  * `for`, `while`
  * `do {} while()`
* Switch
  * `switch`, `case`
* Jump Statements
  * `break`, `continue`
  * `goto` & labels
* Nested loops/branches
* Early jumping for `a && b` and `a || b` conditions.

## 6. Functions

* Declarations & Calls
  * Parameters, return types
    * `void` return type
  * `inline` (Parsed but ignored)
* Function ABI & Calling Conventions `MS x64 ABI`
  * `Win64` only Shadow Space `32 bytes`
  * Function Arguments
    * `Win64`
      * First `4` args to registers, then stack spilled
    * `SysV`
      * First `6` integer args to registers, then stack spilled
      * First `8` floating point args to registers, then stack spilled
  * Structs as function arguments and **return values**
    * `Win64`
      * `sizeof(struct A) <= 8b` => 1-2 integer chunks
      * `         ...          ` => hidden pointer & memcpy
    * `SysV`
      * `sizeof(struct A) <= 16b` => 1-2 integer/SSE chunks
      * `       ...        > 16b` => hidden pointer & memcpy
  * Variadic Functions
    * `Win64`
      * Floating point arguments cloned to general purpose registers (For 4 args)
    * `SysV`
      * Number of SSE registers used stored in `%al`
    
## 7. Pointers

* Basic pointers & addresses
* Pointer arithmetic
* Dereference and member access `a->b` as `*(a).b`

## 8. Completeness
* Automatically call `gcc` preprocessor on source file.
* Win64 ABI
* SysV ABI

## To be Implemented (Ordered from next to never...)
* Return structs according to ABI.
* Write `stdlib` subset for self-compilation.
* Compiler builtins
    * `__builtin_va_list` etc.
* Support a standard library 
    * ~~Support [musl-libc](https://github.com/runtimejs/musl-libc/) (~~Support a standard library~~). (~~Create a standard library~~)~~
* Use physical registers
    * Overflow to stack
* Function ABI Calling Conventions
    * Caller/Callee save registers
* Bitfield in structs
    * `unsigned int flag : 1; // 1 bit`
* Inline functions
* Volatile/Atomic memory
* Labels as values? (part of GCC, not standard C)
