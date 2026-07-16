/**
 * Main Virtual Machine processing
 */

#include "../include/common.h"
#include "../include/debug.h"

int main(int argc, const char* argv[]) {
	Chunk chunk;
	initChunk(&chunk);

	// Add a value
	size_t constant = addConstant(&chunk, 1.2);
	writeChunk(&chunk, OP_CONSTANT, 3);
	writeChunk(&chunk, constant, 3);

	// Return statement
	writeChunk(&chunk, OP_RETURN, 3);

	disassembleChunk(&chunk, "test chunk");
	freeChunk(&chunk);
	return EXIT_SUCCESS;
}
