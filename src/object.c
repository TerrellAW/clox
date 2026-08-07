/**
 * Heap allocated data object implementations
 */

#include <string.h>

#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"
#include "vm.h"

// Macro to allocate an object on the heap
#define ALLOCATE_OBJ(type, objectType) (type*)allocateObject(sizeof(type), objectType)

// Allocate an object of the given size on the heap
static Obj* allocateObject(size_t size, ObjType type) {
	Obj* object	 = (Obj*)reallocate(NULL, 0, size);
	object->type = type;
	vm.objects 	 = object;
	return object;
}

// Create a new string object on the heap
static ObjString* allocateString(char* chars, size_t length, uint32_t hash) {
	// Create string object on the heap
	ObjString* string = ALLOCATE_OBJ(ObjString, OBJ_STRING);

	// Initialize fields
	string->length 	= length;
	string->chars	= chars;
	string->hash	= hash;

	// Intern string
	tableSet(&vm.strings, string, NIL_VAL);

	// Return newly allocated lox string
	return string;
}

// Calculate a hash code for the given string
static uint32_t hashString(const char* key, size_t length) {
	// Start with unsigned integer
	uint32_t hash = 2166136261u;

	// Convert each character into an integer and hash it
	for (int i = 0; i < length; i++) {
		hash ^= (uint8_t)key[i];
		hash *= 16777619;
	}

	return hash;
}

// Take string from char array
ObjString* takeString(char* chars, size_t length) {
	// Calculate hash code for string
	uint32_t hash = hashString(chars, length);

	// Check if string is interned
	ObjString* interned = tableFindString(&vm.strings, chars, length, hash);

	// If it is interned, return the interned string
	if (interned != NULL) {
		FREE_ARRAY(char, chars, length + 1);
		return interned;
	}

	return allocateString(chars, length, hash);
}

// Copy lexeme into string
ObjString* copyString(const char* chars, size_t length) {
	// Calculate hash code for string
	uint32_t hash = hashString(chars, length);

	// Check if string is already interned and return interned string instead
	ObjString* interned = tableFindString(&vm.strings, chars, length, hash);
	if (interned != NULL) return interned;

	// Allocate array of chars on the heap
	char* heapChars = ALLOCATE(char, length + 1);

	// Copy characters to array
	memcpy(heapChars, chars, length);

	// Add null terminator to end
	heapChars[length] = '\0';

	// Return heap allocated Lox string
	return allocateString(heapChars, length, hash);
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

