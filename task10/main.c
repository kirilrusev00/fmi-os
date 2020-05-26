/*
	Да се напише програма на C, която получава като параметри от команден ред две команди (без параметри). Изпълнява първата. Ако тя е завършила успешно изпълнява втората. Ако не, завършва с код -1.
*/

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <err.h>

int main(int argc, char* argv[]) {
	if (argc != 3) {
		errx(1, "Usage: %s cmd1 cmd2", argv[0]);
	}
	int pid = fork();
	if (pid < 0) {
		err(2, "Error in fork");
	}
	if (pid == 0) {
		if (execlp(argv[1], argv[1], (char*) NULL) == -1) {
			err(3, "Error executing %s", argv[1]);
		}
	}
	int status;
	waitpid(pid, &status, 0);
	
	if (WIFEXITED(status)) {
		if (WEXITSTATUS(status) != 0) {
			exit(-1);
		}
	}

	int pid2 = fork();
    if (pid2 < 0) {
        err(2, "Error in fork");
    }
    if (pid2 == 0) {
        if (execlp(argv[2], argv[2], (char*) NULL) == -1) {
            err(3, "Error executing %s", argv[2]);
        }
    }
	
	wait(NULL);	

	exit(0);
}
