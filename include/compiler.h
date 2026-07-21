/**
 * Compiler
 * Compile Lox source code to the vm's bytecode
 */

#ifndef CLOX_COMPILER_H
#define CLOX_COMPILER_H

#include "vm.h"

/**
 * Compiler entry point
 * Compiles source code to bytecode chunk 
 * Returns true if successful, false if error occurred
 */
bool compile(const char* source, Chunk* chunk);

#endif
