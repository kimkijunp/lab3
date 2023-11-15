#include <stdio.h>
#include <signal.h>

int main() {
	  
      	sigset_t set1, set2;
	sigfillset(&set1);  			   
	sigemptyset(&set2); 				    
	sigaddset(&set2, SIGINT);

	printf("Critical region start.\n");	
	
       	if (sigprocmask(SIG_BLOCK, &set1, NULL) == -1) {
		perror("sigprocmask");
		return 1;			     
	}
	
   	sleep(5);

	printf("Less critical region start.\n");


	if (sigprocmask(SIG_UNBLOCK, &set2, NULL) == -1) {		
		perror("sigprocmask");
		return 1;				   
       	}

	sleep(5);
	printf("Non critical region start.\n");
 
	if (sigprocmask(SIG_UNBLOCK, &set1, NULL) == -1) {
       		perror("sigprocmask");
		return 1;
			     
	}
    
       	sleep(5); 
      	return 0;
}
