#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
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

char* get_file_type(struct dirent *c_entry) {
	switch (c_entry->d_type) {
		case DT_BLK: //      This is a block device.
			return "<BLK>";
		case DT_CHR: //      This is a character device.
			return "<CHR>";
		case DT_DIR: //      This is a directory.
			return "<DIR>";
		case DT_FIFO: //     This is a named pipe (FIFO).
			return "<FIFO>";
		case DT_LNK: //      This is a symbolic link.
			return "LINK>";
		case DT_REG: //      This is a regular file.
			return "<FILE>";
		case DT_SOCK: //     This is a UNIX domain socket.
			return "<SOCK>";
		case DT_UNKNOWN: //  The file type could not be determined.
			return "<N/A>";
		default:
			return "<N/A>";
	}
}

void list_dir_contents(char* path) {
    struct dirent **namelist;
    int n = scandir(path, &namelist, NULL, alphasort);
    if (n == 0) {
		printf("\n");
		return;
    } else if (n == -1) {
		// TODO: error handling
        printf("TODO: error handling\n");
		return;
	}

    for (int i = 0; i < n; i++) {
		struct dirent *c_entry = namelist[i];
		printf("%s", get_file_type(c_entry));
		printf("\t%s\n", c_entry->d_name);
        free(c_entry);
    }
    free(namelist);
}

void execute_bin(char* name, char** args) {
	char* executable = find_executable(name);
	if(streq("", executable)) {
		return;
	}
    pid_t pid = fork();
    switch (pid) {
    	case -1:
    	    printf("Could not start %s\n", name);
			break;
    	case 0:
			execv(executable, args);
			exit(EXIT_SUCCESS);
		default:
			int status;
    		waitpid(pid, &status, 0);
			break;
	}
}

void execute_bin_io(char* name, char** args, FILE* in, FILE* out) {
	char* executable = find_executable(name);
	if(streq("", executable)) {
		return;
	}
    pid_t pid = fork();
    switch (pid) {
    	case -1:
    	    printf("Could not start %s\n", name);
			break;
    	case 0:
			// redirect stdin to in
			if(in) {
				dup2(in, STDIN_FILENO);
			}
			// redirect stdout and stderr to out
			if(out) {
				dup2(out, STDOUT_FILENO);
				dup2(out, STDERR_FILENO);
			}
			execv(executable, args);
			exit(EXIT_SUCCESS);
		default:
			int status;
    		waitpid(pid, &status, 0);
			break;
	}
}

void pipe_commands(COMMAND* commands, int size) {
	int fds[size + 1];
	for(int i = 0; i < size; i++) {
		pipe(fds[i]);
	}
	pclose(fds[0]);
	fds[0] = NULL;
	fds[size] = NULL;

	for(int i = 0; i < size; i++) {
		execute_bin_io(commands[i].command, commands[i].args, fds[i], fds[i + 1]);
	}

	for(int i = 1; i < size; i++) {
		pclose(fds[i]);
	}
}

DIR* open_dir(char* current_dir) {
	int descr = open(current_dir, O_DIRECTORY);
	DIR* cd = fdopendir(descr);
	if (cd == NULL) {
		switch (errno) {
			case EACCES: // Permission denied.
    				printf("Permission denied");
					break;
			case EBADF:  // fd is not a valid file descriptor opened for reading.
    				printf("%d for directory %s is not a valid file descriptor opened for reading", descr, current_dir);
					break;
			case EMFILE: // The per-process limit on the number of open file descriptors has been reached.
    				printf("The per-process limit on the number of open file descriptors has been reached.");
					break;
			case ENFILE: // The system-wide limit on the total number of open files has been reached.
    				printf("The system-wide limit on the total number of open files has been reached.");
					break;
			case ENOENT: // Directory does not exist, or name is an empty string.
    				printf("Directory does not exist, or name is an empty string.");
					break;
			case ENOMEM: // Insufficient memory to complete the operation.
    				printf("Insufficient memory to complete the operation.");
					break;
			case ENOTDIR:
    				printf("%s is not a directory", current_dir);
					break;
		}
	}
	return cd;
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


int main(int argc, char **argv, char** envp) {
	char* current_dir = get_current_dir();
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

	system("clear");
	while (1) {
		printf("(%s (EGG> ", current_dir);
		COMMAND commands[128] = {};
    	if ((nread = getline(&line, &len, stdin)) != -1) {
			parse_commands(line, commands);
			COMMAND cmd = commands[0];
			if (streq(cmd.command, "exit") || streq(cmd.command, "eggzit\n")) {
				break;
			} else if (streq(cmd.command, "dir")) {
				if(cmd.args[1]) {
					list_dir_contents(cmd.args[1]);
				} else {
					list_dir_contents(current_dir);
				}
			} else if (streq(cmd.command, "env")) {
				while (*envp) {
					printf("%s\n", *envp);
					envp++;
				}
			} else if (streq(cmd.command, "cd")) {
				change_dir(cmd.args[1]);
				free(current_dir);
				current_dir = get_current_dir();
			} else if (streq(cmd.command, "cls")) {
				system("clear");
			} else {
				execute_bin(cmd.command, cmd.args);
			}
		}
		for(int i = 0; i < 128; i++) {
			// free_command(commands[i]);
		}
	}
	free(current_dir);
	return 0;
}

