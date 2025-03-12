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
	while(*input && *input != '\n' && *input != '\r') {
		sb_append_char(builder, *input);
		input++;
	}
	cmd.args = sb_get_string(builder);
	sb_free(builder);
	return cmd;
}

void free_command(COMMAND cmd) {
	if(cmd.command != NULL) {
		free(cmd.command);
		cmd.command = NULL;
	}
	if(cmd.args != NULL) {
		free(cmd.args);
		cmd.args = NULL;
	}
}

