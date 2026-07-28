/**
 * Data value handling
 */

#ifndef CLOX_VALUE_H
#define CLOX_VALUE_H

#include "common.h"

/**
 * Enum of valid data types
 */
typedef enum {
	VAL_BOOL,
	VAL_NIL,
	VAL_NUMBER,
} ValueType;

/**
 * Union of data types for values
 */
typedef struct {
	ValueType type;
	union {
		bool boolean;
		double number;
	} as;
} Value;

/**
 * Type checking macros
 */
#define IS_BOOL(value)		((value).type == VAL_BOOL)
#define IS_NIL(value)		((value).type == VAL_NIL)
#define IS_NUMBER(value)	((value).type == VAL_NUMBER)

/**
 * Value data type unwrapping macros
 */
#define AS_BOOL(value)		((value).as.boolean)
#define AS_NUMBER(value)	((value).as.number)

/**
 * Value data type casting macros
 */
#define BOOL_VAL(value)		((Value){VAL_BOOL, {.boolean = value}})
#define NIL_VAL				((Value){VAL_NIL, {.number = 0}})
#define NUMBER_VAL(value)	((Value){VAL_NUMBER, {.number = value}})

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
 * Finds whether two values are equal
 */
bool valuesEqual(Value a, Value b);

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
