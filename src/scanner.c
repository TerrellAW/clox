/**
 * Source code scanner implementations
 */

#include <string.h>

#include "../include/scanner.h"

// Scanner data structure
typedef struct {
	const char* start;
	const char* current;
	size_t line;
} Scanner;

// Unitialized scanner instance
Scanner scanner;

// Initialize scanner data structure
void initScanner(const char* source) {
	// All pointers and counters to beginning of file
	scanner.start = source;
	scanner.current = source;
	scanner.line = 1;
}

// Check if at end of file
static bool isAtEnd() {
	return *scanner.current == '\0';
}

// Create a token
static Token makeToken(TokenType type) {
	// Create token from current lexeme
	Token token;
	token.type 	 = type;
	token.start  = scanner.start;
	token.length = (size_t)(scanner.current - scanner.start);
	token.line 	 = scanner.line;
	return token;
}

// Create error token for passing to compiler
static Token errorToken(const char* message) {
	// Create token with error message
	Token token;
	token.type 	 = TOKEN_ERROR;
	token.start  = message;
	token.length = (size_t)strlen(message);
	token.line 	 = scanner.line;
	return token;
}

// Return current and advance
static char advance() {
	scanner.current++;
	return scanner.current[-1];
}

// Peek at the current un-consumed token
static char peek() {
	return *scanner.current;
}

// Ensure whitespace is ignored
static void skipWhitespace() {
	for (;;) {
		// Peek at current token
		char c = peek();

		// Advance past any whitespace
		switch (c) {
			case ' ':
			case '\r':
			case '\t':
				advance();
				break;
			// Handle new line
			case '\n':
				scanner.line++;
				advance();
				break;
			default:
				return;
		}
	}
}

// Consume token if it matches the argument
static bool match(char expected) {
	if (isAtEnd()) return false;
	if (*scanner.current != expected) return false;
	scanner.current++;
	return true;
}

// Tokenize a lexeme
Token scanToken() {
	// Ignore whitespace
	skipWhitespace();

	// Increment start
	scanner.start = scanner.current;

	// Handle end of file
	if (isAtEnd()) return makeToken(TOKEN_EOF);

	// Store current and advance one character
	char c = advance();

	// Tokenize
	switch (c) {
		// Handle single-character tokens
		case '(': return makeToken(TOKEN_LEFT_PAREN);
		case ')': return makeToken(TOKEN_RIGHT_PAREN);
		case '{': return makeToken(TOKEN_LEFT_BRACE);
		case '}': return makeToken(TOKEN_RIGHT_BRACE);
		case ';': return makeToken(TOKEN_SEMICOLON);
		case ',': return makeToken(TOKEN_COMMA);
		case '.': return makeToken(TOKEN_DOT);
		case '-': return makeToken(TOKEN_MINUS);
		case '+': return makeToken(TOKEN_PLUS);
		case '/': return makeToken(TOKEN_SLASH);
		case '*': return makeToken(TOKEN_STAR);
		// Handle one or two character tokens
		case '!':
			return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
		case '=':
			return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
		case '<':
			return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
		case '>':
			return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
	}

	// Handle error
	return errorToken("Unexpected character.");
}
