/**
 * Source code scanner implementations
 */

#include <stdio.h>
#include <string.h>

#include "../include/common.h"
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
