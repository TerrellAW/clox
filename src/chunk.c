/**
 * Bytecode chunk implementations
 */

#include "../include/chunk.h"
#include "../include/memory.h"

// Chunk array initializer
void initChunk(Chunk* chunk) {
	chunk->count = 0;
	chunk->capacity = 0;
	chunk->code = NULL;
	// Initialize constants with out-pointer
	initValueArray(&chunk->constants);
}

// Free chunk array memory
void freeChunk(Chunk* chunk) {
	// Free memory
	FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
	freeValueArray(&chunk->constants);

	// Re-initialize array
	initChunk(chunk);
}

// Append a byte to the end of a chunk
void writeChunk(Chunk* chunk, uint8_t byte) {
	// Check if capacity has been reached
	if (chunk->capacity < chunk->count + 1) {
		size_t oldCapacity = chunk->capacity;
		// Grow capacity dynamically
		chunk->capacity = GROW_CAPACITY(oldCapacity);
		chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
	}

	// Append byte to the end
	chunk->code[chunk->count] = byte;
	chunk->count++;
}

// Add a new constant to a chunk
size_t addConstant(Chunk* chunk, Value value) {
	writeValueArray(&chunk->constants, value);
	// Return index
	return chunk->constants.count - 1;
}

