#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	if (argc == 1) {
		printf("사용법: %s <실행 명령> [인수0, 인수1, 인수2, ...]\n", argv[0]);
		return 1;
	}

	pid_t pid = fork();
	if (pid < 0) {
		perror("fork 오류");		
  		return 1;		        
	}

	if (pid == 0) {
		execvp(argv[1], argv + 1);		
		perror("execvp 오류");  
		exit(EXIT_FAILURE);				      
	} else {
					       	  
	return 0; 
}

