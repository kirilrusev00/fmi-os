#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

void cat(int fd) {
	char c;
    while (read(fd, &c, sizeof(c)) > 0) {
        printf("%c", c);
    }
}

int cat_n(int fd, int count) {
	char c;
    int is_start_line = 1;
    while (read(fd, &c, sizeof(c)) > 0) {
        if (is_start_line == 1) {
            printf("\t%d ", count);
            count++;
            is_start_line = 0;
        }
        printf("%c", c);
        if (c == '\n') {
            is_start_line = 1;
        }
    }
	return count;
}

int main(int argc, char* argv[]) {
	if (argc == 1) {
		cat(0);
		exit(0);
	}

	if (strcmp(argv[1], "-n") == 0) {
		int count = 1;

		if (argc == 2) {
			cat_n(0, count);
        	exit(0);
		}
	
		for (int i = 2; i < argc; i++) {
			if (strcmp(argv[i], "-") == 0) {
				count = cat_n(0, count);
				continue;
			}

			int fd = open(argv[i], O_RDONLY);
			if (fd < 0) {
				err(1, "failed to open %s", argv[i]);
			}

			count = cat_n(fd, count);
			close(fd);
		}
	} else {
		for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-") == 0) {
                cat(0);
                continue;
            }

            int fd = open(argv[i], O_RDONLY);
            if (fd < 0) {
                err(1, "failed to open %s", argv[i]);
            }

            cat(fd);
            close(fd);
        }	
	}

	exit(0);
}
