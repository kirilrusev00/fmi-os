// T5 - Да се напише програма на C, която е аналогична на горния пример, но принуждава бащата да изчака сина си да завърши.

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
	int n=100;

	pid_t pid = fork();

	if (pid > 0) {
		wait(NULL);
		for (int i = 0; i < n; i++) {
			printf("parent\n");
		}
		//wait(NULL);
	} else {
		for (int i = 0; i < n; i++) {
            printf("child\n");
        }
	}

	exit(0);
}
