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

	// Re-insert entries from old table into new larger table
	for (size_t i = 0; i < table->capacity; i++) {
		// Get entry from non-empty bucket
		Entry* entry = &table->entries[i];
		if (entry->key == NULL) continue;

		// Find destination bucket
		Entry* dest = findEntry(entries, capacity, entry->key);

		// Insert into destination bucket
		dest->key	= entry->key;
		dest->value = entry->value;
	}

	// Free memory from old array
	FREE_ARRAY(Entry, table->entries, table->capacity);

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

// Add all entries from one hash table to another
void tableAddAll(Table* from, Table* to) {
	// Iterate through entries in source table
	for (size_t i = 0; i < from->capacity; i++) {
		// Get bucket from source table
		Entry* entry = &from->entries[i];

		// If not empty, add its entry to the destination table
		if (entry->key != NULL) {
			tableSet(to, entry->key, entry->value);
		}
	}
}
