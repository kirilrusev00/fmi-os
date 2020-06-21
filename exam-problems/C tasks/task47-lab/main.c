#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

pid_t executeCommand(char** args, int stdIn, int stdOut, int toClose, const char* msg)
{
	pid_t pid = fork();
	if (pid == -1)
		err(2, "could not fork");

	if (pid == 0)
	{
		if (toClose != -1) {
			close(toClose);
		}
		if (stdIn != -1 && dup2(stdIn, 0) == -1)
			err(4, "could not execute dup2 (%s stdin)", msg);

		if (stdOut != -1 && dup2(stdOut, 1) == -1)
			err(4, "could not execute dup2 (%s stdout)", msg);

		if (execvp(args[0], args) == -1)
	       		err(5, "could not execute execvp");	
	}
	return pid;	
}	

int main(int argc, char** argv)
{
	if (argc != 2)
		errx(1, "usage: ./main [dirname]");

	int a[2];
	if (pipe(a) == -1)
		err(2, "could not pipe");

	char* args[] = { "find", argv[1], "-type", "f", "-printf", "%T@ %P\n", NULL };

	pid_t pid1, pid2, pid3, pid4;
	pid1 = executeCommand(args, -1, a[1], a[0], "1");
	close(a[1]);

	int b[2];
	if (pipe(b) == -1)
		err(2, "could not pipe");
	char* args2[] = { "sort", "-rn", NULL };

	pid2 = executeCommand(args2, a[0], b[1], b[0], "2");
	close(a[0]);
	close(b[1]);

	int c[2];
	if (pipe(c) == -1)
		err(2, "could not pipe");

	char* args3[] = { "head", "-1", NULL };

	pid3 = executeCommand(args3, b[0], c[1], c[0], "3");
	close(b[0]);
	close(c[1]);

	char* args4[] = { "cut", "-d", " ", "-f2", NULL };

	pid4 = executeCommand(args4, c[0], -1, -1, "4");
	close(c[0]);

	int status;
 	if (waitpid(pid1, &status, 0) == -1)
		err(3, "error in wait");

	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		errx(6, "command failed");

	if (waitpid(pid2, &status, 0) == -1)
		err(3, "error in wait");

	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		errx(6, "command failed");

	if (waitpid(pid3, NULL, 0) == -1)
		err(3, "error in wait");	
	
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		errx(6, "command failed");

	if (waitpid(pid4, NULL, 0) == -1)
		err(3, "error in wait");

	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		errx(6, "command failed");

	exit(0);

}
