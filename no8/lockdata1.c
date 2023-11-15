#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define TEST_FILE "testlock"

struct flock acquire_lock() {
	struct flock fl;
	       
       	fl.l_type = F_WRLCK;  /* Write lock */
	fl.l_whence = SEEK_SET;  /* Start from beginning of file */		  
  	fl.l_start = 0;  /* Set the start of the lock region */			 
     	fl.l_len = 0;  /* Extend the lock to the end of the file */			     
     	fl.l_pid = getpid();  /* Set the process ID */
				 
     	return fl;
}

int main() {
	int fd;	    
    	char buf[16];	
	struct flock fl = acquire_lock();
		     
     	if ((fd = open(TEST_FILE, O_WRONLY | O_CREAT, 0666)) == -1) {
		perror("open");				       
	       	exit(1);			
	}
			
    	if (fcntl(fd, F_SETLKW, &fl) == -1) {
		perror("fcntl");				     
	 	exit(1);				
	}
			
	snprintf(buf, sizeof(buf), "Data from process %d", getpid());

	if (write(fd, buf, strlen(buf)) == -1) {
		perror("write");						
    		exit(1);				
	}
			
	fl.l_type = F_UNLCK;  /* Unlock the file */

	if (fcntl(fd, F_SETLK, &fl) == -1) {
		perror("fcntl unlock");					
		exit(1);				
	}
			
	close(fd);

	printf("process %d: wrote \"%s\" to testlock\n", getpid(), buf);

	return 0;
}

