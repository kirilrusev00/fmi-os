#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv) {
	if (argc != 5) {
		errx(1, "Usage: ./main f1.dat f1.idx f2.dat f2.idx");
	}
	
	struct triple_t {
		uint16_t offset;
		uint8_t len;
		uint8_t reserved;
	};

	char* dat_1 = argv[1];
	char* idx_1 = argv[2];
	char* dat_2 = argv[3];
    char* idx_2 = argv[4];	

	struct stat st;
	if (stat(dat_1, &st) < 0) {
		err(2, "failed stat for %s", dat_1);
	}
	if (st.st_size % sizeof(uint8_t) >= 0xFFFF) {
		warnx("size of %s should not be greater than 0xFFFF", dat_1);
	}

	if (stat(idx_1, &st) < 0) {
        err(3, "failed stat for %s", idx_1);
    }
    if (st.st_size % sizeof(struct triple_t) != 0) {
        errx(4, "wrong size of %s", idx_1);
    }

	const int fd_dat1 = open(dat_1, O_RDONLY);
	if (fd_dat1 < 0) {
		err(5, "failed to open %s", dat_1);
	}
	
	const int fd_idx1 = open(idx_1, O_RDONLY);
	if (fd_idx1 < 0) {
		int olderrno = errno;
		close(fd_dat1);
		errno = olderrno;
        err(5, "failed to open %s", idx_1);
    }

	const int fd_dat2 = open(dat_2, O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);
	if (fd_dat2 < 0) {
        int olderrno = errno;
        close(fd_dat1);
		close(fd_idx1);
        errno = olderrno;
        err(5, "failed to open %s", dat_2);
    }

	const int fd_idx2 = open(idx_2, O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);
	if (fd_idx2 < 0) {
        int olderrno = errno;
        close(fd_dat1);
        close(fd_idx1);
        close(fd_dat2);
		errno = olderrno;
        err(5, "failed to open %s", idx_2);
    }

	struct triple_t tr1;
	struct triple_t tr2 = { 0, 0, 0 };

	while(read(fd_idx1, &tr1, sizeof(tr1)) == sizeof(tr1)) {
		if (lseek(fd_dat1, tr1.offset, SEEK_SET) < 0) {
			int olderrno = errno;
			close(fd_dat1);
			close(fd_idx1);
			close(fd_dat2);
			close(fd_idx2);
			errno = olderrno;
			err(6, "cannot seek to %d in %s", tr1.offset, dat_1);	
		}

		uint8_t* buf = malloc(tr1.len);
		if (!buf) {
			int olderrno = errno;
            close(fd_dat1);
            close(fd_idx1);
            close(fd_dat2);
            close(fd_idx2);
            errno = olderrno;
            err(7, "failed malloc");
		}
		size_t read_size = read(fd_dat1, buf, tr1.len * sizeof(uint8_t));
		if (read_size != tr1.len * sizeof(uint8_t)) {
            int olderrno = errno;
			close(fd_dat1);
            close(fd_idx1);
            close(fd_dat2);
            close(fd_idx2);
			free(buf);
            errno = olderrno;
            err(8, "failed read from %s", dat_1);
		} 
		if ((buf[0] < 'A') || (buf[0] > 'Z')) {
			free(buf);
			continue;
		}
		if (write(fd_dat2, buf, tr1.len * sizeof(uint8_t)) < 0) {
			int olderrno = errno;
            close(fd_dat1);
           	close(fd_idx1);
       	    close(fd_dat2);
       	    close(fd_idx2);
   	        free(buf);
			errno = olderrno;
	        err(9, "failed write to %s", dat_2);
		}

		tr2.len = tr1.len;
		tr2.offset += tr2.len;

		if (write(fd_idx2, &tr2, sizeof(tr2)) < 0) {
            int olderrno = errno;
            close(fd_dat1);
            close(fd_idx1);
            close(fd_dat2);
            close(fd_idx2);
            free(buf);
            errno = olderrno;
            err(10, "failed write to %s", idx_2);
        }

		free(buf);
	}

	close(fd_dat1);
	close(fd_idx1);	
	close(fd_dat2);
	close(fd_idx2);

	exit(0);
}
