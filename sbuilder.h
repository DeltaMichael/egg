#ifndef SBUILDER_H
#define SBUILDER_H
#include <stddef.h>

typedef struct {
	char* characters;
	size_t pointer;
	size_t size;
} SBUILDER;

SBUILDER* sb_init();
void sb_append(SBUILDER* builder, char* input);
void sb_append_char(SBUILDER* builder, char input);
char* sb_get_string(SBUILDER* builder);
void sb_reset(SBUILDER* builder);
void sb_double_size(SBUILDER* builder);
void sb_shrink_to_size(SBUILDER* builder);
void sb_free(SBUILDER* builder);
#endif

