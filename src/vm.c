/**
 * Virtual machine implementations
 */

#include <stdarg.h>

#include "../include/common.h"
#include "../include/debug.h"
#include "../include/compiler.h"
#include "../include/vm.h"

// Single global virtual machine instance
VM vm;

// Reset stack pointer to beginning of array
static void resetStack() {
	vm.stackTop = vm.stack;
}

// Handle errors during interpreter runtime
// Can take an arbitrary number of arguments
static void runtimeError(const char* format, ...) {
	// List of arbitrary args
	va_list args;
	// Start of arbitrary args
	va_start(args, format);
	// Print that takes explicit va_list
	vfprintf(stderr, format, args);
	// End of arbitrary args
	va_end(args);
	// Add newline to stderr output
	fputs("\n", stderr);

	// Get and display error location
	size_t instruction = vm.ip - vm.chunk->code - 1;
	size_t line = vm.chunk->lines[instruction];
	fprintf(stderr, "[line %zu] in script\n", line);

	// Reset stack
	resetStack();
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

// Peek at a value at the given location in the stack
static Value peek(size_t distance) {
	return vm.stackTop[-1 - distance];
}

// Run vm's interpreter
InterpretResult run() {

// Read byte and increment pointer
#define READ_BYTE() (*vm.ip++)

// Read all bytes that make up a constant
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

// Do a binary operation with mathematical operators and push result to stack
// Do while loop allows scoping without errors from trailing semicolon
#define BINARY_OP(valueType, op) 							\
	do {													\
		if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) {	\
			runtimeError("Operands must be numbers.");		\
			return INTERPRET_RUNTIME_ERROR;					\
		}													\
		double b = AS_NUMBER(pop());						\
		double a = AS_NUMBER(pop());						\
		push(valueType(a op b));							\
	} while (false)

#ifdef DEBUG_TRACE_EXECUTION
	// Print stack trace header
	printf("== stack ==\n");
#endif

	// Main loop
	// Reads a single bytecode instruction each loop
	for (;;) {

// Disassemble and print instructions if in debug mode
#ifdef DEBUG_TRACE_EXECUTION
		// Print stack trace
		printf("Stack:\t");
		for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
			printf("[ ");
			printValue(*slot);
			printf(" ]");
		}
		printf("\n");
#endif

		uint8_t instruction;
		// Read byte & dispatch instruction based on opcode
		switch (instruction = READ_BYTE()) {
			case OP_CONSTANT:
				// Push value of constant into stack
				Value constant = READ_CONSTANT();
				push(constant);
				break;
			case OP_ADD:
				// Do an additive binary operation
				BINARY_OP(NUMBER_VAL, +);
				break;
			case OP_SUBTRACT:
				// Do a subtractive binary operation
				BINARY_OP(NUMBER_VAL, -);
				break;
			case OP_MULTIPLY:
				// Do a multiplicative binary operation
				BINARY_OP(NUMBER_VAL, *);
				break;
			case OP_DIVIDE:
				// Do a divisive binary operation
				BINARY_OP(NUMBER_VAL, /);
				break;
			case OP_NEGATE: 
				// Check if value is not a number
				if (!IS_NUMBER(peek(0))) {
					runtimeError("Operand must be a number.");
					return INTERPRET_RUNTIME_ERROR;
				}
				// Push negated number value to stack
				push(NUMBER_VAL(-AS_NUMBER(pop())));
				break;
			case OP_RETURN:
#ifdef DEBUG_TRACE_EXECUTION
				// Print stack trace end
				printf("\n");
#endif
				// Pop value from stack
				printValue(pop());
				printf("\n");
				return INTERPRET_OK;
		}
	}

// Undefine macros
#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

// Interpret and load source code into vm's compiler
InterpretResult interpret(const char* source) {
	// Initialize chunk of bytecode
	Chunk chunk;
	initChunk(&chunk);

	// Handle failed compile
	if (!compile(source, &chunk)) {
		freeChunk(&chunk);
		return INTERPRET_COMPILE_ERROR;
	}

	// Set vm pointers
	vm.chunk = &chunk;
	vm.ip	 = vm.chunk->code;

	// Execute code
	InterpretResult result = run();

	// Free memory
	freeChunk(&chunk);
	return result;
}

