/*
	Да се напише програма на С, която получава като параметър име на файл. Създава процес син, който записва стринга foobar във файла (ако не съществува, го създава, в противен случай го занулява), след което процеса родител прочита записаното във файла съдържание и го извежда на стандартния изход, добавяйки по един интервал между всеки два символа.
*/

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		errx(1, "Usage: %s file", argv[0]);
	}

	int pid = fork();
    if (pid == -1) {
        err(3, "Error in fork");
    }
	if (pid == 0) {
		int fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	    if (fd < 0) {
    	    err(2, "Error opening %s", argv[1]);
    	}

		char* msg = "foobar";
		const int MSG_SIZE = 6;
		if (write(fd, msg, MSG_SIZE) != MSG_SIZE) {
			int olderrno = errno;
        	close(fd);
        	errno = olderrno;
        	err(4, "Error writing to %s", argv[1]);
		}
		close(fd);
	}
	else {
		wait(NULL);
		int fd = open(argv[1], O_RDONLY);
	    if (fd < 0) {
    	    err(2, "Error opening %s", argv[1]);
    	}

		char c;
		while (read(fd, &c, sizeof(c)) > 0) {
			printf("%c ", c);
		}
		printf("\n");
		close(fd);
	}

	exit(0);
}
