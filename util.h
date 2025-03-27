#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <string.h>
#include "arglist.h"

typedef struct {
	char* command;
	char** args;
} COMMAND;

typedef struct {
	ARGLIST* args;
} CMD;

CMD* cmd_init();
void cmd_append(CMD* cmd, char* arg);
char** cmd_get(CMD* cmd);

bool streq(char* str1, char* str2);
COMMAND* parse_command(char* input);
char** parse_command_args(char* command, char* input);
char* parse_command_pipe(char* input, COMMAND** commands);
int parse_commands(char* input, COMMAND** commands);
void free_command(COMMAND* cmd);

#endif

