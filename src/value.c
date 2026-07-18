/**
 * Data value implementations
 */

#include "../include/memory.h"
#include "../include/value.h"

// Value array initializer
void initValueArray(ValueArray* array) {
	array->values = NULL;
	array->capacity = 0;
	array->count = 0;
}

// Free value array memory
void freeValueArray(ValueArray* array) {
	// Free memory
	FREE_ARRAY(Value, array->values, array->capacity);

	// Re-initialize array
	initValueArray(array);
}

// Append a value to the end of a constant pool
void writeValueArray(ValueArray* array, Value value) {
	// Check if capacity has been reached
	if (array->capacity < array->count + 1) {
		size_t oldCapacity = array->capacity;
		// Grow capacity dynamically
		array->capacity = GROW_CAPACITY(oldCapacity);
		array->values = GROW_ARRAY(Value, array->values, oldCapacity, array->capacity);
	}

	// Append value to the end
	array->values[array->count] = value;
	array->count++;
}

// Print value of a constant
void printValue(Value value) {
	printf("%g", value);
}

