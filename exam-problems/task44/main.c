#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main() {
	while(1) {
		if (write(1, "enter command: ", 15) != 15) {
			err(1, "Error writing to stdout");
		}
		
		char buf[1024];
		ssize_t read_size = read(0, &buf, sizeof(buf));
		if (read_size < 0) {
			err(2, "Error reading from stdin");
		}
		buf[read_size - 1] = '\0';

		if (strcmp(buf, "exit") == 0) {
			break;
		}

		pid_t pid = fork();
		if (pid < 0) {
			warn("Failed fork");
			continue;
		}

		if (pid == 0) {
			if (execlp(buf, buf, (char*) NULL) == -1) {
				err(3, "Error executing command %s", buf);
			}
		}

		wait(NULL);

	}

	exit(0);
}
