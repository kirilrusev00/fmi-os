#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>
#include <stdio.h>
#include <errno.h>

int cmp(const void *a, const void *b) {
//	return *((uint32_t)*)a - *((uint32_t)*)b;
	if (*((uint32_t*)a) > *((uint32_t*)b) ) {
		return 1;
	} else if (*((uint32_t*)a) < *((uint32_t*)b) ) {
		return -1;
	}
	return 0;
}

int main() {

	struct stat st;
	if (stat("f1", &st) < 0) {
		err(1,"failed stat");
	}

	if (st.st_size % sizeof(uint32_t) != 0) {
		errx(2, "wrong count");
	}

	uint32_t numel=st.st_size / sizeof(uint32_t);
	printf("%d\n", numel);
	uint32_t half=numel/2;
	uint32_t *p=malloc(half*sizeof(uint32_t));
	if (!p) {
		err(3, "failed malloc");
	}
	
	int fd1=open("f1", O_RDONLY);	
	if (fd1 == -1) {
		free(p);
		err(4,"failed open f1");
	}

	int t1=open("temp1", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (t1 == -1) {
		const int _errno = errno;
		free(p);
		close(fd1);
		errno=_errno;
		err(5,"failed to open temp1");
	}

//	if (read(fd1,p,half*sizeof(uint32_t)) != half*sizeof(uint32_t)) {
	size_t res = read(fd1,p,half*sizeof(uint32_t));
	if(res != half*sizeof(uint32_t)) {
		const int _errno = errno;
        free(p);
        close(fd1);
    	close(t1);
	    errno=_errno;
        err(6,"failed to read f1");
	}

	qsort(p,half,sizeof(uint32_t),cmp);

	res=write(t1,p,half*sizeof(uint32_t));
	if(res != half*sizeof(uint32_t)) {
		const int _errno = errno;
        free(p);
        close(fd1);
        close(t1);
        errno=_errno;
        err(7,"failed to write t1");
	}

//	close(t1);
	free(p);

	uint32_t rhalf=numel-half;
	uint32_t *rp=malloc(rhalf*sizeof(uint32_t));

	if (!rp) {
		const int _errno=errno;
		close(fd1);
		errno=_errno;
		err(8,"failed malloc");
	}

	int t2=open("temp2", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (t2 < 0) {
		const int _errno = errno;
		close(fd1);
		free(rp);
		errno = _errno;
		err(9,"failed to open temp2");
	}

	res=read(fd1,rp,half*sizeof(uint32_t));
	if(res != rhalf*sizeof(uint32_t)) {
		const int _errno = errno;
        close(fd1);
		close(t2);
        free(rp);
        errno = _errno;
        err(10,"failed to read f1");
	}
	
	qsort(rp,rhalf,sizeof(uint32_t),cmp);

	res=write(t2,rp,rhalf*sizeof(uint32_t));
	if (res != rhalf*szieof(uint32_t)) {
		const int _errno = errno;
        close(fd1);
		close(t2);
        free(rp);
        errno = _errno;
        err(11,"failed to write temp2");
	}

//	close(t2);
	free(rp);
	close(fd1);

	lseek(t1,0,SEEK_SET);
	lseek(t2,0,SEEK_SET);

	int fd2=open("f2",O_CREATE | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd2 < 0) {
		const int _errno = errno;
		close(t1);
		close(t2);
		errno=_errno;
		err(12,"failed to open f2");
	}

	uint32_t a;
	uint32_t b;

	while ((read(t1, &a, sizeof(a)) == sizeof(a)) && (read(t2, &b, sizeof(b)) == sizeof(b))) {
		if (a <= b) {
			write(fd2, &a, sizeof(a));
			lseek(t2,-1*sizeof(b),SEEK_CUR);
		} else {
			write(fd2, &b, sizeof(b));
			lseek(t1,-1*sizeof(a),SEEK_CUR);
		}
	}

	while (read(t1, &a, sizeof(a)) == sizeof(a)) {
		write(fd2, &a, sizeof(a));
	}

	while (read(t2, &b, sizeof(b)) == sizeof(b)) {
        write(fd2, &b, sizeof(b));
    }

	close(t1);
	close(t2);
	close(fd2);

	exit(0);
}

/*
	f1 - input file	// uint32_t
	f2 - output file (sorted)
	t1,t2 -- temp files	

	filesize(f1) % sizeof(uint32_t) == 0

	100 000 000 * uint32_t ~~ 380 MB
	50M uint32_t ~~ 190 MB < limit 256 MB

	numel = filesize(f1) / sizeof(uint32_t)

	read numel/2 elements from f1 in RAM
	qsort
	dump in t1

	read rest from f1 in RAM
	qsort
	dump in t2

	close(f1)

	a, 	b -> c
	n	m

	i = 0
	j = 0
	k = 0

	while (i<n && j<m) {
		if a[i] <= b[j] {
			c[k++] = a[i++]
		} else {
			c[k++] = b[j++]
		}
	}

	while (i < n) {
		c[k++] = a[i++]
	}
	while (j < m) {
		c[k++] = b[j++]
	}
*/
