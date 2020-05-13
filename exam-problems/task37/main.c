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
		errx(1, "Usage: %s patch.bin f1.bin f2.bin", argv[0]);
	}

	struct entry_t {
		uint16_t pos;
		uint8_t orig;
		uint8_t nb;
	};

	struct stat st;
	if (stat(argv[1], &st) < 0) {
		err(2, "failed stat for %s", argv[1]);
	}

	if (st.st_size % sizeof(struct entry_t) != 0) {
		errx(3, "wrong size of %s", argv[1]);
	}

	if (stat(argv[2], &st) < 0) {
		err(2, "failed stat for %s", argv[2]);
	}
	if (st.st_size > 0xFFFF) {
		warnx("Size of %s is greater than 0xFFFF", argv[2]);
	}

	int fd_f1 = open(argv[2], O_RDONLY);
	if (fd_f1 < 0) {
		err(4, "failed open of %s", argv[2]);
	}
	int fd_f2 = open(argv[3], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd_f2 < 0) {
        int olderrno = errno;
        close(fd_f1);
        errno = olderrno;
        err(4, "failed open of %s", argv[3]);
    }

	uint8_t buf;
	while (read(fd_f1, &buf, sizeof(buf)) > 0) {
		if (write(fd_f2, &buf, sizeof(buf)) != sizeof(buf)) {
			int olderrno = errno;
        	close(fd_f1);
			close(fd_f2);
        	errno = olderrno;
        	err(5, "failed write to %s", argv[3]);
		}
	}
	close(fd_f1);

	int fd_patch = open(argv[1], O_RDONLY);
    if (fd_patch < 0) {
        int olderrno = errno;
        close(fd_f2);
        errno = olderrno;
        err(4, "failed open of %s", argv[1]);
    }

	struct entry_t e;
	while (read(fd_patch, &e, sizeof(e)) == sizeof(e)) {
		if (lseek(fd_f2, e.pos, SEEK_SET) < 0) {
			int olderrno = errno;
            close(fd_f2);
            close(fd_patch);
            errno = olderrno;
            err(6, "failed seek to %d in %s", e.pos, argv[3]);
		}
		
		read(fd_f2, &buf, sizeof(buf));

		if (buf == e.orig) {
			if (lseek(fd_f2, -1, SEEK_CUR) < 0) {
            	int olderrno = errno;
            	close(fd_f2);
            	close(fd_patch);
            	errno = olderrno;
            	err(6, "failed seek to %d in %s", e.pos, argv[3]);
        	}
			if (write(fd_f2, &(e.nb), sizeof(e.nb)) != sizeof(e.nb)) {
				int olderrno = errno;
                close(fd_f2);
                close(fd_patch);
                errno = olderrno;
                err(5, "failed write to %s", argv[3]);
			}
		}
		else {
			errx(7, "There is no such byte in %s", argv[2]);
		}
	}	

    close(fd_f2);
    close(fd_patch);
	
	exit(0);
}
