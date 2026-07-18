/**
 * Main entry point
 */

#include <string.h>

#include "../include/common.h"
#include "../include/error.h"
#include "../include/vm.h"

// Runs Read-Eval-Print Loop
static void repl() {
	// Can only handle limited amount of characters
	// TODO: Use a dynamic array
	char line[1024];

	for (;;) {
		printf("> ");

		// Exit if EOF registered on stdin
		if (!fgets(line, sizeof(line), stdin)) {
			printf("\n");
			break;
		}

		// Run interpreter on inputted line
		interpret(line);
	}
}

// Read file and dynamically allocate string for its contents
static char* readFile(const char* path) {
	// Open file
	FILE* file = fopen(path, "rb");

	// Handle failure to open file
	if (file == NULL) {
		fprintf(stderr, "Could not open file \"%s\".\n", path);
		exit(READ_FAILURE);
	}

	// Get file size
	fseek(file, 0L, SEEK_END);
	size_t fileSize = ftell(file);
	rewind(file);

	// Allocate a memory buffer for file contents with extra space for EOF
	char* buffer = (char*)malloc(fileSize + 1);

	// Handle not enough memory
	if (buffer == NULL) {
		fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
		exit(READ_FAILURE);
	}

	// Read to end of file
	size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);

	// Handle read failure
	if (bytesRead < fileSize) {
		fprintf(stderr, "Could not read file \"%s\".\n", path);
		exit(READ_FAILURE);
	}

	// Add EOF
	buffer[bytesRead] = '\0';

	// Close file reader
	fclose(file);

	// Return buffer
	return buffer;
}

// Compiles and runs a file
static void runFile(const char* path) {
	// Read source file
	char* source = readFile(path);

	// Store result from interpreter
	InterpretResult result = interpret(source);

	// Free memory
	free(source);

	// Handle error results
	if (result == INTERPRET_COMPILE_ERROR) exit(COMPILE_FAILURE);
	if (result == INTERPRET_RUNTIME_ERROR) exit(RUNTIME_FAILURE);
}

int main(int argc, const char* argv[]) {
	// Initialize the virtual machine
	initVM();

	// Process arguments
	if (argc == 1) {
		// If no file specified enter REPL
		repl();
	} else if (argc == 2) {
		// If file specified run it
		runFile(argv[1]);
	} else {
		// Give error
		fprintf(stderr, "Usage: clox [path]\n");
		exit(START_FAILURE);
	}

	// Free the virtual machine and exit
	freeVM();
	return EXIT_SUCCESS;
}
