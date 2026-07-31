<div align="center">

  # clox

</div>

This is an implementation of the clox programming language from Robert Nystrom's book; [Crafting Interpreters](https://craftinginterpreters.com).

## Build

Simply run the `make` command.
It will automatically clean out old build results and place the executable in the `bin/` directory.

To automatically clean out old build results and configs and then build a new executable and configs with Bear, run `make bear`.

### Prerequisites

- [C compiler](https://gcc.gnu.org/)
- [Make](https://ftp.gnu.org/old-gnu/Manuals/make-3.80/html_node/make.html)
- [Bear (Optional)](https://github.com/rizsotto/bear)

## Syntax

Example code written in DotLox can be found [here](https://github.com/TerrellAW/DotLox/tree/main/examples).

The grammar can be found [here](https://github.com/TerrellAW/DotLox/blob/main/doc/GRAMMAR.md). 

Further information on the Lox programming language can be found [here](https://github.com/munificent/craftinginterpreters).

## Compiler

The compiler compiles Lox code into bytecode which is interpreted by the virtual machine.

The bytecode reference can be found [here](doc/BYTECODE.md).

## References

Robert Nystrom's book, [Crafting Interpreters](https://craftinginterpreters.com).
