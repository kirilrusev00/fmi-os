#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdlib.h>

// find argv[1] -type f -printf "%T@ %P\n" | sort | tail -n1 | cut -d ' ' -f2

int main(int argc, char* argv[]) {
	if (argc != 2) {
		errx(1, "Usage: %s <dirname>", argv[0]);
	} 

	int a[2];
	if (pipe(a) == -1) {
		err(2, "could not pipe");
	}

	pid_t pid1 = fork();
	if (pid1 < 0) {
		err(3, "could not fork");
	}
	if (pid1 == 0) {
		close(a[0]);
		if (dup2(a[1], 1) == -1) {
			err(4, "could not exec dup2");
		}
		if (execlp("find", "find", argv[1], "-type", "f", "-printf", "%T@ %P\n", (char*) NULL) == -1) {
			err(5, "could not execute find");
		}
	}
	close(a[1]);

	int b[2];
	if (pipe(b) == -1) {
		err(2, "could not pipe");
	}

	pid_t pid2 = fork();
	if (pid2 < 0) {
		err(3, "could not fork");
	}
	if (pid2 == 0) {
		close(b[0]);
		if (dup2(a[0], 0) == -1) {
			err(4, "could not exec dup2");
		}
		if (dup2(b[1], 1) == -1) {
			err(4, "could not exec dup2");
		}
		if (execlp("sort", "sort", (char*) NULL) == -1) {
			err(6, "could not execute sort");
		}
	}
	close(a[0]);
	close(b[1]);

	int c[2];
	if (pipe(c) == -1) {
		err(2, "could not pipe");
	}

	pid_t pid3 = fork();
	if (pid3 < 0) {
		err(3, "could not fork");
	}
	if (pid3 == 0) {
		close(c[0]);
		if (dup2(b[0], 0) == -1) {
			err(4, "could not exec dup2");
		}
		if (dup2(c[1], 1) == -1) {
			err(4, "could not exec dup2");
		}
		if (execlp("tail", "tail", "-n1", (char*) NULL) == -1) {
			err(7, "could not execute tail");
		}
	}
	close(b[0]);
	close(c[1]);

	int d[2];
	if (pipe(d) == -1) {
		err(2, "could not pipe");
	}

	if (dup2(c[0], 0) == -1) {
		err(4, "could not exec dup2");
	}
	if (execlp("cut", "cut", "-d", " ", "-f2", (char*) NULL) == -1) {
		err(8, "could not execute cut");
	}

	int status;
	if (waitpid(pid1, &status, 0) == -1) {
		err(9, "failed wait");
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
		errx(10, "command failed");
	}

	if (waitpid(pid2, &status, 0) == -1) {
		err(9, "failed wait");
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
		errx(10, "command failed");
	}

	if (waitpid(pid3, &status, 0) == -1) {
		err(9, "failed wait");
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
		errx(10, "command failed");
	}

	exit(0);
}
