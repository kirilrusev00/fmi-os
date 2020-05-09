#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <errno.h>

int cmp(const void* a, const void* b) {
	return *((unsigned char*)a) - *((unsigned char*)b);
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		errx(1, "Wrong argument count");
	}

	int fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		err(2, "failed to open %s", argv[1]);
	}

	struct stat st;
	if (stat(argv[1], &st) == -1) {
		err(3, "failed stat for %s", argv[1]);
	}
	
	if (st.st_size == 0) {
		int olderrno = errno;
		close(fd);
		errno = olderrno;
		exit(0);
	}

	void* buf = malloc(st.st_size);
	if (!buf) {
		int olderrno = errno;
        close(fd);
        errno = olderrno;
		err(4, "no memory");
	}
	
	if (read(fd, buf, st.st_size) != st.st_size) {
		int olderrno = errno;
        close(fd);
		free(buf);
        errno = olderrno;
        err(5, "failed reading from %s", argv[1]);
	}

	qsort(buf, st.st_size, 1, cmp);

	fd = open(argv[2], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd < 0) {
        err(6, "failed to open %s", argv[2]);
    }
	if (write(fd, buf, st.st_size) != st.st_size) {
		int olderrno = errno;
		close(fd);
		free(buf);
		errno = olderrno;
		err(7, "failed to write to %s", argv[2]);
	}

	free(buf);
	close(fd);

	exit(0);
}

