#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define THIS_PROCESS 2
#define THAT_PROCESS 1

int main() {

	int fd;
	struct flock testlock;
	int len;
	char buf[20];


	testlock.l_type = F_RDLCK;
	testlock.l_whence = SEEK_SET;
	testlock.l_start = 0;
	testlock.l_len = 0;


	fd = open("testlock", O_RDWR);
	if (fd == -1) {
		perror("open");
		exit(1);
	}

	if (fcntl(fd, F_SETLKW, &testlock) == -1) {
		fprintf(stderr, "process %d: lock failed", THIS_PROCESS);
		exit(1);
	}

	printf("process %d: locked successfully\n", THIS_PROCESS);

	len = read(fd, buf, sizeof(buf) - 1);
	if (len == -1) {
		perror("read");
		exit(1);
	}

	buf[len] = '\0';  


	printf("process %d: read \"%s\" from testlock\n", THIS_PROCESS, buf);
	printf("process %d: unlocking\n", THIS_PROCESS);


	testlock.l_type = F_UNLCK;
	if (fcntl(fd, F_SETLK, &testlock) == -1) {
		perror("fcntl unlock");
		exit(1);
	}

	close(fd);

	return 0;
}

