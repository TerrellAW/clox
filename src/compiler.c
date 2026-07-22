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

// Order of precedence
typedef enum {
	PREC_NONE,
	PREC_ASSIGNMENT,
	PREC_OR,
	PREC_AND,
	PREC_EQUALITY,
	PREC_COMPARISON,
	PREC_TERM,
	PREC_FACTOR,
	PREC_UNARY,
	PREC_CALL,
	PREC_PRIMARY
} Precedence;

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

// Report error
static void error(const char* message) {
	errorAt(&parser.previous, message);
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

// Handle constant value
static uint8_t makeConstant(Value value) {
	// Add value to constant pool and retrieve its index
	size_t constant = addConstant(currentChunk(), value);

	// Ensure index can be converted to 8-bit integer
	if (constant > UINT8_MAX) {
		error("Too many constants in one chunk.");
		return 0;
	}

	// Return index as 8-bit integer
	return (uint8_t)constant;
}

// Append a constant code and operand to a chunk
static void emitConstant(Value value) {
	emitBytes(OP_CONSTANT, makeConstant(value));
}

// Emit return code to end of chunk
static void endCompiler() {
	emitReturn();
}

// Ensure proper parsing precedence
static void parsePrecedence(Precedence precedence) {
	//TODO
}

// Handle expressions
static void expression() {
	parsePrecedence(PREC_ASSIGNMENT);
}

// Handle parenthesized groupings
static void grouping() {
	// Handle grouped expression
	expression();

	// Look for closing parenthesis
	consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

// Handle number token
static void number() {
	// Convert string to double
	double value = strtod(parser.previous.start, NULL);

	// Push to runtime stack with constant opcode
	emitConstant(value);
}

// Handle unary expression
static void unary() {
	// Get operator from previous token
	TokenType operatorType = parser.previous.type;

	// Compile the operand
	parsePrecedence(PREC_UNARY);

	// Emit operator instruction
	switch (operatorType) {
		case TOKEN_MINUS: emitByte(OP_NEGATE); break;
		default: return;
	}
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

	// Consume first character
	advance();

	// Parse and compile expression
	expression();

	// Look for end of file
	consume(TOKEN_EOF, "Expect end of expression.");

	// End program
	endCompiler();

	// Return success or failure
	return !parser.hadError;
}
