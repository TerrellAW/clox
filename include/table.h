/**
 * Hash Table
 */

#ifndef CLOX_TABLE_H
#define CLOX_TABLE_H

#include "common.h"
#include "value.h"

/**
 * Hash table entry
 */
typedef struct {
	ObjString* key;
	Value value;
} Entry;

/**
 * Hash table structure
 */
typedef struct {
	size_t count;
	size_t capacity;
	Entry* entries;
} Table;

/**
 * Hash table initializer
 */
void initTable(Table* table);

/**
 * Free hash table from memory
 */
void freeTable(Table* table);

/**
 * Get value associated with the given key
 * Return true if the key's bucket is not empty
 */
bool tableGet(Table* table, ObjString* key, Value* value);

/**
 * Add the given key-value pair to the given hash table
 * Returns true if a new entry was added, else false
 */
bool tableSet(Table* table, ObjString* key, Value value);

/**
 * Remove an entry from a hash table
 * Returns true if successful
 */
bool tableDelete(Table* table, ObjString* key);

/**
 * Copy all entries from one hash table to another
 */
void tableAddAll(Table* from, Table* to);

/**
 * Find given string in table
 */
ObjString* tableFindString(Table* table, const char* chars, size_t length, uint32_t hash);

#endif
