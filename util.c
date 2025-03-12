#include "util.h"
#include "sbuilder.h"
#include <stdlib.h>

bool streq(char* str1, char* str2) {
	return strcmp(str1, str2) == 0;
}

COMMAND parse_command(char* input) {
	COMMAND cmd;
	SBUILDER* builder = sb_init();
	while(*input && *input != ' ' && *input != '\n' && *input != '\r') {
		sb_append_char(builder, *input);
		input++;
	}
	cmd.command = sb_get_string(builder);
	input++;
	sb_append(builder, input);
	cmd.args = sb_get_string(builder);
	sb_free(builder);
	return cmd;
}

void free_command(COMMAND cmd) {
	free(cmd.command);
	free(cmd.args);
}

