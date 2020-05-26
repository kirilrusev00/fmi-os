/*
	Да се напише програма на C, която която създава файл в текущата директория и генерира два процесa, които записват низовете foo и bar в създадения файл.
	-	Програмата не гарантира последователното записване на низове.
	-	Променете програмата така, че да записва низовете последователно, като първия е foo.
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <errno.h>

void wr_msg(int fd, char* msg, int size) {
	if (write(fd, msg, size) != size) {
		int olderrno = errno;
		close(fd);
		errno = olderrno;
		err(4, "Error writing to file");
	}
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		errx(1, "Usage: %s file", argv[0]);
	}
	
	int fd = open(argv[1], O_TRUNC | O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
	if (fd < 0) {
		err(2, "Error opening %s", argv[1]);
	}
	
	int pid = fork();
	if (pid < 0) {
		int olderrno = errno;
		close(fd);
		errno = olderrno;
		err(3, "Error in fork");
	}
	if (pid == 0) {
		wr_msg(fd, "foo", 3);
	}
	else {
		wait(NULL); // Comment to wait for "foo" to be written first
		int pid2 = fork();
    	if (pid2 < 0) {
        	int olderrno = errno;
        	close(fd);
        	errno = olderrno;
    	    err(3, "Error in fork");
	    }
    	if (pid2 == 0) {
        	wr_msg(fd, "bar", 3);
    	}
	}

	exit(0);
}
