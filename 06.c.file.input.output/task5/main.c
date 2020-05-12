// копирайте съдържанието на файл1 във файл2
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	if (argc != 3) {
		errx(1, "Not right number of arguments");
	}

	int fd1 = open(argv[1], O_RDWR);
	if (fd1 < 0) {
		err(2, "failed to open %s", argv[1]);
	}

	int fd2 = open(argv[2], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd2 < 0) {
		int olderrno=errno;
		close(fd1);
		errno=olderrno;
        err(3, "failed to open %s", argv[2]);
    }
 
	char c[4096];
	ssize_t read_size;

	while ((read_size = read(fd1, &c, sizeof(c))) > 0) {
		if (write(fd2, &c, read_size) != read_size) {
			int olderrno=errno;
        	close(fd1);
			close(fd2);
        	errno=olderrno;
        	err(4, "failed to write to %s", argv[2]);
		}
	}

	close(fd1);
	close(fd2);

	exit(0);
}
