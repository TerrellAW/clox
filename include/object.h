/**
 * Heap allocated data object
 */

#ifndef CLOX_OBJECT_H
#define CLOX_OBJECT_H

#include "common.h"
#include "value.h"

/**
 * Macro to extract an object type from a Value
 */
#define OBJ_TYPE(value)		(AS_OBJ(value)->type)

/**
 * Macro to check if object is a string
 */
#define IS_STRING(value)	isObjType(value, OBJ_STRING)

/**
 * Macro to return ObjString pointer from Value
 */
#define AS_STRING(value)	((ObjString*)AS_OBJ(value))

/**
 * Macro to return a character array from a Value
 */
#define AS_CSTRING(value)	(((ObjString*)AS_OBJ(value))->chars)

/**
 * Valid heap allocated data types
 */
typedef enum {
	OBJ_STRING,
} ObjType;

/**
 * Heap allocated object struct
 */
struct Obj {
	ObjType type;
};

/**
 * String type heap allocated data object
 */
struct ObjString {
	Obj obj;
	size_t length;
	char* chars;
};

/**
 * Produce a string object from a char array
 * Claims ownership of the already allocated string
 */
ObjString* takeString(char* chars, size_t length);

/**
 * Copy a string's characters directly from lexeme into a Lox string
 */
ObjString* copyString(const char* chars, size_t length);

/**
 * Print a heap allocated object's value
 */
void printObject(Value value);

// Check data type of an object
static inline bool isObjType(Value value, ObjType type) {
	return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif
