<div align="center">

  # clox

</div>

This is an implementation of the clox programming language from Robert Nystrom's book; [Crafting Interpreters](https://craftinginterpreters.com).

## Build

Simply run the `make` command.
It will automatically clean out old build results and place the executable in the `bin/` directory.
Generate a configuration file for the clangd language server with `bear -- make`.

### Prerequisites

- C compiler
- Make

## Syntax

Example code written in DotLox can be found [here](https://github.com/TerrellAW/DotLox/tree/main/examples).

The grammar can be found [here](https://github.com/TerrellAW/DotLox/blob/main/doc/GRAMMAR.md). 

Further information on the Lox programming language can be found [here](https://github.com/munificent/craftinginterpreters).

## Compiler

The compiler compiles Lox code into bytecode which is interpreted by the virtual machine.

The bytecode reference can be found [here](doc/BYTECODE.md).

## References

Robert Nystrom's book, [Crafting Interpreters](https://craftinginterpreters.com).
