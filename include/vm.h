/**
 * Virtual machine
 */

#ifndef CLOX_VM_H
#define CLOX_VM_H

#include "chunk.h"

/**
 * Virtual machine data structure
 * Executes an array of bytecode chunks
 */
typedef struct {
	// Bytecode chunk
	Chunk* chunk;
	// Instruction pointer
	uint8_t* ip;
} VM;

/**
 * Interpreter result enumerator
 * Contains possible results after interpreting a chunk
 */
typedef enum {
	INTERPRET_OK,
	INTERPRET_COMPILE_ERROR,
	INTERPRET_RUNTIME_ERROR
} InterpretResult;

/**
 * Virtual machine initializer
 */
void initVM();

/**
 * Virtual machine deallocator
 */
void freeVM();

/**
 * Virtual machine entry point
 * Begins interpreting code for execution
 * Returns a result from the InterpretResult enum
 */
InterpretResult interpret(Chunk* chunk);

#endif
