#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "util.h"
#include <dirent.h>

char* get_current_dir();
int filter_executable(const struct dirent* d);
char* find_executable(char* name);
char* get_file_type(struct dirent *c_entry);
void list_dir_contents(char* path);
void execute_bin(char* name, char** args);
void pipe_commands(COMMAND** commands, int size);
DIR* open_dir(char* current_dir);
void change_dir(char* new_dir);
#endif

