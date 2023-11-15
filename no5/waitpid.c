#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void fatal(const char *message) {	   
       	perror(message);	    
    	exit(EXIT_FAILURE);
}
int main() {
	 
     	pid_t pid;
	int status, exit_status;		  
      	if ((pid = fork()) < 0)			        
	    	fatal("fork 실패");		     
     	if (pid == 0) /* 자식 */				   
       	{				
		sleep(4); /* 수행을 4초 동안 중단 */	
		exit(5);						
	}


  while (waitpid(pid, &status, WNOHANG) == 0) {
 	  printf("아직 기다리는 중... \n");
   	  sleep(1);
    }
  
  if (WIFEXITED(status)) {
	  exit_status = WEXITSTATUS(status);	       
	  printf("프로세스 %d의 종료 상태는 %d였습니다. \n", pid, exit_status);		          
  }
  
  exit(0);

}    
