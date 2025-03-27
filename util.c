#include "util.h"
#include "sbuilder.h"
#include "arglist.h"
#include <stdlib.h>
#include <stdio.h>


CMD* cmd_init(char* command) {
	CMD* out = malloc(sizeof(CMD));
	out->command = strdup(command);
	out->args = arglist_init();
	arglist_append(out->args, out->command);
	return out;
}

void cmd_append(CMD* cmd, char* arg) {
	arglist_append(cmd->args, arg);
}

char** cmd_get(CMD* cmd) {
	return arglist_get_args(cmd->args);
}

void cmd_free(CMD* cmd) {
	if(cmd == NULL) {
		return;
	}
	arglist_free(cmd->args);
	if(cmd->command != NULL) {
		free(cmd->command);
	}
	free(cmd);
}

bool streq(char* str1, char* str2) {
	return strcmp(str1, str2) == 0;
}

