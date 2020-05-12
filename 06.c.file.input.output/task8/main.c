// Koпирайте файл /etc/passwd в текущата ви работна директория и променете разделителят на копирания файл от ":", на "?"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>

int main() {
	int fd1 = open("passwd", O_RDONLY);
	if (fd1 < 0) {
		err(1, "failed to open passwd");
	}

	int fd2 = open("output.txt", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd2 < 0) {
        err(2, "failed to open output.txt");
    }

	char c;
	ssize_t read_size;
	while ((read_size = read(fd1, &c, sizeof(c))) > 0) {
		if (c == ':') {
			c = '?';
		}
		if (write(fd2, &c, sizeof(c)) != read_size) {
			int olderrno = errno;
			close(fd1);
			close(fd2);
			errno=olderrno;
			err(3, "failed to write to output.txt");
		}
	}

	close(fd1);
	close(fd2);

	exit(0);
}
