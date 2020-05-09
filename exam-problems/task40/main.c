#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <err.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

// ./main -c [N | N-M]
// ./main -d [STRING] -f [N | N-M]

void cut_c(int start, int end) {
	int count = 0;
	char c;
	while (read(0, &c, sizeof(c)) > 0) {
		count++;
		if (c == '\n') {
            printf("%c", c);
            count = 0;
        }
		if (start <= count && count <= end) {
			printf("%c", c);
		}
	}
}

void cut_d(char delim, int start, int end) {
    int count = 1;
    char c;
    while (read(0, &c, sizeof(c)) > 0) {
        if (c == '\n') {
            printf("%c", c);
            count = 1;
			continue;
        }
		if (c == delim) {
            count++;
			if (start == count) { continue; }
        }
        if (start <= count && count <= end) {
            printf("%c", c);
        }
    }
}

int main(int argc, char* argv[]) {
	if (argc != 3 && argc != 5) {
		errx(1, "wrong argument count");
	}

	if (strcmp(argv[1], "-c") == 0) {
		int start = 0;
		int end = 0;
//		get_start_end(argv[2], start, end);
		if (strlen(argv[2]) == 1) {
        char s = argv[2][0];
        if (s < '1' || '9' < s) {
            errx(2, "second argument not a number");
        }
        start = s - '0';
        end = start;
    	} else if (strlen(argv[2]) == 3) {
        	char s = argv[2][0];
        	char e = argv[2][2];
        	if (s < '1' || '9' < s || argv[2][1] != '-' || e < '1' || '9' < e) {
            	errx(3, "second argument not a range of numbers");
        	}
        	start = argv[2][0] - '0';
        	end = argv[2][2] - '0';
    	} else {
        	errx(4, "wrong second argument");
    	}
		cut_c(start, end);
	}
	
	if (strcmp(argv[1], "-d") == 0 && strcmp(argv[3], "-f") == 0) {
        char delim = argv[2][0];
		int start = 0;
        int end = 0;
        if (strlen(argv[4]) == 1) {
        char s = argv[4][0];
        if (s < '1' || '9' < s) {
            errx(5, "fourth argument not a number");
        }
        start = s - '0';
        end = start;
        } else if (strlen(argv[4]) == 3) {
            char s = argv[4][0];
            char e = argv[4][2];
            if (s < '1' || '9' < s || argv[4][1] != '-' || e < '1' || '9' < e) {
                errx(6, "fourth argument not a range of numbers");
            }
            start = argv[4][0] - '0';
            end = argv[4][2] - '0';
        } else {
            errx(7, "wrong fourth argument");
        }
        cut_d(delim, start, end);
    }


	exit(0);
}
