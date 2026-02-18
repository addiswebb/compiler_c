# Compiler C
An unoptimised C compiler written in C supporting C89 following Win64 MS ABI Conventions.
With the goal of eventual self compilation.

# Architecture
The compiler is comprised of 6 major sections. Each one has a clear task. The compiler manages all 4 sections, and hands the work of one onto the next. It also handles loading the file from disc into memory and parsing compile flags given at runtime.

### The Tokenizer. 
Its job is to take in the loaded file (called a translation unit), as a string of characters and convert it into an array of tokens. These tokens represent core parts of the grammar defined below, e.g. **TK_INT_LITERAL** is simply a raw number "1234", or **TK_RETURN** represents the the 'return' keyword. It performs this by loading tokens sequentially into a buffer until it finds a whitespace character. It then parses the buffer into a token. Essentially looking at each word and deciding what kind of token it is. There is more hidden complexity to this in skipping comments, or handling special characters but this is a good overview. Once it reaches the EOF (end of the file) it stops and returns the Token Array back to the compiler.

### The Parser
The parser recieves the linear array of tokens and is tasked with parsing it into a AST (abstract syntax tree). The tree is structured as follows:

* **N_TRANSLATION_UNIT**: Array of declarations of either a function or variable
* **N_FUNCTION**: Name, Params, and an **N_COMPOUND** block.
* **N_COMPOUND**: Array of statements.

The parser, starting from the very first token, uses its context and grammar to decide what node is next, but also if there is a syntax error. E.g., if the current token is a **TK_RETURN**, given the grammar below, we can expect an `expr` to follow. The parser then tries to parse an `expr` and if it succeeds, we combine the **TK_RETURN** token and parsed `expr` into a single Node, **N_RETURN**. This node is then appended to the **N_COMPOUND** parent node. Another example, if the current token is **TK_OPEN_CURLY**, given the grammar, this can only be the start of a **N_COMPOUND**. The parser knows to parse the following tokens as an array of statements and append them to the compound nodes list, and only stops when **TK_CLOSE_CURLY** is found. Same as the **Tokenizer**, when we reach the end of the token array, we return the parsed **AST** to the compiler.

### Semantic Analysis Pass
The **AST** constructed by the parser is mostly typeless. Only explictly typed expressions have type information. It is the job of the semantic analysis pass to give every node a correct type. While doing so, it also ensures correctness through type checking. In the majority of cases a binary operation requires two operands of the same type. If the operands are found to have different types, we try to promote them to the same common type. If this is not possible and their types and completely incompatible, a type error is thrown. Semantic Analysis also handles converting constructs which are simply syntactic sugar into their literal underlying forms. E.g 'a[5]' is just '*(a + sizeof(element) * 5)'. Similarily 'a->b' is just '*(a).b'. These conversions are handled in this pass.

### IR Gen
This step involves taking the **AST** and converting it from the recursive structured tree (where nodes link to nodes linking to other nodes), into a more linear and assembly like "Intermediate Representation", **IR**. The **IR** generator, traverses the tree according to our definition, and when it reaches a statement or expression, it converts it to corresponding **IR**  instructions. 

### Analysis Pass
When **IR** is generated, the instructions mostly use Virtual Registers. A virtual register is defined once and its value is read from many times. A virtual register does not get redefined nor does it actually exist in memory. The job of the Analysis pass is to replace these virtual registers with physical stack slots and registers. To achieve this, first a Control Flow Graph is generated from the **IR Blocks**. Each block is read to determine where it can branch to. The result is that every block has a list of successor blocks and predecessor blocks. The next step is to define what virtual registers a block defines/creates and what registers it uses but did not create. This is done by looping over every instruction within the block, it is known for each instruction which operand is a definition and which operands are just used. With this information we populate the blocks 'used' and 'defined' arrays.
The next step is to compute for each block, the virtual registers which come from outside the block and which exist after the block ends. These are called the 'live_in' and 'live_out' arrays. The details of this are complex and will be skipped for brevity. At this point it can be concluded that for every block, where a virtual register is in its 'live_in' but not the 'live_out', the register dies within the block. For such instances we search for the last occurence of it and set that as the registers end instruction. The start instruction of every virtual register is simply found by looping over all instructions and recording when it is defined.
With the liveness/lifetimes of every virtual register computed, we can now allocate them to reusable stack slots. This process is simple, given the virtual registers sorted from first defined to last defined, we check if their is a suitable stack slot evailiable, if so we assign the register to it. Otherwise we create a new stack slot with the correct size and set its next_free variable to the registers last_used. This results in stack slots being reused once their are free.

E.g., We reach a **N_RETURN** node. It has an `expr`, which we need to generate **IR** for before we can call **IR_RET**. First, we call `ir_gen_expression`, which creates the **IR** for the return expression and stores the result in some virtual register. `ir_gen_expression` then returns the register, which we can give to our **IR_RET** instruction. Now the **IR_RET** has only the register where its return value is. This makes converting to assembly very easy later on. Its similar to assembly but it only represents what the code will do, where assembly represents exactly what the computer will do (which as a human, is not always as clear). 

This simplicity is vital for easily translating **IR** into actual assembly, which takes place in the final step.

### x86 Gen
Here we lower the **IR** to raw assembly, replacing simplified **IR_BINARYOP**, or other instruction with hardware specific instructions. Loading memory to and from registers, handling bit flags etc. While the assembly can be tricky to come to terms with, converting the generated **IR** to x86-64 assembly is actually the easiest step. This is thanks to the work done by the Tokenizer, Parser and **IR** gen.

E.g., given our **IR_RET** instruction, we first take the given register (Which promises to store a return value) and load it into the `%rax` CPU register. This register is used for all return values. Then we reset the %rsp stack pointer back to the start of the functions stack frame, which was stored in %rbp. Then we pop the return address off the stack (Which is at the top of the stack, when we are at the start of the functions stack frame) and finally call `ret`. Which jumps back to the return address in `%rbp` and "brings" our return value along in `%rax`.

You can see how such a simple **IR** instruction can expand into a much more complex set of assembly instructions. This highlights the purpose of the intermediate representation, to hide away this overwhelming complexity so we can later focus on optimizaton and stuff.

# Explanation
Below is the grammar for these comments to help understanding.

Rules:
* *Italics for unimplemented*.
* '*' for any number, including zero.
* '+' for any non-zero number.
* '?' for optional
* '[]' container for consumption, wraps general things like `expr` or `var decl`
* [true] means any non-zero value
* [false] means a value of zero only

>### 1.  `(type)`
"any valid variable type"
* int 
* float
* *char*
* *double*

This will later include modifiers and any typedef'd types

>### 2. `identifier` 
"Any alpha-starting alpha-numeric string which may contain underscores '_' "

>### 3. `expr`
"Anything that can be evaluated"
* Function call
```c
    foo()
```
* Binary Operation
```c
    10 + a
```
* Unary Operation
```c
    i++
```
>### 4. `{compound}`
"A list of block items wrapped in curly braces"
```c
{
    [block item]*
}
```

>### 5. `block item`
"Either a `var decl` or a `statement`"

>### 6. `statement`
"Any self sufficient piece of code"
* `identifier`
* `{compound}`
* `expr`
* `;`

* Or Control Flow Statement
    * \[ `if`, `for`, `while`, `return`, `break`, `continue`, `switch` \]

>### 7. `var decl`
"A `(type)` followed by an `identifier`, may be uninitialized"
* Simple Variable declaration:
```c
int a;
```
* Variable declaration & Initialization:
```c
int a = 10;
```
>### 8. Standard C Statements
### `if` statement
Jumps to either `true` or `false` dependent on `cond`'s value 
*{0 => false, any other value => true}*
```c
if (cond) `true` [else `false`]?
```
* Where:
    * `cond` is any `expr`
    * `true` & `false` are any `statement`, most commonly `{compound}`
    * `else` is optional. When present, must be followed by a `statement`
    
### `while` loop
Executes `true` if `cond` is [true]
```c
while (cond) `true`
```
* Where:
    * `cond` is any `expr`
    * `true` is any `statement` most commonly `{compound}`

### `for` loop
Executes `init`, then on `cond` being [true], executes `true` then `iter` and finally loops back to check `cond`. 
* `init` is executed once at the start
* `cond` is checked before every iteration
* `true` is executed only if `cond` is [true]
* `iter` is executed after `true`
```c
for (init; cond; iter) `true`
```
* Where:
    * `init` is either `var decl` or `statement` 
    * `cond` & `iter` are any `expr`
    * `true` is any `statement` most commonly `{compound}`
    
### `return` statement
Returns control flow to callee, also returns a value. If no value is given, returns `0`.
```c
    return [`value`]?;
```
* Where:
    * `value` is an optional `expr`

# Implemented
* Scopes
* Variable Declaration
    * Shadowing
* Types:
    * Int, Float, Char, Double, Short, Long
* Literals 
    * Octal, Hexadecimal, Decimal, Binary
    * Floating Point
    * Strings
    * Char
* All Binary operators:
    * Arithmetic:  \[ `+`, `-`, `*`, `/` \]
    * Bitwise:  \[ `&`, `|`, `^`, `<<`, `>>` \]
    * Comparitive: \[ `<`, `>`, `<=`, `>=`, `==`, `!=` \]
    * Logical: \[ `&&`, `||` \]
    * Assignment
        * Any arithmetic or bitwise operator appended by `=`, assigns the value of the operation e.g `+=`.
* All Unary operators:
    * Arithmetic:  \[ `+`, `-`, `!`, `++`, `--` \]
    * Bitwise: \[ `~` ]
    * sizeof
* Control Flow
    * If/Else branching
    * For Loops
    * While Loops
    * Nested branches
* Functions
    * Passing Parameters
* Pointers & Addresses 
    *   [Pointer,int] Arithmetic
* Type conversion
    * `(int)x`, `(float *)y`
* Arrays
    * Declaring arrays
    * C Strings
    * Indexing with `[]`
* Array Initialization
    * Using `= {};`
        * Empty, Undersized, Oversized
    * size inference
    * Allow infered size allocation `int[]`
* Advanced Control Flow
    * continue, break
    * switch, case,
* Structs
    * `struct A {}`
    * Member access using `.` and `->`
    * Padding rules
    * Nested structs
*  Enums
    * `enum B {}`
    * Specify value `enum C { ONE = 1, TWO, }`
* Storage Specifiers
    * extern
    * static
* typedef
* Function ABI Calling Conventions
    * MS x64 ABI
    * Use registers for Parameters
    * Overflow 6/4+ to stack
    
## To be Implemented (Ordered from next to never...)
* Refactor for consistency
* Ensure correct order of stack spilled function params
* Variadic Functions
* Function ABI Calling Conventions
    * SysV ABI
    * Handle Structs
        * sizeof(struct A) < 16b => use registers
        *       ...        > 16b => use hidden pointer to copy on function stack 
    * Caller/Callee save registers
* Use physical registers
    * Overflow to stack
* Array Initialization
    * Compound literals `(Type){}`
* Pointers
    * [Pointer,Pointer] Arithmetic
    * Function pointers
* Advanced Control Flow:
    * goto & labels
* Literals
    * Handle overflows and multi char literal `char a = 'abcd';`
* Structs and Unions, Enums
    * Designated Initializers `.member = value` [C99+]
    * Bitfield in structs
        * `unsigned int flag : 1; // 1 bit`
* Variants
    * Unsigned
* Qualifiers
    * const, volatile
* preprocessor
    * `#define`, `#include` etc for conditional compilation.
* Inline functions
* Labels as values? (part of GCC, not standard C)
* Volatile/Atomic memory
* Create a standard library
    * `printf`
    * `malloc`
    * `free`
