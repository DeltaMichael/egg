#ifndef HMAP_H
#define HMAP_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
	char* key;
	char* value;
} ENTRY;

typedef struct {
	ENTRY** entries;
	size_t inserted;
	size_t size;
} HMAP;

HMAP* hmap_init();
ENTRY* entry_init(char* key, char* value);
void hmap_insert(HMAP* hmap, char* key, char* value);
void hmap_delete(HMAP* hmap, char* key);
char* hmap_get(HMAP* hmap, char* key);
bool hmap_contains(HMAP* hmap, char* key);
int hash(char* key);
size_t hmap_get_index(HMAP* hmap, char* key);
void hmap_resize(HMAP* hmap);

#endif

