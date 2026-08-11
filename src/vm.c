/**
 * Virtual machine implementations
 */

#include <stdarg.h>
#include <string.h>

#include "common.h"
#include "debug.h"
#include "object.h"
#include "memory.h"
#include "compiler.h"
#include "vm.h"

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
	vm.objects = NULL;
	initTable(&vm.globals);
	initTable(&vm.strings);
}

// Free vm
void freeVM() {
	// Free global variables
	freeTable(&vm.globals);
	// Free internal strings
	freeTable(&vm.strings);
	// Free all remaining objects
	freeObjects();
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

// Determine if a value is falsey
static bool isFalsey(Value value) {
	return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

// Concatenate two values from the stack into a string
static void concatenate() {
	// Pop two values from top of stack as strings
	ObjString* b = AS_STRING(pop());
	ObjString* a = AS_STRING(pop());

	// Get total length of both strings
	size_t length = a->length + b->length;

	// Allocate space for new string
	char* chars = ALLOCATE(char, length + 1);

	// Copy strings into new string
	memcpy(chars, a->chars, a->length);
	memcpy(chars + a->length, b->chars, b->length);

	// Add null terminator
	chars[length] = '\0';

	// Push result back onto stack
	ObjString* result = takeString(chars, length);
	push(OBJ_VAL(result));
}

// Run vm's interpreter
InterpretResult run() {

// Read byte and increment pointer
#define READ_BYTE() (*vm.ip++)

// Read all bytes that make up a constant
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

// Return string from constant pool
#define READ_STRING() AS_STRING(READ_CONSTANT());

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
			case OP_NIL:
				// Push nil onto the stack
				push(NIL_VAL);
				break;
			case OP_TRUE:
				// Push true onto the stack
				push(BOOL_VAL(true));
				break;
			case OP_FALSE:
				// Push false onto the stack
				push(BOOL_VAL(false));
				break;
			case OP_POP:
				// Pop value from top of stack
				pop();
				break;
			case OP_GET_LOCAL: {
				// Get slot of local variable
				uint8_t slot = READ_BYTE();

				// Push value from slot to top of stack
				push(vm.stack[slot]);
				break;
			}
			case OP_SET_LOCAL: {
				// Set slot for local variable
				uint8_t slot = READ_BYTE();

				// Put value in its slot
				vm.stack[slot] = peek(0);
				break;
			}
			case OP_GET_GLOBAL: {
				// Get identifier name from constant pool
				ObjString* name = READ_STRING();

				// Initialize variable to store global's value
				Value value;

				// Try to get global's value
				if (!tableGet(&vm.globals, name, &value)) {
					// Report error if failed
					runtimeError("Undefined variable '%s'.", name->chars);

					// End interpreter with runtime error
					return INTERPRET_RUNTIME_ERROR;
				}
				
				// Push global's value to the stack
				push(value);
				break;
			}
			case OP_DEFINE_GLOBAL: {
				// Get identifier name from constant pool
				ObjString* name = READ_STRING();

				// Add to globals hash table and pop from stack
				tableSet(&vm.globals, name, peek(0));
				pop();
				break;
			}
			case OP_SET_GLOBAL: {
				// Get identifier name from constant pool
				ObjString* name = READ_STRING();

				// Update value of global if it exists, else runtime error
				if (tableSet(&vm.globals, name, peek(0))) {
					// Delete newly created variable
					tableDelete(&vm.globals, name);
					// Fail with runtime error
					runtimeError("Undefined variable '%s'.", name->chars);
					return INTERPRET_RUNTIME_ERROR;
				}
				break;
			}
			case OP_EQUAL:
				// Pop two values from the stack
				Value a = pop();
				Value b = pop();
				// Check equality and push result
				push(BOOL_VAL(valuesEqual(a, b)));
				break;
			case OP_GREATER:
				// Do a greater than comparison
				BINARY_OP(BOOL_VAL, >);
				break;
			case OP_LESS:
				// Do a less than comparison
				BINARY_OP(BOOL_VAL, <);
				break;
			case OP_ADD:
				// Handle string concatenation
				if (IS_STRING(peek(0)) && IS_STRING(peek(1))) {
					concatenate();
				// Handle numeric addition
				} else if (IS_NUMBER(peek(0)) && IS_NUMBER(peek(1))) {
					// Do an additive binary operation
					double b = AS_NUMBER(pop());
					double a = AS_NUMBER(pop());
					push(NUMBER_VAL(a + b));
				// Other types gives runtime error
				// TODO: Handle other types concatenation
				// TODO: Create a macro that can concatenate any data type into a string
				} else {
					runtimeError("Operands must be two numbers or two strings.");
					return INTERPRET_RUNTIME_ERROR;
				}
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
			case OP_NOT:
				// Check if value is falsy and push to stack as bool
				push(BOOL_VAL(isFalsey(pop())));
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
			case OP_PRINT:
				// Pop value from stack and print
				printValue(pop());

				// Print newline at end of value
				printf("\n");
				break;
			case OP_RETURN:
#ifdef DEBUG_TRACE_EXECUTION
				// Print stack trace end
				printf("\n");
#endif
				// Exit interpreter
				return INTERPRET_OK;
		}
	}

// Undefine macros
#undef READ_BYTE
#undef READ_CONSTANT
#undef READ_STRING
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

