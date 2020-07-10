#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>

int cmp (const void* a, const void* b) {
	return *((unsigned char*)a) - *((unsigned char*)b);
}

int main (int argc, char* argv[]) {
	if (argc != 3) {
		errx(1, "Wrong arguments count! Usage: %s input.bin output.bin", argv[0]);
	}

	struct stat st;
	if (stat(argv[1], &st) < 0) {
		err(2, "Failed stat for file %s", argv[1]);
	}

	if (st.st_size % sizeof(uint16_t) != 0) {
		errx(3, "Size of %s is invalid", argv[1]);
	}

	uint32_t num_elem = st.st_size / sizeof(uint32_t);
	uint32_t half = num_elem / 2;

	uint32_t *buf = malloc(half * sizeof(uint32_t));
	if (!buf) {
		err(4, "Failed malloc");
	}

	int fd_input = open(argv[1], O_RDONLY);
	if (fd_input < 0) {
		int olderrno = errno;
		free(buf);
		errno = olderrno;
		err(5, "Failed to open %s", argv[1]);
	}

	size_t res = read(fd_input, buf, half*sizeof(uint32_t));
	if (res != half*sizeof(uint32_t)) {
		int olderrno = errno;
        free(buf);
		close(fd_input);
        errno = olderrno;
        err(6, "Failed read from %s", argv[1]);
	}

	qsort(buf, half, sizeof(uint32_t), cmp);

	int fd_temp1 = open("temp1", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd_temp1 < 0) {
		int olderrno = errno;
		free(buf);
		close(fd_input);
		errno = olderrno;
		err(5, "Failed to open temp file 1");
	}
	res = write(fd_temp1, buf, half*sizeof(uint32_t));
	if (res != half*sizeof(uint32_t)) {
		int olderrno = errno;
		free(buf);
		close(fd_input);
		close(fd_temp1);
		errno = olderrno;
		err(7, "Failed to write to temp file 1");
	}

	free(buf);
	half = num_elem - half;
	
	buf = malloc(half * sizeof(uint32_t));
	if (!buf) {
		err(4, "Failed malloc");
	}
	res = read(fd_input, buf, half*sizeof(uint32_t));
	if (res != half*sizeof(uint32_t)) {
		int olderrno = errno;
        free(buf);
		close(fd_input);
    	close(fd_temp1);
	    errno = olderrno;
        err(6, "Failed read from %s", argv[1]);
	}

	close(fd_input);

	qsort(buf, half, sizeof(uint32_t), cmp);
	int fd_temp2 = open("temp2", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd_temp2 < 0) {
		int olderrno = errno;
		free(buf);
		close(fd_input);
		close(fd_temp1);
		errno = olderrno;
		err(5, "Failed to open temp file 2");
	}
	
	res = write(fd_temp2, buf, half*sizeof(uint32_t));
	if (res != half*sizeof(uint32_t)) {
		int olderrno = errno;
		free(buf);
		close(fd_input);
		close(fd_temp1);
		errno = olderrno;
		err(7, "Failed to write to temp file 2");
	}

	free(buf);
	
	if (lseek(fd_temp1, 0, SEEK_SET) < 0) {
		int olderrno = errno;
        close(fd_temp1);
        close(fd_temp2);
        errno = olderrno;
        err(8, "Failed lseek in temp file 1");
    }

	if (lseek(fd_temp2, 0, SEEK_SET) < 0) {
        int olderrno = errno;
        close(fd_temp1);
        close(fd_temp2);
        errno = olderrno;
        err(8, "Failed lseek in temp file 2");
    }

	int fd_output = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd_output < 0) {
		int olderrno = errno;
		close(fd_temp1);
		close(fd_temp2);
		errno = olderrno;
		err(5, "Failed to open file %s", argv[2]);
	}

	uint32_t a;
	uint32_t b;
	while (read(fd_temp1, &a, sizeof(uint32_t)) == sizeof(uint32_t) && read(fd_temp2, &b, sizeof(uint32_t)) == sizeof(uint32_t)) {
		if (a <= b) {
			if (write(fd_output, &a, sizeof(uint32_t)) != sizeof(uint32_t)) {
				int olderrno = errno;
        		close(fd_output);
        		close(fd_temp1);
				close(fd_temp2);
        		errno = olderrno;
        		err(7, "Failed to write to %s", argv[2]);
			}
			if (lseek(fd_temp2, -sizeof(uint32_t), SEEK_CUR) < 0) {
				int olderrno = errno;
                close(fd_output);
                close(fd_temp1);
                close(fd_temp2);
                errno = olderrno;
                err(8, "Failed lseek in temp file 2");
			}
		} else {
			if (write(fd_output, &b, sizeof(uint32_t)) != sizeof(uint32_t)) {
				int olderrno = errno;
        		close(fd_output);
        		close(fd_temp1);
				close(fd_temp2);
        		errno = olderrno;
        		err(7, "Failed to write to %s", argv[2]);
			}
			if (lseek(fd_temp1, -sizeof(uint32_t), SEEK_CUR) < 0) {
                int olderrno = errno;
                close(fd_output);
                close(fd_temp1);
                close(fd_temp2);
                errno = olderrno;
                err(8, "Failed lseek in temp file 1");
            }
		}
	}

	if (read(fd_temp2, &b, sizeof(b)) != sizeof(b)) {
		write(fd_output, &a, sizeof(a));
	} else {
		write(fd_output, &b, sizeof(b));
	}

	while (read(fd_temp1, &a, sizeof(uint32_t)) == sizeof(uint32_t)) {
		if (write(fd_output, &a, sizeof(uint32_t)) != sizeof(uint32_t)) {
			int olderrno = errno;
        	close(fd_output);
        	close(fd_temp1);
			close(fd_temp2);
        	errno = olderrno;
        	err(7, "Failed to write to %s", argv[2]);
		}
	}

	while (read(fd_temp1, &b, sizeof(uint32_t)) == sizeof(uint32_t)) {
        if (write(fd_output, &b, sizeof(uint32_t)) != sizeof(uint32_t)) {
            int olderrno = errno;
            close(fd_output);
            close(fd_temp1);
            close(fd_temp2);
            errno = olderrno;
            err(7, "Failed to write to %s", argv[2]);
        }   
    }

	close(fd_temp1);
	close(fd_temp2);
	close(fd_output);

	exit(0);
}
