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

	// Variable for storing passed tombstones
	Entry* tombstone = NULL;

	// Find empty bucket to put entry in with linear probing
	for (;;) {
		// Get entry at bucket index
		Entry* entry = &entries[index];

		// Check if bucket is empty or a tombstone
		if (entry->key == NULL) {
			// Empty bucket
			if (IS_NIL(entry->value)) {
				// Return previously passed tombstone or current empty bucket
				// If a tombstone was passed it will be prioritized
				// If no tombstone was found, the current empty bucket is used
				return tombstone != NULL ? tombstone : entry;
			// Tombstone
			} else {
				// Remember tombstone
				if (tombstone == NULL) tombstone = entry;
			}
		// Found key, works because of string interning
		} else if (entry->key == key) {
			return entry;
		}

		// Increment index, loop to beginning at the end
		index = (index + 1) % capacity;
	}
}

// Get value associated with key
bool tableGet(Table* table, ObjString* key, Value* value) {
	// Check if table is empty
	if (table->count == 0) return false;

	// Find the bucket associated with the given key
	Entry* entry = findEntry(table->entries, table->capacity, key);

	// Check if bucket is empty
	if (entry->key == NULL) return false;

	// Send value to caller
	*value = entry->value;
	return true;
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

	// Clear out table count
	table->count = 0;

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

		// Get new count without tombstones
		table->count++;
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

	// Check whether a bucket is empty or a tombstone
	bool isNewKey = entry->key == NULL;

	// Increment if its a new non-tombstone bucket
	if (isNewKey && IS_NIL(entry->value)) table->count++;

	// Add key-value pair to table
	entry->key	 = key;
	entry->value = value;
	return isNewKey;
}

// Delete an entry by marking it as a tombstone
bool tableDelete(Table* table, ObjString* key) {
	// Check if table is empty
	if (table->count == 0) return false;

	// Find the entry
	Entry* entry = findEntry(table->entries, table->capacity, key);

	// Return false if entry not found
	if (entry->key == NULL) return false;

	// Place a tombstone at the entry
	entry->key	 = NULL;
	entry->value = BOOL_VAL(true);

	// Successful deletion
	return true;
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

// Find a string in the interned string table to avoid adding duplicates
ObjString* tableFindString(Table* table, const char* chars, size_t length, uint32_t hash) {
	// Check if table is empty
	if (table->count == 0) return NULL;

	// Get index
	uint32_t index = hash % table->capacity;

	// Iterate through entries to find matching string
	for (;;) {
		// Get entry
		Entry* entry = &table->entries[index];

		// Stop if empty bucket is found
		if (entry->key == NULL) {
			if (IS_NIL(entry->value)) return NULL;
		// Matching string found
		} else if (entry->key->length == length &&
				   entry->key->hash == hash		&&
				   memcmp(entry->key->chars, chars, length) == 0) {
			return entry->key;
		}

		// Increment index
		index = (index + 1) % table->capacity;
	}
}
