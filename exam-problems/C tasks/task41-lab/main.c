
// ./main input.bin output.bin

/**

man 3 qsort ... in-place

Counting sort:
0 -> 0
1 -> 2
2 -> 3
3 -> 0
4 -> 10

...

65536 -> 12

uint16_t stats[1<<16];

MAX_SIZE = (1<<16 - 1) * sizeof(uint16_t)

*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>

int main(int argc, const char* argv[]) {

/*
	while(read input.bin file)
		keep statistics for the numbers in an array

	foreach element in the counting-sort-array
		 write in output.bin
	
*/
	if(argc != 3)
	{
		errx(1, "Usage: ./main input.bin output.bin");
	}
	
	struct stat st;
	if(stat(argv[1], &st) < 0)
	{
		err(2, "Failed stat for file %s", argv[1]);
	}
	
	if(st.st_size % sizeof(uint16_t) != 0)
	{
		errx(3, "Invalid size of file %s", argv[1]);
	}
	
	if(st.st_size / sizeof(uint16_t) > 0xFFFF)
	{
		warnx("Size of %s is greater than 0xFFFF. Problems may occur", argv[1]);
	}
	
	int fdinput = open(argv[1], O_RDONLY);
	if(fdinput == -1)
	{
		err(4, "Failed to open file %s", argv[1]);
	}	
	
	size_t MAX_NUMBER = 65535; // 0xFFFF

	uint16_t counting[MAX_NUMBER + 1];
 	for (int i = 0; i <= MAX_NUMBER; i++)
	{
		counting[i] = 0;
	}

	printf("initialized counting array");
 	
	size_t bufsize = sizeof(uint16_t)*1024;	
	uint16_t *buffer = (uint16_t*) malloc (bufsize);
	
	if(buffer == NULL) 
	{
		errx(6, "Memory error in allocation");
	}

	ssize_t readsize;
	while((readsize = read(fdinput, buffer, sizeof(bufsize))) > 0)
	{
		for(int i = 0; i < readsize; i++)
		{
			counting[buffer[i]]++;
		}		
	}

	close(fdinput);
	
	int fdoutput = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
	if(fdoutput == -1)
	{
		const int olderrno = errno;
		close(fdinput);
		errno = olderrno;
		err(4, "Failed to open file %s", argv[2]);
	}

	// uint16_t = [0; 65535] = [0x0000; 0xFFFF] little endian

	ssize_t position = 0;
	uint32_t i = 0;
	while(i <= MAX_NUMBER) // iterate over each number
	{
		position = 0;
		
		// fill the buffer
		while(counting[i] > 0 && position < bufsize - 1) 
		{	
			if (counting[i] == 0) {
				++i; // move to next number
			} else {
				buffer[position] = i;
				position++;
				counting[i]--;
			}
		}
		
		// write the buffer to the file
		if (position != write(fdoutput, buffer, position)) {
			err(5, "cannot write %ld bytes to output file", position);
		}
	}
	
	close(fdoutput);	
	free(buffer);

	exit(0);
}

