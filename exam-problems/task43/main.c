#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

// cat <file_name> | sort

int main(int argc, char* argv[]) {
	if (argc != 2) {
		errx(1, "Usage: %s <file_name>", argv[0]);
	}

	int a[2];
	if (pipe(a) == -1) {
		err(2, "Failed pipe");
	}

	pid_t pid = fork();
	if (pid == -1) {
		err(3, "Failed fork");
	}

	if (pid == 0) {
		close(a[0]);
		dup2(a[1], 1);
		if (execlp("cat", "cat", argv[1], (char*) NULL) == -1) {
			err(4, "Failed exec");
		}
	}
	wait(NULL);
	close(a[1]);
	dup2(a[0], 0);
	if (execlp("sort", "sort", (char*) NULL) == -1){
		err(4, "Failed exec");
	}

	exit(0);
}
