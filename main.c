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

bool streq(char* str1, char* str2) {
	return strcmp(str1, str2) == 0;
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

const char* get_file_type(struct dirent *c_entry) {
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

int main(int argc, char **argv, char** envp) {
	int current_dir_length = 128;
	char* current_dir = calloc(current_dir_length, sizeof(uint8_t));
	while (getcwd(current_dir, current_dir_length) == NULL) {
		if (errno == ERANGE) {
			current_dir_length *= 2;
			realloc(current_dir, current_dir_length);
		} else {
			printf("Egg exited errno %d", errno);
			exit(1);
		}
	}
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

	system("clear");
	while (1) {
		printf("(%s (EGG> ", current_dir);
    	if ((nread = getline(&line, &len, stdin)) != -1) {
			if (streq(line, "exit\n")) {
				break;
			} else if (streq(line, "dir\n")) {
				DIR* cd = open_dir(current_dir);
				if (cd == NULL) {
					continue;
				}
				struct dirent* c_entry = readdir(cd);
				while (c_entry != NULL) {
					printf("%s", get_file_type(c_entry));
					printf("\t%s\n", c_entry->d_name);
					c_entry = readdir(cd);
				}
			} else if (streq(line, "env\n")) {
				while (*envp) {
					printf("%s\n", *envp);
					envp++;
				}
			} else {
            	fwrite(line, nread, 1, stdout);
			}
		}
	}
	free(current_dir);
	return 0;
}

