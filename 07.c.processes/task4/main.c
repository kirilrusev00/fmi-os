// T4 - Да се напише програма на C, която създава процес дете и демонстрира принцина на конкурентност при процесите.

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
	int n=100;

	pid_t pid = fork();

	if (pid > 0) {
		for (int i = 0; i < n; i++) {
			printf("parent\n");
		}
	} else {
		for (int i = 0; i < n; i++) {
            printf("child\n");
        }
	}

	exit(0);
}
