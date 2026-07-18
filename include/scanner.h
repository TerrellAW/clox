/**
 * Source code scanner
 * Tokenizes source code
 */

#ifndef CLOX_SCANNER_H
#define CLOX_SCANNER_H

#include "common.h"

/**
 * Enum of valid token types
 */
typedef enum {
	// Single-character tokens
	TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
	TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
	TOKEN_COMMA, 	  TOKEN_DOT,
	TOKEN_PLUS,	 	  TOKEN_SLASH,
	TOKEN_SEMICOLON,  TOKEN_MINUS,
	TOKEN_STAR,
	// One or two character tokens
	TOKEN_BANG,		  TOKEN_BANG_EQUAL,
	TOKEN_EQUAL,	  TOKEN_EQUAL_EQUAL,
	TOKEN_GREATER,	  TOKEN_GREATER_EQUAL,
	TOKEN_LESS,		  TOKEN_LESS_EQUAL,
	// Literals
	TOKEN_IDENT, 	  TOKEN_STRING, TOKEN_NUMBER,
	// Keywords
	TOKEN_AND, 	 	  TOKEN_CLASS,  TOKEN_ELSE,
	TOKEN_FALSE, 	  TOKEN_FOR,    TOKEN_FUN,
	TOKEN_IF,	 	  TOKEN_NIL,	TOKEN_OR,
	TOKEN_PRINT, 	  TOKEN_RETURN, TOKEN_SUPER,
	TOKEN_THIS,  	  TOKEN_TRUE,   TOKEN_VAR,
	TOKEN_WHILE,
	// Special tokens
	TOKEN_ERROR, 	  TOKEN_EOF
} TokenType;

/**
 * Token data structure
 */
typedef struct {
	TokenType type;
	const char* start;
	size_t length;
	size_t line;
} Token;

/**
 * Initialize scanner
 */
void initScanner(const char* source);

/**
 * Tokenize a lexeme
 */
Token scanToken();

#endif
