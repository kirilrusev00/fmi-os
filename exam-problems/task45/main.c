#include <unistd.h>
#include <sys/types.h>
#include <err.h>
#include <stdlib.h>

// cut -d: -f7 /etc/passwd | sort | uniq -c | sort -n

int main() {
	int a[2];
	if (pipe(a) == -1) {
		err(1, "Failed pipe");
	}

	pid_t pid = fork();
	if (pid == -1) {
		err(2, "Failed fork");
	}

	if (pid == 0) {
		close(a[0]);
		if (dup2(a[1], 1) == -1) {
			err(3, "Failed dup");
		}
		if (execlp("cut", "cut", "-d:", "-f7", "/etc/passwd", (char*) NULL) == -1) {
			err(4, "Failed exec");
		} 
	}
	close(a[1]);

	int b[2];
	if (pipe(b) == -1) {
        err(1, "Failed pipe");
    }

	pid_t pid2 = fork();
    if (pid2 == -1) {
        err(2, "Failed fork");
    }

	if (pid2 == 0) {
        if (dup2(a[0], 0) == -1) {
            err(3, "Failed dup");
        }
		close(b[0]);
        if (dup2(b[1], 1) == -1) {
            err(3, "Failed dup");
        }
        if (execlp("sort", "sort", (char*) NULL) == -1) {
            err(4, "Failed exec");
        } 
    }
	close(a[0]);
	close(b[1]);

    int c[2];
    if (pipe(c) == -1) {
        err(1, "Failed pipe");
    }

    pid_t pid3 = fork();
    if (pid3 == -1) {
        err(2, "Failed fork");
    }

    if (pid3 == 0) {
        if (dup2(b[0], 0) == -1) {
            err(3, "Failed dup");
        }
        close(c[0]);
        if (dup2(c[1], 1) == -1) {
            err(3, "Failed dup");
        }
        if (execlp("uniq", "uniq", "-c", (char*) NULL) == -1) {
            err(4, "Failed exec");
        }
    }
	close(b[0]);
	close(c[1]);

	if (dup2(c[0], 0) == -1) {
		err(3, "Failed dup");
	}
	if (execlp("sort", "sort", "-n", (char*) NULL) == -1) {
            err(4, "Failed exec");
    }

	exit(0);
}
