/**
 * Data value handling
 */

#ifndef CLOX_VALUE_H
#define CLOX_VALUE_H

#include "common.h"

/**
 * Double-precision floating point value
 */
typedef double Value;

/**
 * Dynamic array of Values
 * Acts as a constant pool for complex data like strings
 */
typedef struct {
	size_t capacity;
	size_t count;
	Value* values;
} ValueArray;

/**
 * Value array initializer
 * Initializes an empty constant pool
 */
void initValueArray(ValueArray* array);

/**
 * Value array deallocator
 * Frees memory associated with a constant pool
 */
void freeValueArray(ValueArray* array);

/**
 * Append a value to the end of a constant pool
 */
void writeValueArray(ValueArray* array, Value value);

/**
 * Print value of a constant
 */
void printValue(Value value);

#endif
