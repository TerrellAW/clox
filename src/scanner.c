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

// Check if character is part of a name
static bool isAlpha(char c) {
			// Handle lower case
	return 	(c >= 'a' && c <= 'z') ||
			// Handle upper case
			(c >= 'A' && c <= 'Z') ||
			// Handle underscore
			c == '_';
}

// Check if character is a digit
static bool isDigit(char c) {
	return c >= '0' && c <= '9';
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

// Peek at the current unconsumed token
static char peek() {
	return *scanner.current;
}

// Peek at next token after current uncomsumed one
static char peekNext() {
	if (isAtEnd()) return '\0';
	return scanner.current[1];
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
			// Skip comments
			case '/':
				if (peekNext() == '/') {
					// Ignore entire line
					while (peek() != '\n' && !isAtEnd()) advance();
				} else if (peekNext() == '*') {
					// Ignore until end of multi-line comment
					while (!(peek() == '*' && peekNext() == '/') && !isAtEnd()) {
						if (peek() == '\n') scanner.line++;
						advance();
					}
					// Consume end of comment
					advance();
					advance();
				} else {
					return;
				}
				break;
			default:
				return;
		}
	}
}

// Check for keyword
static TokenType checkKeyword(size_t start, size_t length,
		const char* rest, TokenType type) {
	// Compare lexeme to keyword to see if it matches exactly
	if (scanner.current - scanner.start == start + length &&
			memcmp(scanner.start + start, rest, length) == 0) {
		return type;
	}

	return TOKEN_IDENT;
}

// Associate an identifier token with its type
static TokenType identifierType() {
	// Keyword state machine
	switch (scanner.start[0]) {
		case 'a': return checkKeyword(1, 2, "nd", TOKEN_AND);
		case 'c': return checkKeyword(1, 4, "lass", TOKEN_CLASS);
		case 'e': return checkKeyword(1, 3, "lse", TOKEN_ELSE);
		case 'i': return checkKeyword(1, 1, "f", TOKEN_IF);
		case 'n': return checkKeyword(1, 2, "il", TOKEN_NIL);
		case 'o': return checkKeyword(1, 1, "r", TOKEN_OR);
		case 'p': return checkKeyword(1, 4, "rint", TOKEN_PRINT);
		case 'r': return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
		case 's': return checkKeyword(1, 4, "uper", TOKEN_SUPER);
		case 'v': return checkKeyword(1, 2, "ar", TOKEN_VAR);
		case 'w': return checkKeyword(1, 4, "hile", TOKEN_WHILE);
		// Starting letters with multiple keywords
		case 'f':
			if (scanner.current - scanner.start > 1) {
				// Branch into potential keywords
				switch (scanner.start[1]) {
					case 'a': return checkKeyword(2, 3, "lse", TOKEN_FALSE);
					case 'o': return checkKeyword(2, 1, "r", TOKEN_FOR);
					case 'u': return checkKeyword(2, 1, "n", TOKEN_FUN);
				}
			}
			break;
		case 't':
			if (scanner.current - scanner.start > 1) {
				switch (scanner.start[1]) {
					case 'h': return checkKeyword(2, 2, "is", TOKEN_THIS);
					case 'r': return checkKeyword(2, 2, "ue", TOKEN_TRUE);
				}
			}
			break;
	}

	// If not keyword, return identifier
	return TOKEN_IDENT;
}

// Handle identifiers
static Token identifier() {
	// Allow digits in name
	while (isAlpha(peek()) || isDigit(peek())) advance();
	// Determine type of identifier
	return makeToken(identifierType());
}

// Handle numbers
static Token number() {
	// Advance through digits
	while (isDigit(peek())) advance();

	// Handle decimal point
	if (peek() == '.' && isDigit(peekNext())) {
		// Consume dot
		advance();

		// Advance through digits after point
		while (isDigit(peek())) advance();
	}

	// Return number token
	return makeToken(TOKEN_NUMBER);
}

// Handle strings
static Token string() {
	while (peek() != '"' && !isAtEnd()) {
		// Supports multi-line strings
		if (peek() == '\n') scanner.line++;
		advance();
	}

	if (isAtEnd()) return errorToken("Unterminated string.");

	// Consume closing quote
	advance();

	// Return string token
	return makeToken(TOKEN_STRING);
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

	// Handle identifiers
	if (isAlpha(c)) return identifier();

	// Handle numbers
	if (isDigit(c)) return number();

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
		// Handle string values
		case '"': return string();
	}

	// Handle error
	return errorToken("Unexpected character.");
}
