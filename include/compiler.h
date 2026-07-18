/**
 * Compiler
 * Compile Lox source code to the vm's bytecode
 */

#ifndef CLOX_COMPILER_H
#define CLOX_COMPILER_H

/**
 * Compiler entry point
 * Compiles source code to bytecode for the virtual machine to execute
 */
void compile(const char* source);

#endif
