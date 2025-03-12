#include "sbuilder.h"
#include <stdlib.h>
#include <stdio.h>

SBUILDER* sb_init() {
	SBUILDER* builder = malloc(sizeof(SBUILDER));
	builder->pointer = 0;
	builder->size = 128;
	builder->characters = malloc(sizeof(char) * builder->size);
	return builder;
}

void sb_append(SBUILDER* builder, char* input) {
	while(*input != 0) {
		sb_append_char(builder, *input);
		input++;
	}
}

void sb_append_char(SBUILDER* builder, char input) {
	if (builder->pointer >= builder->size) {
		sb_double_size(builder);
	}
	builder->characters[builder->pointer] = input;
	builder->pointer++;
}

char* sb_get_string(SBUILDER* builder) {
	sb_append_char(builder, 0);
	sb_shrink_to_size(builder);
	char* out = builder->characters;
	sb_reset(builder);
	return out;
}

void sb_reset(SBUILDER* builder) {
	builder->pointer = 0;
	builder->size = 128;
	builder->characters = malloc(builder->size);
}

void sb_double_size(SBUILDER* builder) {
	builder->size *= 2;
	realloc(builder->characters, builder->size);
}

void sb_shrink_to_size(SBUILDER* builder) {
	builder->size = builder->pointer + 1;
	realloc(builder->characters, builder->size);
}

void sb_free(SBUILDER* builder) {
	free(builder->characters);
	builder->characters = NULL;
	free(builder);
}

