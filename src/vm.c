/**
 * Virtual machine implementations
 */

#include <stdio.h>

#include "../include/common.h"
#include "../include/vm.h"

// Single global virtual machine instance
VM vm;

// Initialize vm
void initVM() {

}

// Free vm
void freeVM() {

}

// Run vm's interpreter
InterpretResult run() {
// Read byte and increment pointer
#define READ_BYTE() (*vm.ip++)
// Read all bytes that make up a constant
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

	// Main loop
	// Reads a single bytecode instruction each loop
	for (;;) {
		uint8_t instruction;
		// Read byte & dispatch instruction based on opcode
		switch (instruction = READ_BYTE()) {
			case OP_CONSTANT: {
				// Process value of constant
				Value constant = READ_CONSTANT();
				printValue(constant);
				printf("\n");
				break;
			}
			case OP_RETURN: {
				return INTERPRET_OK;
			}
		}
	}

// Undefine macros
#undef READ_BYTE
#undef READ_CONSTANT
}

// Load chunk of bytecode into vm's interpreter
InterpretResult interpret(Chunk* chunk) {
	vm.chunk = chunk;
	vm.ip = vm.chunk->code;
	return run();
}

