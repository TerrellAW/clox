/**
 * Memory management implementations
 */

#include "memory.h"
#include "vm.h"

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

// Free an object from memory
static void freeObject(Obj* object) {
	// Free data associated with specific types of objects
	switch (object->type) {
		case OBJ_STRING:
			ObjString* string = (ObjString*)object;
			FREE_ARRAY(char, string->chars, string->length + 1);
			FREE(ObjString, object);
			break;
	}
}

// Free objects linked list
void freeObjects() {
	// Get pointer to vm's objects linked list
	Obj* object = vm.objects;

	// Iterate through linked list and free all objects
	while (object != NULL) {
		Obj* next = object->next;
		freeObject(object);
		object = next;
	}
}
