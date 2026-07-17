/**
 * Virtual machine implementations
 */

#include <stdio.h>

#include "../include/common.h"
#include "../include/debug.h"
#include "../include/vm.h"

// Single global virtual machine instance
VM vm;

// Reset stack pointer to beginning of array
static void resetStack() {
	vm.stackTop = vm.stack;
}

// Initialize vm
void initVM() {
	resetStack();
}

// Free vm
void freeVM() {

}

// Push value to stack
void push(Value value) {
	// Add value to ontop of stack
	*vm.stackTop = value;
	// Move pointer past value
	vm.stackTop++;
}

// Pop value from stack
Value pop() {
	// Decrement stack pointer to value
	vm.stackTop--;
	// Return value
	return *vm.stackTop;
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

// Disassemble and print instructions if in debug mode
#ifdef DEBUG_TRACE_EXECUTION
		// Print stack trace
		printf("\t  Stack: \t");
		for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
			printf("[ ");
			printValue(*slot);
			printf(" ]");
		}
		printf("\n");
		// Log instruction in bytecode
		disassembleInstruction(vm.chunk, (size_t)(vm.ip - vm.chunk->code));
#endif

		uint8_t instruction;
		// Read byte & dispatch instruction based on opcode
		switch (instruction = READ_BYTE()) {
			case OP_CONSTANT:
				// Push value of constant into stack
				Value constant = READ_CONSTANT();
				push(constant);
				break;
			case OP_NEGATE: 
				// Push negated value to stack
				push(-pop());
				break;
			case OP_RETURN:
				// Pop value from stack
				printValue(pop());
				printf("\n");
				return INTERPRET_OK;
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

