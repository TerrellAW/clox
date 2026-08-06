/**
 * Hash table implementations
 */

#include <string.h>

#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"

// Max load constant
#define TABLE_MAX_LOAD 0.75

// Initialize an empty hash table
void initTable(Table* table) {
	table->count	= 0;
	table->capacity	= 0;
	table->entries	= NULL;
}

// Free hash table memory
void freeTable(Table* table) {
	FREE_ARRAY(Entry, table->entries, table->capacity);
	initTable(table);
}

// Find bucket associated with a key's hash code
// Return that bucket or the next empty one
static Entry* findEntry(Entry* entries, size_t capacity, ObjString* key) {
	// Get bucket index
	uint32_t index = key->hash % capacity;

	// Find empty bucket to put entry in with linear probing
	for (;;) {
		// Get entry at bucket index
		Entry* entry = &entries[index];

		// Return entry if it has the same key or is empty
		if (entry->key == key || entry->key == NULL) {
			return entry;
		}

		// Increment index, loop to beginning at the end
		index = (index + 1) % capacity;
	}
}

// Allocate array of buckets for hash table
static void adjustCapacity(Table* table, size_t capacity) {
	// Allocate entries array
	Entry* entries = ALLOCATE(Entry, capacity);

	// Zero initialize each bucket
	for (size_t i = 0; i < capacity; i++) {
		entries[i].key	 = NULL;
		entries[i].value = NIL_VAL;
	}

	// Initialize table
	table->entries	= entries;
	table->capacity	= capacity;
}

// Insert an entry into a hash table bucket
bool tableSet(Table* table, ObjString* key, Value value) {
	// Allocate table space for entry
	if (table->count + 1 > table->capacity * TABLE_MAX_LOAD) {
		// Grow table capacity
		size_t capacity = GROW_CAPACITY(table->capacity);

		// Allocate memory for buckets
		adjustCapacity(table, capacity);
	}

	// Find a bucket to put the entry in
	Entry* entry = findEntry(table->entries, table->capacity, key);

	// Check whether a key is new or being updated
	bool isNewKey = entry->key == NULL;
	if (isNewKey) table->count++;

	// Add key-value pair to table
	entry->key	 = key;
	entry->value = value;
	return isNewKey;
}
