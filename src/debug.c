/**
 * Debug tools implementation
 */

#include <stdio.h>

#include "../include/debug.h"

// Disassemble chunk of bytecode
void disassembleChunk(Chunk* chunk, const char* name) {
	// Print name of chunk
	printf("== %s ==\n", name);

	// Disassemble instructions
	for (size_t offset = 0; offset < chunk->count;) 
		offset = disassembleInstruction(chunk, offset);
}

// Handle simple instruction
static size_t simpleInstruction(const char* name, size_t offset) {
	printf("%s\n", name);
	return offset + 1;
}

// Disassemble an instruction
size_t disassembleInstruction(Chunk* chunk, size_t offset) {
	// Print offset at beginning of line
	printf("%04d ", offset);

	// Print instruction
	uint8_t instruction = chunk->code[offset];
	switch (instruction) {
		case OP_RETURN:
			return simpleInstruction("OP_RETURN", offset);
		default:
			fprintf(stderr, "Unknown opcode %d\n", instruction);
			return offset + 1;
	}
}

