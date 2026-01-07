# Compiler C
Writting a compiler for C, which will eventually self compile.
# Parser
All p_parse functions are given comments describing what they consume.

Below is the grammar for these comments to help understanding.

Rules:
* *Italics for unimplemented*.
* '*' for any number, including zero.
* '+' for any non-zero number.
* '?' for optional
* '[]' container for consumption, wraps general things like `expr` or `var decl`

>### 1.  `(type)`
"any valid variable type"
* int 
* float
* *char*
* *double*

This will later include modifiers and any typedef'd types

>### 2. `ident` / `identifier` 
"any alpha-starting alpha numeric string which may contain '_'/underscores"

>### 3. `expr`
"anything that can be evaluated"
* Function call

    main();
* Arithmetic

    10 + a;
