/**
 * Memory management implementations
 */

#include "../include/memory.h"

// Reallocate memory for all data structures
void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
	// Free allocated memory
	if (newSize == 0) {
		free(pointer);
		return NULL;
	}

	// Change allocated memory
	void* result = realloc(pointer, newSize);

	// Handle failed reallocation
	if (result == NULL) exit(EXIT_FAILURE);

	return result;
}
