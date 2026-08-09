/**
 * Compiler implementations
 */

#include "common.h"
#include "compiler.h"
#include "scanner.h"

// Conditional debug library
#ifdef DEBUG_PRINT_CODE
#include "debug.h"
#endif

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

// Function pointer for ParseFn
typedef void (*ParseFn)(bool canAssign);

// Rule from a row in the parsing table
typedef struct {
	ParseFn prefix;
	ParseFn infix;
	Precedence precedence;
} ParseRule;

// Forward declarations
static void expression();
static void statement();
static void declaration();
static ParseRule* getRule(TokenType type);

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
		fprintf(stderr, " at '%.*s'", (int)token->length, token->start);
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

// Check if token type matches
static bool check(TokenType type) {
	return parser.current.type == type;
}

// Advance if token type matches
static bool match(TokenType type) {
	// Stop if token type doesn't match
	if (!check(type)) return false;
	// Advance if it does
	advance();
	return true;
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

// Dump chunk of bytecode to terminal for debugging
#ifdef DEBUG_PRINT_CODE
	if (!parser.hadError) {
		disassembleChunk(currentChunk(), "code");
	}
#endif
}

// Ensure proper parsing precedence
static void parsePrecedence(Precedence precedence) {
	// Consume next token
	advance();

	// Look up the token's parse rule
	ParseFn prefixRule = getRule(parser.previous.type)->prefix;

	// If rule is null report syntax error
	if (prefixRule == NULL) {
		error("Expect expression.");
		return;
	}

	// Ensure assignment only occurs at low enough precedence
	bool canAssign = precedence <= PREC_ASSIGNMENT;
	prefixRule(canAssign);

	// Handle infix expression
	while (precedence <= getRule(parser.current.type)->precedence) {
		// Consume next token
		advance();
		
		// Handle precedence
		ParseFn infixRule = getRule(parser.previous.type)->infix;
		infixRule(canAssign);
	}

	// Report invalid assignment
	if (canAssign && match(TOKEN_EQUAL)) {
		error("Invalid assignment target.");
	}
}

// Add identifier to constant pool and return its index
static uint8_t identifierConstant(Token* name) {
	return makeConstant(OBJ_VAL(copyString(name->start, name->length)));
}

// Try to parse variable name, report error if it failes
static uint8_t parseVariable(const char* errorMessage) {
	// Check for identifier
	consume(TOKEN_IDENT, errorMessage);

	// Make an identifier constant and return its index
	return identifierConstant(&parser.previous);
}

// Define global variable on the stack
static void defineVariable(uint8_t global) {
	emitBytes(OP_DEFINE_GLOBAL, global);
}

// Handle expression
static void expression() {
	parsePrecedence(PREC_ASSIGNMENT);
}

// Handle variable declaration
static void varDeclaration() {
	// Parse and store global variable
	uint8_t global = parseVariable("Expect variable name.");

	// Parse expression and store result
	if (match(TOKEN_EQUAL)) {
		expression();
	// Set as nil if no initializer
	} else {
		emitByte(OP_NIL);
	}

	// Check for semicolon
	consume(TOKEN_SEMICOLON, "Expect ';' after variable declaration.");

	// Create the global variable on the stack
	defineVariable(global);
}

// Handle expression statement
static void expressionStatement() {
	// Parse expression
	expression();

	// Find end of line
	consume(TOKEN_SEMICOLON, "Expect ';' after value.");

	// Instruction to pop value from the stack
	emitByte(OP_POP);
}

// Handle print statement
static void printStatement() {
	// Parse expression
	expression();
	
	// Find end of line
	consume(TOKEN_SEMICOLON, "Expect ';' after value.");

	// Instruction to print result of expression
	emitByte(OP_PRINT);
}

// Handle error synchronization
// Ignores tokens until a statement boundary is found
static void synchronize() {
	// Reset panic mode flag
	parser.panicMode = false;

	// Consume until statement boundary
	while (parser.current.type != EOF) {
		// Stop at end of line
		if (parser.previous.type == TOKEN_SEMICOLON) return;

		// Stop at next statement
		switch (parser.current.type) {
			case TOKEN_CLASS:
			case TOKEN_FUN:
			case TOKEN_VAR:
			case TOKEN_FOR:
			case TOKEN_IF:
			case TOKEN_WHILE:
			case TOKEN_PRINT:
			case TOKEN_RETURN:
				return;

			default:
				/* Nothing */;
		}

		// Advance without parsing
		advance();
	}
}

// Handle declarations
static void declaration() {
	// If variable declaration forward to varDeclaration()
	if (match(TOKEN_VAR)) {
		varDeclaration();
	// If not a declaration forward to statement()
	} else {
		statement();
	}

	// Start error synchronization
	if (parser.panicMode) synchronize();
}

// Handle statements
static void statement() {
	// Handle print statement
	if (match(TOKEN_PRINT)) {
		printStatement();
	// Handle expression statement
	} else {
		expressionStatement();
	}
}

// Handle parenthesized groupings
static void grouping(bool canAssign) {
	// Handle grouped expression
	expression();

	// Look for closing parenthesis
	consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

// Handle number token
static void number(bool canAssign) {
	// Convert string to double
	double value = strtod(parser.previous.start, NULL);

	// Push to runtime stack with constant opcode
	emitConstant(NUMBER_VAL(value));
}

// Handle string token
static void string(bool canAssign) {
	emitConstant(OBJ_VAL(copyString(parser.previous.start + 1, parser.previous.length - 2)));
}

// Load variable
static void namedVariable(Token name, bool canAssign) {
	// Get identifier's index
	uint8_t arg = identifierConstant(&name);

	// Set global if assignment
	if (canAssign && match(TOKEN_EQUAL)) {
		// Parse expression
		expression();
		// Store result in global variable
		emitBytes(OP_SET_GLOBAL, arg);
	// Else get global
	} else {
		// Op code to get global variable
		emitBytes(OP_GET_GLOBAL, arg);
	}
}

// Handle variables
static void variable(bool canAssign) {
	namedVariable(parser.previous, canAssign);
}

// Handle binary expression
static void binary(bool canAssign) {
	// Get operator from previous token
	TokenType operatorType = parser.previous.type;

	// Get parsing rule
	ParseRule* rule = getRule(operatorType);

	// Get precedence
	parsePrecedence((Precedence)(rule->precedence + 1));

	// Handle operation
	switch (operatorType) {
		case TOKEN_BANG_EQUAL:		emitBytes(OP_EQUAL, OP_NOT);	break;
		case TOKEN_EQUAL_EQUAL:		emitByte(OP_EQUAL);				break;
		case TOKEN_GREATER:			emitByte(OP_GREATER);			break;
		case TOKEN_GREATER_EQUAL:	emitBytes(OP_LESS, OP_NOT);		break;
		case TOKEN_LESS:			emitByte(OP_LESS);				break;
		case TOKEN_LESS_EQUAL:		emitBytes(OP_GREATER, OP_NOT);	break;
		case TOKEN_PLUS:			emitByte(OP_ADD);				break;
		case TOKEN_MINUS:			emitByte(OP_SUBTRACT);			break;
		case TOKEN_STAR:			emitByte(OP_MULTIPLY);			break;
		case TOKEN_SLASH:			emitByte(OP_DIVIDE);			break;

		default: return;
	}
}

// Handle literals with dedicated opcodes
static void literal(bool canAssign) {
	switch (parser.previous.type) {
		case TOKEN_FALSE: 	emitByte(OP_FALSE);	break;
		case TOKEN_NIL:		emitByte(OP_NIL);	break;
		case TOKEN_TRUE:	emitByte(OP_TRUE);	break;
		default: return;
	}
}

// Handle unary expression
static void unary(bool canAssign) {
	// Get operator from previous token
	TokenType operatorType = parser.previous.type;

	// Compile the operand
	parsePrecedence(PREC_UNARY);

	// Emit operator instruction
	switch (operatorType) {
		case TOKEN_BANG:	emitByte(OP_NOT);	 break;
		case TOKEN_MINUS: 	emitByte(OP_NEGATE); break;
		default: return;
	}
}

// Array of ParseRules, the parser table
// Uses designated initializer syntax to avoid having to count indexes
ParseRule rules[] = {
    [TOKEN_LEFT_PAREN]    = {grouping,  NULL,	  PREC_NONE},
    [TOKEN_RIGHT_PAREN]   = {NULL, 		NULL,	  PREC_NONE},
    [TOKEN_LEFT_BRACE]    = {NULL, 		NULL,	  PREC_NONE},
    [TOKEN_RIGHT_BRACE]   = {NULL, 		NULL,	  PREC_NONE},
    [TOKEN_COMMA]         = {NULL, 		NULL,	  PREC_NONE},
    [TOKEN_DOT]           = {NULL, 		NULL,	  PREC_NONE},
    [TOKEN_MINUS]         = {unary,		binary,   PREC_TERM},
    [TOKEN_PLUS]          = {NULL, 		binary,   PREC_TERM},
    [TOKEN_SEMICOLON]     = {NULL, 		NULL, 	  PREC_NONE},
    [TOKEN_SLASH]         = {NULL, 		binary,   PREC_FACTOR},
    [TOKEN_STAR]          = {NULL, 		binary,   PREC_FACTOR},
    [TOKEN_BANG]          = {unary,		NULL, 	  PREC_NONE},
    [TOKEN_BANG_EQUAL]    = {NULL, 		binary,   PREC_EQUALITY},
    [TOKEN_EQUAL]         = {NULL, 		NULL, 	  PREC_NONE},
    [TOKEN_EQUAL_EQUAL]   = {NULL, 		binary,	  PREC_EQUALITY},
    [TOKEN_GREATER]       = {NULL, 		binary,	  PREC_COMPARISON},
    [TOKEN_GREATER_EQUAL] = {NULL, 		binary,	  PREC_COMPARISON},
    [TOKEN_LESS]          = {NULL, 		binary,	  PREC_COMPARISON},
    [TOKEN_LESS_EQUAL]    = {NULL, 		binary,	  PREC_COMPARISON},
    [TOKEN_IDENT]         = {variable,	NULL,	  PREC_NONE},
    [TOKEN_STRING]        = {string,	NULL,	  PREC_NONE},
    [TOKEN_NUMBER]        = {number, 	NULL,	  PREC_NONE},
    [TOKEN_AND]           = {NULL,		NULL,	  PREC_NONE},
    [TOKEN_CLASS]         = {NULL,	 	NULL,	  PREC_NONE},
    [TOKEN_ELSE]          = {NULL,	 	NULL,	  PREC_NONE},
    [TOKEN_FALSE]         = {literal,	NULL,	  PREC_NONE},
    [TOKEN_FOR]           = {NULL,	    NULL,	  PREC_NONE},
    [TOKEN_FUN]           = {NULL,	    NULL,	  PREC_NONE},
    [TOKEN_IF]            = {NULL,	    NULL,	  PREC_NONE},
    [TOKEN_NIL]           = {literal,  	NULL,	  PREC_NONE},
    [TOKEN_OR]            = {NULL,	    NULL,	  PREC_NONE},
    [TOKEN_PRINT]         = {NULL,	    NULL,	  PREC_NONE},
    [TOKEN_RETURN]        = {NULL,	    NULL,	  PREC_NONE},
    [TOKEN_SUPER]         = {NULL,	    NULL,	  PREC_NONE},
    [TOKEN_THIS]          = {NULL,	    NULL,	  PREC_NONE},
    [TOKEN_TRUE]          = {literal,  	NULL,	  PREC_NONE},
    [TOKEN_VAR]           = {NULL,	    NULL,	  PREC_NONE},
    [TOKEN_WHILE]         = {NULL,	    NULL,	  PREC_NONE},
    [TOKEN_ERROR]         = {NULL,	    NULL,	  PREC_NONE},
    [TOKEN_EOF]           = {NULL,	    NULL,	  PREC_NONE},
};

// Get rule from parsing rules table
static ParseRule* getRule(TokenType type) {
	return &rules[type];
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

	// Parse and compile declarations/statements until end of file
	while (!match(TOKEN_EOF)) {
		declaration();
	}

	// Look for end of file
	consume(TOKEN_EOF, "Expect end of expression.");

	// End program
	endCompiler();

	// Return success or failure
	return !parser.hadError;
}
