#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	off_t f1_size;
	off_t f2_size;

	struct stat st;
	if (stat("f1", &st) < 0)
    {
    	err(4, "cannot stat f1");
    }
	f1_size = st.st_size;

	if (stat("f2", &st) < 0)
    {
        err(4, "cannot stat f2");
    }
    f2_size = st.st_size;

	struct pair_t
    {
        uint32_t start;
        uint32_t length;
    };

	if (f1_size % sizeof(struct pair_t)!=0)
	{
		errx(5,"Wrong size in f1");
	}

	if (f2_size % sizeof(uint32_t)!=0)
    {
        errx(6,"Error in f2");
    }

	int fd1;
	int fd2;
	int fd3;

	fd1=open("f1", O_RDONLY)
	if (fd1 < 0)
	{
		err(1, "Cannot open file f1");
	}

	fd2=open("f2", O_RDONLY)
    if (fd2 < 0)
    {
        int olderrno=errno;
		close(fd1);
		errno=olderrno;
		err(2, "Cannot open file f2");
    }

	fd3=open("f3", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)
    if (fd3 < 0)
    {
		int olderrno=errno;
        close(fd1);
		close(fd2);
        errno=olderrno;
        err(3, "Cannot open file f3");
    }

	struct pair_t pair;

	while(read(fd1, &pair, sizeof(pair)))
	{
		off_t check = lseek(fd2, (pair.start-1)*sizeof(uint32_t), SEEK_SET);
		if (check < 0)
		{
			int olderrno = errno;
			close(fd1);
			close(fd2);
			close(fd3);
			errno = olderrno;
			err(7, "Error in positions");
		}

	//	uint32_t len = pair.length;
		uint32_t storage;
		uint32_t number_count = 0;
		while (read(fd2, &storage, sizeof(storage)))
		{
			write(fd3, &storage, sizeof(storage)); //?
			number_count++;
			if (number_count == pair.length)
			{
				break;
			}
		}

	}

	close(fd1);
	close(fd2);
	close(fd3);
	exit(0);
}
