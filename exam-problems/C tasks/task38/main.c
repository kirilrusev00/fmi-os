#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

void tr_d(char* set) {
	char rem[256] = { 0 };
	for (size_t i = 0; i < strlen(set); i++) {
		rem[(int) set[i]] = 1;
	}
	char c;
	while (read(0, &c, sizeof(c)) == sizeof(c)) {
		if (rem[(int) c] == 0) {
			write(1, &c, sizeof(c));
		} 
/*
	char *ptr = strchr(set, c);
	if (ptr!=NULL) {
		write(1, &c, sizeof(c));
	} 
*/
	}
}

void tr_s(char* set) {
	char rem[256] = { 0 }; 
    for (size_t i = 0; i < strlen(set); i++) {
        rem[(int) set[i]] = 1;
    }
	
	char c_prev;
	char c;
	if (read(0, &c_prev, sizeof(c_prev)) < 0) {
		err(2, "failed read from stdin");
	}
	write(1, &c_prev, sizeof(c_prev));
	while (read(0, &c, sizeof(c)) == sizeof(c)) {
        if (rem[(int) c] == 0 || c != c_prev) {
			write(1, &c, sizeof(c));
        }
		c_prev = c;
    }
}

void tr(char* set1, char* set2) {
	if (strlen(set1) != strlen(set2)) {
		errx(3, "set1 and set2 must have equal size");
	}

	char rem[256];
	for (size_t i = 0; i < 256; i++) {
		rem[i] = (char) i;
	}

    for (size_t i = 0; i < strlen(set1); i++) {
        rem[(int) set1[i]] = set2[i];
    }

	char c;
    while (read(0, &c, sizeof(c)) == sizeof(c)) {
		write(1, &rem[(int) c], sizeof(rem[(int) c]));
    }
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		errx(1, "wrong argument count");
	}

	if (strcmp(argv[1], "-d") == 0) {
		tr_d(argv[2]);
		exit(0);
	}
	else if (strcmp(argv[1], "-s") == 0) {
        tr_s(argv[2]);
		exit(0);
    }

	tr(argv[1], argv[2]);
	exit(0);
}
