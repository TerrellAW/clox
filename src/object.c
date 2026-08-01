/**
 * Heap allocated data object implementations
 */

#include <string.h>

#include "memory.h"
#include "object.h"
#include "value.h"
#include "vm.h"

// Macro to allocate an object on the heap
#define ALLOCATE_OBJ(type, objectType) (type*)allocateObject(sizeof(type), objectType)

// Allocate an object of the given size on the heap
static Obj* allocateObject(size_t size, ObjType type) {
	Obj* object	 = (Obj*)reallocate(NULL, 0, size);
	object->type = type;
	return object;
}

// Create a new string object on the heap
static ObjString* allocateString(char* chars, size_t length) {
	// Create string object on the heap
	ObjString* string = ALLOCATE_OBJ(ObjString, OBJ_STRING);

	// Initialize fields
	string->length 	= length;
	string->chars	= chars;

	// Return newly allocated lox string
	return string;
}

// Take string from char array
ObjString* takeString(char* chars, size_t length) {
	return allocateString(chars, length);
}

// Copy lexeme into string
ObjString* copyString(const char* chars, size_t length) {
	// Allocate array of chars on the heap
	char* heapChars = ALLOCATE(char, length + 1);

	// Copy characters to array
	memcpy(heapChars, chars, length);

	// Add null terminator to end
	heapChars[length] = '\0';

	// Return heap allocated Lox string
	return allocateString(heapChars, length);
}

// Print object's value
void printObject(Value value) {
	// Handle different types of objects
	switch (OBJ_TYPE(value)) {
		case OBJ_STRING:
			// Print as a C string
			printf("%s", AS_CSTRING(value));
			break;
	}
}

