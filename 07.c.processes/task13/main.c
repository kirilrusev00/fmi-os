/*
	Да се напише програма на C, която получава като командни параметри две команди (без параметри). Изпълнява ги едновременно и извежда на стандартния изход номера на процеса на първата завършила успешно. Ако нито една не завърши успешно извежда -1.
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>

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
	int pid2 = fork();
    if (pid2 < 0) {
        err(2, "Error in fork");
    }
    if (pid2 == 0) {
        if (execlp(argv[2], argv[2], (char*) NULL) == -1) {
            err(3, "Error executing %s", argv[2]);
        }
    }
	int status;	
	pid_t cpid = waitpid(-1, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
		printf("%d\n", cpid);
	}
	else {
		cpid = waitpid(-1, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        	printf("%d\n", cpid);
    	}
		else {
			printf("-1\n");
		}
	}

	exit(0);
}
