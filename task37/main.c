#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <errno.h>
#include <stdint.h>

int main(int argc, char* argv[]) {
	if (argc != 4) {
		errx(1, "wrong argument count");
	}

	char* patch = argv[1];
	char* f1 = argv[2];
	char* f2 = argv[3];

	struct entry_t {
		uint16_t off;
		uint8_t orig;
		uint8_t new;
	};

	struct stat st;
	if (stat(patch, &st) < 0) {
		err(1, "failed stat for %s", patch);
	}

	int fd_f1 = open(f1, O_RDONLY);
	if (fd_f1 < 0) {
		err(1, "failed open of %s", f1);
	}
	int fd_patch = open(patch, O_RDONLY);
    if (fd_patch < 0) {
        int olderrno = errno;
		close(fd_f1);
		errno = olderrno;
		err(1, "failed open of %s", patch);
    }
	int fd_f2 = open(f2, O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd_patch < 0) {
        int olderrno = errno;
        close(fd_f1);
		close(fd_patch);
        errno = olderrno;
        err(1, "failed open of %s", f2);
    }

	uint8_t buf;
	while (read(fd_f1, &buf, sizeof(buf)) > 0) {
		if (write(fd_f2, &buf, sizeof(buf)) != sizeof(buf)) {
			int olderrno = errno;
        	close(fd_f1);
			close(fd_f2);
        	close(fd_patch);
        	errno = olderrno;
        	err(1, "failed write to %s", f2);
		}
	}
	close(fd_f1);

	struct entry_t e;
	while (read(fd_patch, &e, sizeof(e)) == sizeof(e)) {
		int off_f2 = lseek(fd_f2, e.off, SEEK_SET);
		if (off_f2 < 0) {
			int olderrno = errno;
            close(fd_f2);
            close(fd_patch);
            errno = olderrno;
            err(1, "failed lseek for %s", f2);
		}
		if (write(fd_f2, &e.new, sizeof(e.new)) != sizeof(e.new)) {
            int olderrno = errno;
            close(fd_f2);
            close(fd_patch);
            errno = olderrno;
            err(1, "failed write to %s", f2);
        }
	}	

    close(fd_f2);
    close(fd_patch);
	
	exit(0);
}
