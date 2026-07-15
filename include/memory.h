/**
 * Memory management operations
 */

#ifndef CLOX_MEMORY_H
#define CLOX_MEMORY_H

#include "common.h"

/**
 * Macro to scale capacity based on a previous capacity
 */
#define GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)

/**
 * Macro to grow a dynamic array to support a higher capacity
 */
#define GROW_ARRAY(type, pointer, oldCount, newCount) \
	(type*)reallocate(pointer, sizeof(type) * (oldCount), \
	sizeof(type) * (newCount))

/**
 * Macro to free memory from a dynamic array
 */
#define FREE_ARRAY(type, pointer, oldCount) \
	reallocate(pointer, sizeof(type) * (oldCount), 0)

/**
 * Function to reallocate memory for a data structure
 */
void* reallocate(void* pointer, size_t oldSize, size_t newSize);

#endif
