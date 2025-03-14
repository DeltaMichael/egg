#ifndef ARGLIST_H
#define ARGLIST_H

#include "stdint.h"
#include "stddef.h"

typedef struct {
	char** items;
	size_t pointer;
	size_t size;
} ARGLIST;

ARGLIST* arglist_init();
void arglist_append(ARGLIST* list, char* argument);
char** arglist_get_args(ARGLIST* list);
void arglist_resize(ARGLIST* list);
void arglist_reset(ARGLIST* list);
void arglist_free(ARGLIST* list);

#endif

