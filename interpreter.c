#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include "util.h"
#include "hmap.h"
#include "sbuilder.h"

static char* exec_filter = NULL;

char* get_current_dir() {
	int current_dir_length = 128;
	char* current_dir = calloc(current_dir_length, sizeof(uint8_t));
	while (getcwd(current_dir, current_dir_length) == NULL) {
		if (errno == ERANGE) {
			current_dir_length *= 2;
			current_dir = realloc(current_dir, current_dir_length);
		} else {
			printf("Egg exited errno %d", errno);
			exit(1);
		}
	}
	return current_dir;
}

int filter_executable(const struct dirent* d) {
	return 0 == strcmp(exec_filter, d->d_name);
}

char* find_executable(char* name) {
    struct dirent **namelist;
    int n;
	exec_filter = name;
    n = scandir("/usr/bin", &namelist, filter_executable, alphasort);
    if (n == 0) {
        printf("%s: No such command\n", name);
		return "";
    } else if (n == -1) {
		// TODO: error handling
        printf("TODO: error handling\n");
		return "";
	}
	SBUILDER* builder = sb_init();
	sb_append(builder, "/usr/bin/");
	sb_append(builder, name);
	char* out = sb_get_string(builder);
	sb_free(builder);

	// Free the names
    while (n--) {
        free(namelist[n]);
    }
    free(namelist);
	return out;
}

void pipe_commands(CMD** commands) {

	// TODO: replace this with appropriate data structure
	// determine the size, it's null-terminated
	int size = 0;
	CMD** cmd_pointer = commands;
	while(*cmd_pointer != NULL) {
		size++;
		cmd_pointer++;
	}

	// check for built-ins
	for(int i = 0; i < size; i++) {
		// TODO: use hashmap here
		if(streq(commands[i]->command, "cd") || streq(commands[i]->command, "exit") || streq(commands[i]->command, "eggzit")) {
			if(size == 1) { // execute the built-in
				if(streq(commands[i]->command, "cd")) {
					char** args = cmd_get(commands[i]);
					if(args[1]) {
						change_dir(args[1]);
					}
					return;
				}

				if(streq(commands[i]->command, "exit") || streq(commands[i]->command, "eggzit")) {
					exit(0);
				}

			} else {
				// do nothing
				return;
			}
		}
	}

	int fds[2 * (size - 1)];
	for(int i = 0; i < size - 1; i++) {
		if (-1 == pipe(fds + 2 * i)) {
			switch(errno) {
				case EFAULT:
					printf("pipefd is not valid.");
					break;
				case EMFILE:
					printf("The per-process limit on the number of open file descriptors has been reached.");
					break;
				case ENFILE:
					printf("The system-wide limit on the total number of open files has been reached or the user hard limit on memory that can be allocated for pipes has been reached and the caller is not privileged; see pipe(7).");
					break;
				default:
					printf("Error occurred when calling pipe()");
					break;
			}
		}

	}

	for(int i = 0; i < size; i++) {
		char** args = cmd_get(commands[i]);
		char* executable = find_executable(args[0]);
		if(streq("", executable)) {
			continue;
		}
		// start the child process
    	pid_t pid = fork();
		if(0 == pid) {
			// if it's not the first command
			if(i > 0) {
				// child reads from the prev pipe read end
				dup2(fds[2*(i - 1)], STDIN_FILENO);
			}

			// if it's not the last command
			if(i < size - 1) {
				// child writes to the current pipe write end
				dup2(fds[2*i + 1], STDOUT_FILENO);
			}

			// close the pipe fds in the child
			// not closing these could lead to blocking,
			// the kernel will wait for reads, even though they've been redirected
			for(int j = 0; j < 2 * (size - 1); j++) {
				close(fds[j]);
			}
			execv(executable, args);
			exit(EXIT_SUCCESS);
		}
	}

	// close descriptors in the parent
	for(int j = 0; j < 2 * (size - 1); j++) {
		close(fds[j]);
	}

	// wait for changes in any of the children
	for(int i = 0; i < size; i++) {
		wait(NULL);
	}
}

void change_dir(char* new_dir) {
	if (-1 == chdir(new_dir)) {
		switch(errno) {
			case EACCES: // Search permission is denied for one of the components of path.  (See also path_resolution(7).)
				printf("Search permission is denied for one of the components of %s", new_dir);
				break;
			case EFAULT: // path points outside your accessible address space.
				printf("%s points outside your accessible address space", new_dir);
				break;
			case EIO: //    An I/O error occurred.
				printf("An I/O error occured");
				break;
			case ELOOP: //  Too many symbolic links were encountered in resolving path.
				printf("Too many symbolic links were encountered in resolving %s", new_dir);
				break;
			case ENAMETOOLONG: // path is too long.
				printf("Directory path is too long");
				break;
			case ENOENT: // The directory specified in path does not exist.
				printf("%s does not exist", new_dir);
				break;
			case ENOMEM: // Insufficient kernel memory was available.
				printf("Insufficient kernel memory was available");
				break;
			case ENOTDIR: // A component of path is not a directory.
				printf("A component of %s is not a directory", new_dir);
				break;
		}
	}
}

