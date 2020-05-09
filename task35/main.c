#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	if (argc != 4) {
		errx(1, "wrong argument count");
	}

	const char* patch = argv[1];
	const char* f1 = argv[2];
	const char* f2 = argv[3];

	struct stat st;
	if (stat(f1, &st) < 0) {
		err(1, "failed stat of %s", f1);
	}
	off_t f1_size = st.st_size;
	if (stat(f2, &st) < 0) {
        err(1, "failed stat of %s", f2);
    }
	if (f1_size != st.st_size) {
		errx(1, "sizes of %s and %s are not equal", f1, f2);
	}

	struct entry_t {
		uint16_t off;
		uint8_t orig;
		uint8_t new;
	};

	const int fd_f1 = open(f1, O_RDONLY);
	if (fd_f1 < 0) {
		err(1, "failed to open %s", f1);
	} 
	const int fd_f2 = open(f2, O_RDONLY);
    if (fd_f2 < 0) {
		int olderrno = errno;
		close(fd_f1);
		errno = olderrno;
        err(1, "failed to open %s", f2);
    } 
	const int fd_patch = open(patch, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd_patch < 0) {
        int olderrno = errno;
        close(fd_f1);
		close(fd_f2);
        errno = olderrno;
        err(1, "failed to open %s", patch);
    }

	uint8_t a;
	uint8_t b;
	
	uint16_t offset = 0;
	struct entry_t e;
	while(read(fd_f1, &a, sizeof(a)) && read(fd_f2, &b, sizeof(b))) {
		printf("f1:%d\tf2:%d\n", a, b);
		if (a != b) {
			e.off = offset;
			e.orig = a;
			e.new = b;
			
			if (write(fd_patch, &e, sizeof(e)) != sizeof(e)) {
				int olderrno = errno;
        		close(fd_f1);
        		close(fd_f2);
        		close(fd_patch);
				errno = olderrno;
        		err(1, "failed write to %s", patch);
			}
		}
		offset++;
	}

	close(fd_f1);
    close(fd_f2);
	close(fd_patch);

	exit(0);
}

