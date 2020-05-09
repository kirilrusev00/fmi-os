#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

#define SIZE 256

int main(int argc, char* argv[]) {
	if (argc != 3) {
		errx(1, "wrong argument count");
	}

	char* input = argv[1];
	char* output = argv[2];

	struct stat st;
	if (stat(input, &st) < 0) {
		err(2, "failed stat for %s", input);
	}
	if (st.st_size % sizeof(uint32_t) != 0) {
		errx(3, "wrong size of %s", input);
	}
	
	int fd_input = open(input, O_RDONLY);
    if (fd_input < 0) {
        err(4, "failed to open %s", input);
    }

	int fd_output = open(output, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd_output < 0) {
        int olderrno = errno;
        close(fd_input);
        errno = olderrno;
        err(5, "failed to open %s", output);
    }
	
	const uint32_t BUF_SIZE = 2<<10;
	uint32_t histo[SIZE] = { 0 };
	uint8_t buf[BUF_SIZE];
	ssize_t read_size;
	uint32_t total_size = 0;

	while((read_size = read(fd_input, &buf, sizeof(buf))) > 0) {
		for (uint32_t i = 0; i < BUF_SIZE; i++) {
			++histo[buf[i]];
			++total_size;
		}
	}
	if (read_size < 0) {
		int olderrno = errno;
        close(fd_input);
		close(fd_output);
        errno = olderrno;
        err(6, "failed to read from %s", input);
	}

	uint16_t byte = 0;
	while (byte < SIZE) {
		size_t pos = 0;
		while (pos < BUF_SIZE && byte < SIZE) {
			if (histo[byte] == 0) {
				byte++;
			}
			else {
				buf[pos] = byte;
				--histo[byte];
				++pos;
			}
		}
		ssize_t write_size = pos;
		if (write(fd_output, &buf, write_size) != write_size) {
			int olderrno = errno;
        	close(fd_input);
        	close(fd_output);
    	    errno = olderrno;
	        err(7, "failed to write to %s", output);
		}
	}

	close(fd_input);
	close(fd_output);
		
	exit(0);
}
