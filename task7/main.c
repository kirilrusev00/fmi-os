/*
	Да се напише програма на С, която получава като параметри три команди (без параметри), изпълнява ги последователно, като изчаква края на всяка и извежда на стандартния изход номера на завършилия процес, както и неговия код на завършване.
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

void exec_cmd(char* cmd_name) {
	int pid = fork();
	if (pid == 0) {
        if (execlp(cmd_name, cmd_name, (char*) NULL) == -1) {
			err(2, "Error executing %s", cmd_name);
		}
    } 
	int status;
	waitpid(pid, &status, 0);
	
	printf("pid: %d\texit code: %d\n", pid, WEXITSTATUS(status));
}

int main(int argc, char* argv[]) {
	if (argc != 4) {
		err(1, "Usage: %s cmd1 cmd2 cmd3", argv[0]);
	}

	for (int i = 1; i <= 3; i++) {
		exec_cmd(argv[i]);
	}

	exit(0);
}
