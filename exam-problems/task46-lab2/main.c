#include <err.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

const int maxArgs = 2;

int main (int argc, char** argv) {
	if (argc > 2) {
		errx(1, "wrong number of arguments");
	}

	char* cmd;
	if (argc == 2) {
		cmd = argv[1];
	}
	else {
		cmd = "echo";
	}
	
	char args[maxArgs][6];
//		args[0] - par1
//		args[1] - par2
								// argv[0]	argv[1]	argv[2]	NULL
	char* pargs[maxArgs + 2];	// cmd		par1	par2	NULL
	pargs[0] = cmd;

	for (int i = 0; i < maxArgs; i++) {
		pargs[i+1] = args[i];
	}
	pargs[maxArgs+1] = NULL;
	int i = 0;

	// args[0] vs. args[1]
	while (scanf("%5s", args[i%maxArgs]) != EOF) {
		if (strlen(args[i%maxArgs]) > 4) {
			errx(2, "all arguments must have length <= 4");
		}
		i++;

		if (!(i%maxArgs)) {
			pid_t p = fork();
			if (p == 0) {
				if (execvp(cmd, pargs) == -1) {
					err(4, "execvp failed");
				}
			}
			if (p == -1) {
				err(3, "could not fork");
			}
			wait(NULL);
		}
	}

	if (i % maxArgs) {
		pargs[i%maxArgs+1] = NULL;
		if (execvp(cmd, pargs) == -1) {
			err(4, "execvp failed");
        }
	}

	exit(0);
}
