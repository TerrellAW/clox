/**
 * Compiler implementations
 */

#include "../include/common.h"
#include "../include/compiler.h"
#include "../include/scanner.h"

// Begin compilation of source to bytecode
void compile(const char* source) {
	// Initialize scanner
	initScanner(source);

	// TODO: Remove this
	size_t line = -1;
	for (;;) {
		Token token = scanToken();
		if (token.line != line) {
			printf("%4d ", token.line);
			line = token.line;
		} else {
			printf("   | ");
		}
		printf("%2d '%.*s'\n", token.type, token.length, token.start);

		if (token.type == TOKEN_EOF) break;
	}
}
