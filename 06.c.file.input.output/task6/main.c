// Реализирайте команда cat, работеща с произволен брой подадени входни параметри.

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	if (argc <= 1) {
		errx(1, "You should give at least 1 argument");
	}

	int fd;
	char c[4096];
	ssize_t read_size;
	for (int i = 1; i < argc; i++) {
		fd = open(argv[i], O_RDWR);
		if (fd < 0) {
			err(2, "Failed to open %s", argv[i]);
		}
		while ((read_size = read(fd, &c, sizeof(c))) > 0) {
			if (write(1, &c, read_size) != read_size) {
				int olderrno = errno;
				close(fd);
				errno = olderrno;
				err(3, "Failed to write");
			}
		}
		close(fd);
	}

	exit(0);
}

