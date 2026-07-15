/**
 * Bytecode definitions
 */

#ifndef CLOX_CHUNK_H
#define CLOX_CHUNK_H

#include "common.h"

/**
 * Enum of operation codes
 */
typedef enum {
	OP_RETURN,
} OpCode;

/**
 * Dynamic array of opcode chunks
 */
typedef struct {
	size_t count;
	size_t capacity;
	uint8_t* code;
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

#endif
