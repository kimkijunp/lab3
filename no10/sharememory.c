#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>
#include <errno.h>
#include <unistd.h>

#define SHMSIZE 4096
#define SHMNAME "/my_shm"
#define SEMNAME "/my_sem"

void p(sem_t *semd);
void v(sem_t *semd);

int main(int argc, char* argv[]) {
	FILE *fp, *copy_fp;  
	char *path = argv[1]; 
	char buf[SHMSIZE]; 
	int shmd, count;
	void *shmaddr;
	sem_t *semd;
	pid_t pid;
	int i;


	
	if ((semd = sem_open(SEMNAME, O_CREAT, 0600, 1)) == SEM_FAILED) {
		perror("sem_open failed");
		exit(1);
	}   	

	sem_unlink(SEMNAME); 	
	memset(buf, 0, SHMSIZE); 	
	if ((pid = fork()) == 0) {




		
		if ((shmd = shm_open(SHMNAME, O_CREAT | O_RDWR, 0666)) == -1) {
			perror("shm_open failed");
			exit(1);
		}
		
		if (ftruncate(shmd, SHMSIZE) != 0) {
			perror("ftruncate failed");
			exit(1);
		}


		
		if ((shmaddr = mmap(0, SHMSIZE, PROT_WRITE, MAP_SHARED, shmd, 0)) == MAP_FAILED) {
			perror("mmap failed");
			exit(1);
		}
		
		if ((fp = fopen(path, "r")) == NULL) {
			perror("fopen");
			exit(1);
		}

		p(semd);
		printf("Child Process %d : semaphore in use\n", getpid());

		while ((count = fread(buf, sizeof(char), SHMSIZE, fp)) > 0) {
			memcpy(shmaddr, buf, count);
		}
		
		v(semd);
		printf("Child Process %d : releasing semaphore\n", getpid());

		
		if (munmap(shmaddr, SHMSIZE) == -1) {
			perror("munmap failed");
			exit(1);
		}
		
		fclose(fp);
		close(shmd);
		
		exit(0);
	} else if (pid > 0) {
		pid = wait(NULL);
		
		if ((shmd = shm_open(SHMNAME, O_RDWR, 0666)) == -1) {
			perror("shm_open failed");
			exit(1);
		}
		
		if ((shmaddr = mmap(0, SHMSIZE, PROT_READ, MAP_SHARED, shmd, 0)) == MAP_FAILED) {
			perror("mmap failed");
			exit(1);
		}
		
		if ((copy_fp = fopen("copy.txt", "w")) == NULL) {
			perror("fopen");
			exit(1);

		}

		p(semd);
		printf("Parent Process %d : semaphore in use\n", getpid());
		fwrite(shmaddr, sizeof(char), SHMSIZE, copy_fp);
		fclose(copy_fp);
		printf("Parent Process %d : releasing semaphore\n", getpid());
		v(semd);


		
		if (munmap(shmaddr, SHMSIZE) == -1) {
			perror("munmap failed");
			exit(1);
		}
		close(shmd);

		
		if (shm_unlink(SHMNAME) == -1) {
			perror("shm_unlink failed");
			exit(1);
		}

		if (sem_close(semd) == -1) {
			perror("sem_close failed");
			exit(1);
		}

	} else {
		perror("fork Error");
		exit(1);
	}
	return 0;
}

void p(sem_t *semd) {
	if (sem_wait(semd) == -1) {
		perror("sem_wait failed");
		exit(1);

	}
}



void v(sem_t *semd) {
	if (sem_post(semd) == -1) {
		perror("sem_post failed");
		exit(1);

	}
}
