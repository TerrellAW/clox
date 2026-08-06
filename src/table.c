/**
 * Hash table implementations
 */

#include <string.h>

#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"

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
