/**
 * Debug tools implementation
 */

#include <stdio.h>

#include "../include/debug.h"
#include "../include/value.h"

// Disassemble chunk of bytecode
void disassembleChunk(Chunk* chunk, const char* name) {
	// Print name of chunk
	printf("== %s ==\n", name);

	// Disassemble instructions
	for (size_t offset = 0; offset < chunk->count;) 
		offset = disassembleInstruction(chunk, offset);
}

// Handle constant value instruction
static size_t constantInstruction(const char* name, Chunk* chunk, size_t offset) {
	uint8_t constant = chunk->code[offset + 1];

	// Print name and index of constant
	printf("%-16s %4d '", name, constant);

	// Print value of constant
	printValue(chunk->constants.values[constant]);

	// End line
	printf("'\n");

	// Return offset for next instruction
	return offset + 2;
}

// Handle simple instruction
static size_t simpleInstruction(const char* name, size_t offset) {
	printf("%s\n", name);
	return offset + 1;
}

// Disassemble an instruction
size_t disassembleInstruction(Chunk* chunk, size_t offset) {
	// Print offset at beginning of line
	printf("%04zu ", offset);

	// If same number as last line print vertical line
	if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1]) {
		printf("\t| ");
	// Else print source line number
	} else {
		printf("%4zu ", chunk->lines[offset]);
	}

	// Print instruction
	uint8_t instruction = chunk->code[offset];
	switch (instruction) {
		case OP_CONSTANT:
			return constantInstruction("OP_CONSTANT", chunk, offset);
		case OP_RETURN:
			return simpleInstruction("OP_RETURN", offset);
		default:
			fprintf(stderr, "Unknown opcode %d\n", instruction);
			return offset + 1;
	}
}

