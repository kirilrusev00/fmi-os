#include <sys/types.h>
#include <sys/wait.h>

#include <err.h>
#include <unistd.h>

static void
wait_for_child(const char * const name, const pid_t pid)
{
	int status;
	const pid_t npid = waitpid(pid, &status, 0);
	if (npid != pid)
		errx(1, "parent: tried to wait for %s at %ld, got %ld",
		    name, (long)pid, (long)npid);
	else if (WIFSIGNALED(status))
		errx(1, "parent: the %s child was killed by signal %d",
		    name, WTERMSIG(status));
	else if (!WIFEXITED(status))
		errx(1, "parent: the %s child neither exited normally nor "
		    "was killed by a signal, status %d",
		    name, status);
	else if (WEXITSTATUS(status) != 0)
		errx(1, "parent: unexpected exit status for %s: %d",
		    name, WEXITSTATUS(status));
}

static void
child_pipes(const char * const name, const int * const pipe_in,
	const int * const pipe_out)
{
	if (pipe_in != NULL)
	{
		if (close(0) == -1)
			err(1, "ch-%s: could not close the parent's stdin", name);
		if (dup2(pipe_in[0], 0) == -1)
			err(1, "ch-%s: could not fix up stdin", name);
		if (close(pipe_in[0]) == -1)
			err(1, "ch-%s: could not close the old stdin fd", name);
	}

	if (pipe_out != NULL)
	{
		if (close(pipe_out[0]) == -1)
			err(1, "ch-%s: could not close the read end", name);
		if (close(1) == -1)
			err(1, "ch-%s: could not close the parent's stdout", name);
		if (dup2(pipe_out[1], 1) == -1)
			err(1, "ch-%s: could not fix up stdout", name);
		if (close(pipe_out[1]) == -1)
			err(1, "ch-%s: could not close the old stdout fd", name);
	}
}

int
main(const int argc, const char * const argv[])
{
	if (argc != 2)
		errx(1, "usage: pipes /path/to/dir");

	int pipe_find_sort[2];
	if (pipe(pipe_find_sort) == -1)
		err(1, "Could not create the first pipe");
	const pid_t pid_find = fork();
	if (pid_find == -1) {
		err(1, "Could not fork for find");
	} else if (pid_find == 0) {
		child_pipes("find", NULL, pipe_find_sort);
		execlp("find", "find", "--", argv[1], "-printf", "%T@ %p\\n",
		    NULL);
		err(1, "ch-find: could not run find(1)");
	}
	if (close(pipe_find_sort[1]) == -1)
		err(1, "parent: could not close the find pipe write end");

	int pipe_sort_head[2];
	if (pipe(pipe_sort_head) == -1)
		err(1, "Could not create the second pipe");
	const pid_t pid_sort = fork();
	if (pid_sort == -1) {
		err(1, "Could not fork for sort");
	} else if (pid_sort == 0) {
		child_pipes("sort", pipe_find_sort, pipe_sort_head);
		execlp("sort", "sort", "-rn", NULL);
		err(1, "ch-sort: could not run sort(1)");
	}
	if (close(pipe_sort_head[1]) == -1)
		err(1, "parent: could not close the sort pipe write end");

	int pipe_head_cut[2];
	if (pipe(pipe_head_cut) == -1)
		err(1, "Could not create the third pipe");
	const pid_t pid_head = fork();
	if (pid_head == -1) {
		err(1, "Could not fork for head");
	} else if (pid_head == 0) {
		child_pipes("head", pipe_sort_head, pipe_head_cut);
		execlp("head", "head", "-n", "1", NULL);
		err(1, "ch-head: could not run head(1)");
	}
	if (close(pipe_head_cut[1]) == -1)
		err(1, "parent: could not close the head pipe write end");

	const int pid_cut = fork();
	if (pid_cut == -1) {
		err(1, "Could not fork for cut");
	} else if (pid_cut == 0) {
		child_pipes("cut", pipe_head_cut, NULL);
		execlp("cut", "cut", "-d", " ", "-f", "2", NULL);
		err(1, "ch-cut: could not run cut(1)");
	}
	if (close(pipe_find_sort[0]) == -1)
		err(1, "parent: could not close the find pipe read end");

	wait_for_child("find", pid_find);
	wait_for_child("sort", pid_sort);
	wait_for_child("head", pid_head);
	wait_for_child("cut", pid_cut);
	return 0;
}
