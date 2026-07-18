/**
 * Virtual machine
 */

#ifndef CLOX_VM_H
#define CLOX_VM_H

#include "chunk.h"
#include "value.h"

/**
 * Define maximum size of a stack
 */
#define STACK_MAX 256

/**
 * Virtual machine data structure
 * Executes an array of bytecode chunks
 */
typedef struct {
	// Bytecode chunk
	Chunk* chunk;
	// Instruction pointer
	uint8_t* ip;
	// Value stack
	Value stack[STACK_MAX];
	Value* stackTop;
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
 * Push a value to the top of the stack
 */
void push(Value value);

/**
 * Pop a value from the top of the stack
 */
Value pop();

/**
 * Virtual machine entry point
 * Begins interpreting code for execution
 * Returns a result from the InterpretResult enum
 */
InterpretResult interpret(const char* source);

#endif
