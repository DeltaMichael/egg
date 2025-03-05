#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>

bool streq(char* str1, char* str2) {
	return strcmp(str1, str2) == 0;
}

int main(int argc, char **argv) {
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
			} else {
            	fwrite(line, nread, 1, stdout);
			}
		}
	}
	free(current_dir);
	return 0;
}

