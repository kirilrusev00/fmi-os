// T2 - Да се напише програма на C, която изпълнява команда ls с точно един аргумент.

#include <stdlib.h>
#include <unistd.h>
#include <err.h>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		errx(1, "Usage: %s name", argv[0]);
	}

	if (execlp("ls", "ls", argv[1], (char*) NULL) == -1) {
		err(2, "Error while executing ls");
	}
	
	exit(0);
}
