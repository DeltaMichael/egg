#include "arglist.h"
#include <stdlib.h>


ARGLIST* arglist_init() {
	ARGLIST* list = malloc(sizeof(ARGLIST));
	list->pointer = 0;
	list->size = 128;
	list->items = malloc(list->size * sizeof(char*));
	return list;
}

void arglist_append(ARGLIST* list, char* argument) {
	if (list->pointer >= list->size) {
		// resize
		list->size *= 2;
		list->items = realloc(list->items, list->size * sizeof(char*));
	}
	list->items[list->pointer] = argument;
	list->pointer++;
}

char** arglist_get_args(ARGLIST* list) {
	arglist_append(list, (char*)NULL);
	list->items = realloc(list->items, (list->pointer + 1) * sizeof(char*));
	char** out = list->items;
	arglist_reset(list);
	return out;
}

void arglist_reset(ARGLIST* list) {
	list->pointer = 0;
	list->size = 128;
	list->items = malloc(list->size * sizeof(char*));
}

void arglist_free(ARGLIST* list) {
	free(list->items);
	list->items = NULL;
	free(list);
}

