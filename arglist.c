#include "arglist.h"
#include <stdlib.h>
#include <string.h>


ARGLIST* arglist_init() {
	ARGLIST* list = malloc(sizeof(ARGLIST));
	list->pointer = 0;
	list->size = 128;
	list->items = calloc(list->size, sizeof(char*));
	return list;
}

void arglist_append(ARGLIST* list, char* argument) {
	if (list->pointer >= list->size) {
		// resize
		list->size *= 2;
		list->items = realloc(list->items, list->size * sizeof(char*));
	}
	list->items[list->pointer] = strdup(argument);
	list->pointer++;
}

char** arglist_get_args(ARGLIST* list) {
	char** out = calloc(list->pointer + 1, sizeof(char*));
	for(int i = 0; i < list->pointer; i++) {
		out[i] = strdup(list->items[i]);
	}

	list->pointer = 0;
	// arglist_reset(list);
	return out;
}

void arglist_reset(ARGLIST* list) {
	for(int i = 0; i < list->pointer; i++) {
		if(list->items[i] != NULL) {
			free(list->items[i]);
		}
	}
	list->pointer = 0;
	list->size = 128;
	free(list->items);
	list->items = calloc(list->size, sizeof(char*));
}

void arglist_free(ARGLIST* list) {
	if(list == NULL) {
		return;
	}
	for(int i = 0; i < list->size; i++) {
		if(list->items[i] != NULL) {
			free(list->items[i]);
		}
	}
	free(list->items);
	list->items = NULL;
	free(list);
}

