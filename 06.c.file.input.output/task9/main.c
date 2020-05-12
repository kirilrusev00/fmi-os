/*
Напишете програма, която приема точно 2 аргумента. Първият може да бъде само --min, --max или --print (вижте man 3 strcmp). Вторият аргумент е двоичен файл, в който има записани цели неотрицателни двубайтови числа (uint16_t - вижте man stdint.h). Ако първият аргумент е:

--min - програмата отпечатва кое е най-малкото число в двоичния файл.
--max - програмата отпечатва кое е най-голямото число в двоичния файл.
--print - програмата отпечатва на нов ред всяко число.
Използвайте двоичния файл binary/dump или генерирайте сами такъв (правилно подравнен).
*/

//Модифицирайте предната програма, така че да намерите най-големия байт.

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

void print_min(int fd) {
	uint16_t min;
	uint16_t c;
	if (read(fd, &c, sizeof(c)) > 0) {
        min = c;
    } else {
		printf("file is empty");
		return;
	}

	while (read(fd, &c, sizeof(c)) > 0) {
		if (c < min) {
			min = c;
		}
	}
	printf("%d\n", min);
}

void print_max(int fd) {
    uint16_t max;
    uint16_t c;
    if (read(fd, &c, sizeof(c)) > 0) {
        max = c;
    } else {
        printf("file is empty");
        return;
    }

    while (read(fd, &c, sizeof(c)) > 0) {
        if (c > max) {
            max = c;
        }
    }
    printf("%d\n", max);
}

void print_max_byte(int fd) {
    char max;
    char c;
    if (read(fd, &c, sizeof(c)) > 0) {
        max = c;
    } else {
        printf("file is empty");
        return;
    }

    while (read(fd, &c, sizeof(c)) > 0) {
        if (c > max) {
            max = c;
        }
    }
    printf("MAX BYTE: %d\n", c);
}

void print(int fd) {
    uint16_t c;
    while (read(fd, &c, sizeof(c)) > 0) {
		printf("%d\n", c); 
    }
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		errx(1, "Wrong arguments");
	}
	
	uint8_t choice;
	if (strcmp("--min", argv[1]) == 0) {
		choice = 1;
	} else if (strcmp("--max", argv[1]) == 0) {
        choice = 2;
    } else if (strcmp("--print", argv[1]) == 0) {
        choice = 3;
    } else if (strcmp("--max-byte", argv[1]) == 0) {
        choice = 4;
	} else {
		errx(2, "Wrong first argument");
	}

	struct stat st;
	if (stat(argv[2], &st) < 0) {
		err(4, "failed stat");
	}
	off_t size = st.st_size;
	if (size % sizeof(uint16_t) != 0) {
		errx(3, "wrong binary file");
	}

	int fd = open(argv[2], O_RDONLY);
	if (fd < 0) {
		err(5, "failed to open binary");
	}

	switch(choice) {
		case 1:
			print_min(fd);
			break;
		case 2:
            print_max(fd);
            break;
		case 3:
            print(fd);
            break;
		case 4:
            print_max_byte(fd);
            break;
	}

	close(fd);

	exit(0);
}
