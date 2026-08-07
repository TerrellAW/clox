/**
 * Debug tools implementation
 */

#include "debug.h"
#include "value.h"

// Disassemble chunk of bytecode
void disassembleChunk(Chunk* chunk, const char* name) {
	// Print name of chunk
	printf("\n== %s ==\n", name);

	// Disassemble instructions
	for (size_t offset = 0; offset < chunk->count;) 
		offset = disassembleInstruction(chunk, offset);

	printf("\n");
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
		case OP_NIL:
			return simpleInstruction("OP_NIL", offset);
		case OP_TRUE:
			return simpleInstruction("OP_TRUE", offset);
		case OP_FALSE:
			return simpleInstruction("OP_FALSE", offset);
		case OP_POP:
			return simpleInstruction("OP_POP", offset);
		case OP_EQUAL:
			return simpleInstruction("OP_EQUAL", offset);
		case OP_GREATER:
			return simpleInstruction("OP_GREATER", offset);
		case OP_LESS:
			return simpleInstruction("OP_LESS", offset);
		case OP_ADD:
			return simpleInstruction("OP_ADD", offset);
		case OP_SUBTRACT:
			return simpleInstruction("OP_SUBTRACT", offset);
		case OP_MULTIPLY:
			return simpleInstruction("OP_MULTIPLY", offset);
		case OP_DIVIDE:
			return simpleInstruction("OP_DIVIDE", offset);
		case OP_NOT:
			return simpleInstruction("OP_NOT", offset);
		case OP_NEGATE:
			return simpleInstruction("OP_NEGATE", offset);
		case OP_PRINT:
			return simpleInstruction("OP_PRINT", offset);
		case OP_RETURN:
			return simpleInstruction("OP_RETURN", offset);
		default:
			fprintf(stderr, "Unknown opcode %d\n", instruction);
			return offset + 1;
	}
}

