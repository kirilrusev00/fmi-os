#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/wait.h>

int logLine(const char* path, time_t start, time_t end, int exitCode)
{
	char buf[3*sizeof(intmax_t)+4];
	sprintf(buf, "%jd %jd %d\n", (intmax_t)start, (intmax_t)end, exitCode);
	int fd = open(path, O_CREAT | O_APPEND | O_WRONLY, 0644);
	if(fd == -1) return -1;
	int len = strlen(buf);
	if(write(fd, buf, len) != len)
	{
		errno = EIO;
		return -1;
	}
	if(close(fd) == -1) return -1;
	return 0;
}

int main(int argc, char** argv)
{
	if(argc < 3)
		errx(1, "wrong number of arguments");
	char *duration = argv[1];
	if(*duration < '0' || *duration > '9' || duration[1])
		errx(2, "invalid first argument");
	int condOld, condNew = 0;
	do
	{
		time_t start = time(NULL);
		pid_t p = fork();
		if(p == 0)
			if(execv(argv[2], argv+2) == -1)
				err(5, "execv failed");
		if(p == -1)
			err(4, "could not fork");
		int status;
		wait(&status);
		time_t end = time(NULL);

		const int exitcode = (WIFEXITED(status)) ? (WEXITSTATUS(status)) : 129;

		if(logLine("run.log", start, end, exitcode) < 0)
			err(6, "error");
		condOld = condNew;
		condNew = WEXITSTATUS(status) && end-start < *duration-'0';
	}
	while(!condOld || !condNew);
}
