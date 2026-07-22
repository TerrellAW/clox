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

// Bytecode chunk being compiled
Chunk* compilingChunk;

// Return current bytecode chunk
static Chunk* currentChunk() {
	return compilingChunk;
}

// Handle error messaging and flag
static void errorAt(Token* token, const char* message) {
	// Enter panic mode to prevent error cascade
	if (parser.panicMode) return;
	parser.panicMode = true;
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

// Consume token if it matches argument, otherwise give error
static void consume(TokenType type, const char* message) {
	if (parser.current.type == type) {
		advance();
		return;
	}

	errorAtCurrent(message);
}

// Append a compiled byte to a chunk
static void emitByte(uint8_t byte) {
	writeChunk(currentChunk(), byte, parser.previous.line);
}

// Append two bytes to a chunk
static void emitBytes(uint8_t byte1, uint8_t byte2) {
	emitByte(byte1);
	emitByte(byte2);
}

// Append a return code to a chunk
static void emitReturn() {
	emitByte(OP_RETURN);
}

// Emit return code to end of chunk
static void endCompiler() {
	emitReturn();
}

// Handle expressions
static void expression() {
	//TODO
}

// Begin compilation of source to bytecode
bool compile(const char* source, Chunk* chunk) {
	// Initialize scanner
	initScanner(source);

	// Initialize chunk
	compilingChunk = chunk;

	// Initialize parser error handling
	parser.hadError = false;
	parser.panicMode = false;

	advance();
	expression();
	consume(TOKEN_EOF, "Expect end of expression.");

	// End program
	endCompiler();

	// Return success or failure
	return !parser.hadError;
}
