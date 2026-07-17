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

	// Add a values to the stack
	size_t constant = addConstant(&chunk, 1.2);
	writeChunk(&chunk, OP_CONSTANT, 3);
	writeChunk(&chunk, constant, 3);
	constant = addConstant(&chunk, 66);
	writeChunk(&chunk, OP_CONSTANT, 6);
	writeChunk(&chunk, constant, 6);

	// Negate value at top of stack
	writeChunk(&chunk, OP_NEGATE, 13);

	// Return value at top of stack
	writeChunk(&chunk, OP_RETURN, 13);

	// Interpret code in virtual machine
	interpret(&chunk);

	// Free the virtual machine
	freeVM();

	freeChunk(&chunk);
	return EXIT_SUCCESS;
}
