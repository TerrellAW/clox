/**
 * Bytecode definitions
 */

#ifndef CLOX_CHUNK_H
#define CLOX_CHUNK_H

#include "common.h"
#include "value.h"

/**
 * Enum of operation codes
 */
typedef enum {
	OP_CONSTANT,
	OP_RETURN,
} OpCode;

/**
 * Dynamic array of opcode-constant chunks
 */
typedef struct {
	size_t count;
	size_t capacity;
	// Opcodes
	uint8_t* code;
	// Values associated with opcodes
	ValueArray constants;
} Chunk;

/**
 * Chunk array initializer
 * Initializes an empty chunk
 */
void initChunk(Chunk* chunk);

/**
 * Chunk array deallocator
 * Frees memory associated with a chunk of bytes
 */
void freeChunk(Chunk* chunk);

/**
 * Append a byte to the end of a chunk
 */
void writeChunk(Chunk* chunk, uint8_t byte);

/**
 * Add a new constant to a chunk and return its index
 */
size_t addConstant(Chunk* chunk, Value value);

#endif
