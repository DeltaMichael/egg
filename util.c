#include "util.h"
#include "sbuilder.h"
#include "arglist.h"
#include <stdlib.h>
#include <stdio.h>

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
	char* raw_args = sb_get_string(builder);
	cmd.args = parse_command_args(cmd.command, raw_args);
	sb_free(builder);
	free(raw_args);
	return cmd;
}

char* parse_command_pipe(char* input, COMMAND* commands) {
	COMMAND cmd;
	SBUILDER* builder = sb_init();

	// get the command
	while(*input && *input != ' ' && *input != '\n' && *input != '\r') {
		sb_append_char(builder, *input);
		input++;
	}
	cmd.command = sb_get_string(builder);
	input++;

	// get the args
	while(*input && *input != '\n' && *input != '\r' && *input != '|') {
		sb_append_char(builder, *input);
		input++;
	}
	char* raw_args = sb_get_string(builder);
	cmd.args = parse_command_args(cmd.command, raw_args);
	sb_free(builder);
	free(raw_args);
	*commands = cmd;

	if(*input == '|') {
		input++;
		return input;
	}
	return NULL;
}

void parse_commands(char* input, COMMAND* commands) {
	char* raw_input = input;
	COMMAND* local_cmd = commands;
	while(raw_input != NULL) {
		raw_input = parse_command_pipe(raw_input, local_cmd);
		local_cmd += 1;
	}
}

char** parse_command_args(char* command, char* input) {
	SBUILDER* builder = sb_init();
	ARGLIST* arglist = arglist_init();
	arglist_append(arglist, command);
	while(*input) {
		while(*input && (*input == ' ' || *input == '\n' || *input == '\r')) {
			input++;
		}
		if(*input == '"') {
			input++;
			while(*input && *input != '"') {
				sb_append_char(builder, *input);
				input++;
			}
			input++;
		} else {
			while(*input && (*input != ' ' && *input != '\n' && *input != '\r')) {
				sb_append_char(builder, *input);
				input++;
			}
		}
		arglist_append(arglist, sb_get_string(builder));
	}
	char** out = arglist_get_args(arglist);
	sb_free(builder);
	arglist_free(arglist);
	return out;
}

void free_command(COMMAND cmd) {
	if(cmd.command != NULL) {
		free(cmd.command);
		cmd.command = NULL;
	}
	if(cmd.args != NULL) {
		char** args = cmd.args;
		while(*args != NULL) {
			free(*args);
			args++;
		}
		free(cmd.args);
		cmd.args = NULL;
	}
}

