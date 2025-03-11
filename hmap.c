#include "hmap.h"
#include "util.h"

#include <stdlib.h>

HMAP* hmap_init() {
	HMAP *out = malloc(sizeof(HMAP));
	out->inserted = 0;
	out->size = 1000;
	out->entries = calloc(out->size, sizeof(ENTRY));
	return out;
}

ENTRY* entry_init(char* key, char* value) {
	ENTRY* entry = malloc(sizeof(ENTRY));
	entry->key = key;
	entry->value = value;
	return entry;
}

void hmap_insert(HMAP* hmap, char* key, char* value) {
	// TODO: check ratio and resize if necessary, e.g. hmap->inserted / hmap->size > 0.5
	size_t index = hmap_get_index(hmap, key);
	while(index < hmap->size && hmap->entries[index] != NULL) {
		index++;
	}
	if (index >= hmap->size) {
		// TODO: resize and try again
	} else {
		hmap->entries[index] = entry_init(key, value);
	}
}

void hmap_delete(HMAP* hmap, char* key) {
	size_t index = hmap_get_index(hmap, key);
	ENTRY* entry;
	while(index < hmap->size && !streq(entry->key, key)) {
		entry = hmap->entries[index];
		index++;
	}
	if(index < hmap->size) {
		entry->key = NULL;
		entry->value = NULL;
		hmap->entries[index] = NULL;
		free(entry);
	}
}

char* hmap_get(HMAP* hmap, char* key) {
	size_t index = hmap_get_index(hmap, key);
	ENTRY* entry;
	while(index < hmap->size && !streq(entry->key, key)) {
		entry = hmap->entries[index];
		index++;
	}
	if(index >= hmap->size) {
		return NULL;
	}
	return entry->value;
}

bool hmap_contains(HMAP* hmap, char* key) {
	return hmap_get(hmap, key) != NULL;
}

int hash(char* key) {
	int out = 0;
	while(key) {
		out += *key;
		key++;
	}
	return out;
}

size_t hmap_get_index(HMAP* hmap, char* key) {
	return hash(key) % hmap->size;
}

void hmap_resize(HMAP* hmap) {
	// TODO
	// 1. Create new array
	// 2. Recompute hash keys
	// 3. Copy values
	// 4. Delete old array
}

