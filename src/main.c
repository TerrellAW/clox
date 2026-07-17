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

	// Add 1.2 to the stack
	size_t constant = addConstant(&chunk, 1.2);
	writeChunk(&chunk, OP_CONSTANT, 3);
	writeChunk(&chunk, constant, 3);

	// Add 5 to the stack
	constant = addConstant(&chunk, 5);
	writeChunk(&chunk, OP_CONSTANT, 4);
	writeChunk(&chunk, constant, 4);

	// Multiply 1.2 by 5
	writeChunk(&chunk, OP_MULTIPLY, 5);

	// Add 24 to stack
	constant = addConstant(&chunk, 24);
	writeChunk(&chunk, OP_CONSTANT, 6);
	writeChunk(&chunk, constant, 6);

	// Add 0.4 to stack
	constant = addConstant(&chunk, 0.4);
	writeChunk(&chunk, OP_CONSTANT, 10);
	writeChunk(&chunk, constant, 10);

	// Divide 8 by 0.4	
	writeChunk(&chunk, OP_DIVIDE, 11);

	// Add 6 to 60
	writeChunk(&chunk, OP_ADD, 11);

	// Negate 66
	writeChunk(&chunk, OP_NEGATE, 13);

	// Add 108 to stack
	constant = addConstant(&chunk, 108);
	writeChunk(&chunk, OP_CONSTANT, 13);
	writeChunk(&chunk, constant, 13);

	// Add 108 to -66
	writeChunk(&chunk, OP_ADD, 13);

	// Return the meaning of life
	writeChunk(&chunk, OP_RETURN, 13);

	// Interpret code in virtual machine
	interpret(&chunk);

	// Free the virtual machine
	freeVM();

	freeChunk(&chunk);
	return EXIT_SUCCESS;
}
