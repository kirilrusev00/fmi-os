/*
	Да се напише програма на С, която получава като параметър команда (без параметри) и при успешното ѝ изпълнение, извежда на стандартния изход името на командата.
*/

#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		errx(1, "Usage: %s command_name", argv[0]);
	}

	pid_t pid = fork();
	if (pid == 0) {
		if (execlp(argv[1], argv[1], (char*) NULL) == -1) {
			err(2, "Error executing command");
		}
	}
	int status;
	waitpid(pid, &status, 0);
	if (WEXITSTATUS(status) == 0) {
		wait(NULL);
		printf("%s\n", argv[1]);
	}

	exit(0);
}
