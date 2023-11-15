#include <syslog.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
void demonize(char *cmd) {
	int i, fd0, fd1, fd2;	       
       	pid_t pid;		 
     	struct rlimit rl;		  
 	struct sigaction sa;			 
     	umask(0); /* 파일 생성 마스크 초기화 */
			        			    
    	if (getrlimit(RLIMIT_NOFILE, &rl) < 0)				
		perror("getrlimit");
				    				  
      	if ((pid = fork()) < 0)				
		exit(1);			
	else if (pid != 0)			
		exit(0); /* 부모 프로세스는 종료 */
					    			
	setsid(); /* 세션 리더로 변환 */
					        					   
   	sa.sa_handler = SIG_IGN;						
    	sigemptyset(&sa.sa_mask);				
	sa.sa_flags = 0;
							    				
	if (sigaction(SIGHUP, &sa, NULL) < 0)							
		perror("sigaction: SIGHUP 무시할 수 없습니다.");
					
	if ((pid = fork()) < 0)					
		exit(1);			
	else if (pid != 0)			
		exit(0); /* 부모 프로세스는  종료 */			
	chdir("/"); /* 작업 디렉토리 변경 */
									    	
	if (rl.rlim_max == RLIM_INFINITY)									
		rl.rlim_max = 1024;

	for (i = 0; i < rl.rlim_max; i++)
		close(i);
					

	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);

	openlog(cmd, LOG_CONS, LOG_DAEMON);

	if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
		syslog(LOG_ERR, "예상치 못한 파일 디스크립터 %d %d %d", fd0, fd1, fd2);
		exit(1);

	}
}



int main(int argc, char *argv[]) {
	demonize(argv[1]);
	sleep(10);
	syslog(LOG_INFO, "이것은 데몬 테스트 프로세스입니다.");
	sleep(100);
	closelog();
	return 0;

}

