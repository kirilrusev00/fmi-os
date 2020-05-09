#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char* argv[]) {
	if (argc != 4) {
		errx(1, "Wrong argument count");
	}

	struct stat st;
	if ((stat(argv[1], &st)) < 0) {
		err(2, "failed stat for file %s", argv[1]);
	}
	off_t f1_size = st.st_size;	

	if ((stat(argv[2], &st)) < 0) {
        err(3, "failed stat for file %s", argv[2]);
    }
	off_t f2_size = st.st_size;

	struct pair_t {
		uint32_t start;
		uint32_t len;
	};

	if (f1_size % sizeof(struct pair_t) != 0) {
		errx(4, "wrong size of file %s", argv[1]);
	}
	if (f2_size % sizeof(uint32_t) != 0) {
        errx(5, "wrong size of file %s", argv[2]);
    }

	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 < 0) {
		err(6, "failed to open %s", argv[1]);
	}
	int fd2 = open(argv[2], O_RDONLY);
    if (fd2 < 0) {
    	int olderrno = errno;
		close(fd1);
		errno = olderrno;
	    err(7, "failed to open %s", argv[2]);
    }
	int fd3 = open(argv[3], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd3 < 0) {
        int olderrno = errno;
        close(fd1);
		close(fd2);
        errno = olderrno;
		err(8, "failed to open %s", argv[3]);
    }
	
	struct pair_t pair;

	while (read(fd1, &pair, sizeof(pair))) {
		off_t offset = lseek(fd2, (pair.start-1)*sizeof(uint32_t), SEEK_SET);
		if (offset < 0) {
			int olderrno = errno;
        	close(fd1);
        	close(fd2);
        	close(fd3);
			errno = olderrno;
        	err(9, "failed lseek for %s", argv[2]);
		}

		uint32_t c;
		uint32_t nums_written = 0;
		while (read(fd2, &c, sizeof(c))) {
			if (nums_written == pair.len) {
				break;
			}

			if (write(fd3, &c, sizeof(c)) != sizeof(c)) {
				int olderrno = errno;
    	        close(fd1);
	           	close(fd2);
        	    close(fd3);
    	        errno = olderrno;
	            err(10, "failed write to %s", argv[3]);
			}

			nums_written++;
		}
	}	

	close(fd1);
	close(fd2);
	close(fd3);

	exit(0);
}


