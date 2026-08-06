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

#endif
