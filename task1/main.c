/*
	Да се напише програма на C, която изпълнява команда date.
*/

#include <stdlib.h>
#include <unistd.h>
#include <err.h>

int main() {

	if (execlp("date", "date", (char*) NULL) == -1) {
		err(1, "Error while executing date");
	}

	exit(0);
}
