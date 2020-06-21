// cat /etc/passwd | cut -d ':' -f 7 | sort | uniq
// cut -d ':' -f 7 /etc/passwd | sort | uniq

#include <unistd.h>
#include <sys/types.h>
#include <err.h>
#include <stdlib.h>

int main()
{
	int a[2];
	
	if (pipe(a) == -1)
	{
		err(1, "Cannot create pipe");
	}

	pid_t p = fork();
	if (p < 0)
	{
		err(2, "Cannot fork");
	}

	if (p == 0)
	{
		close(a[0]);
		if (dup2(a[1], 1) < 0)
		{
			err(3, "Cannot make a duplicate");
		}
		if (execlp("cut", "cut", "-d:", "-f7", "/etc/passwd", (char*)NULL) < 0)
		{
			err(4, "Cannot execute command cut");	
		}
	}
	close(a[1]);

	int b[2];
	if (pipe(b) < 0)
	{
		err(5, "Cannot create pipe");
	}

	pid_t m = fork();
	if (m < 0)
	{
		err(6, "Cannot fork");
	}

	if (m == 0)
	{
		if (dup2(a[0], 0) < 0)
		{
			err(7, "Cannot make a duplicate");
		}
		close(b[0]);
		if (dup2(b[1], 1) < 0)
		{
			err(8, "Cannot make a duplicate");
		}
		if (execlp("sort", "sort", (char*)NULL) < 0)
		{
			err(9, "Cannot execute command sort");
		}
	}
	close(b[1]);

	if (dup2(b[0], 0) < 0)
	{
		err(10, "Cannot make a duplicate");
	}
	if (execlp("uniq", "uniq", (char*)NULL) < 0)
	{
		err(11, "Cannot execute command uniq");
	}

	exit(0);
}
