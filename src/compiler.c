/**
 * Compiler implementations
 */

#include "../include/common.h"
#include "../include/compiler.h"
#include "../include/scanner.h"

// Parser data structure
typedef struct {
	// Store two tokens
	Token current;
	Token previous;
	// Flag errors
	bool hadError;
	// Prevent error cascades
	bool panicMode;
} Parser;

// Global parser
Parser parser;

// Handle error messaging and flag
static void errorAt(Token* token, const char* message) {
	// Print first part of error message
	fprintf(stderr, "[line %zu] Error", token->line);

	// Print error location
	if (token->type == TOKEN_EOF) {
		fprintf(stderr, " at end");
	} else if (token->type == TOKEN_ERROR) {
		// Nothing
	} else {
		fprintf(stderr, " at '%.*s'", token->length, token->start);
	}

	// Print error message
	fprintf(stderr, ": %s\n", message);

	// Flag error
	parser.hadError = true;
}

// Handle error at current token
static void errorAtCurrent(const char* message) {
	errorAt(&parser.current, message);
}

// Advance to next token and store previous
static void advance() {
	// Store current in previous
	parser.previous = parser.current;

	// Get next token
	for (;;) {
		parser.current = scanToken();
		if (parser.current.type != TOKEN_ERROR) break;

		// Handle error token
		errorAtCurrent(parser.current.start);
	}
}

// Begin compilation of source to bytecode
bool compile(const char* source, Chunk* chunk) {
	// Initialize scanner
	initScanner(source);

	advance();
	expression();
	consume(TOKEN_EOF, "Expect end of expression.");

	// Return success or failure
	return !parser.hadError;
}
