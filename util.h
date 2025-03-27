#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <string.h>
#include "arglist.h"

typedef struct {
	char* command;
	ARGLIST* args;
} CMD;

CMD* cmd_init();
void cmd_append(CMD* cmd, char* arg);
char** cmd_get(CMD* cmd);
void cmd_free(CMD* cmd);

bool streq(char* str1, char* str2);

#endif

