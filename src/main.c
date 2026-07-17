/**
 * Main entry point
 */

#include "../include/common.h"
#include "../include/debug.h"
#include "../include/vm.h"

int main(int argc, const char* argv[]) {
	// Initialize the virtual machine
	initVM();

	Chunk chunk;
	initChunk(&chunk);

	// Add a value
	size_t constant = addConstant(&chunk, 1.2);
	writeChunk(&chunk, OP_CONSTANT, 3);
	writeChunk(&chunk, constant, 3);

	// Add many values
	constant = addConstant(&chunk, 66);
	writeChunk(&chunk, OP_CONSTANT, 6);
	writeChunk(&chunk, constant, 6);
	constant = addConstant(&chunk, 42);
	writeChunk(&chunk, OP_CONSTANT, 12);
	writeChunk(&chunk, constant, 12);
	constant = addConstant(&chunk, 888);
	writeChunk(&chunk, OP_CONSTANT, 12);
	writeChunk(&chunk, constant, 12);

	// Return statement
	writeChunk(&chunk, OP_RETURN, 12);

	// Interpret code in virtual machine
	interpret(&chunk);

	// Free the virtual machine
	freeVM();

	freeChunk(&chunk);
	return EXIT_SUCCESS;
}
