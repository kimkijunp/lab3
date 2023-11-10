#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

#define BUFSIZE 512 

char practice_1[BUFSIZE] = "Hello world! input my text! i am yongjjang!";
char practice_2[BUFSIZE] = "Early bird catches the worm";
char practice_3[BUFSIZE] = "He is truly happy who makes others happy";

int main() {
	time_t start_time, end_time;	    
    	char a[20];
	char buffer[BUFSIZE];		   
   	char buf1[BUFSIZE]; 			   
       	char buf2[BUFSIZE]; 			    
    	char buf3[BUFSIZE]; 			
	int wrong_count = 0;			
	int sum = 0;			
	double total = 0;

		
	printf("타자 연습 프로그램입니다. 시작(Enter)\n");
	printf("<<<<<<<<<<<Press Enter>>>>>>>>>>>>\n");					        
	fgets(a, sizeof(a), stdin);						
	start_time = time(NULL);

				
	printf("%s\n", practice_1);				
	fgets(buf1, sizeof(buf1), stdin);				
	printf("%s\n", practice_2);					
	fgets(buf2, sizeof(buf2), stdin);				
	printf("%s\n", practice_3);					
	fgets(buf3, sizeof(buf3), stdin);				
	end_time = time(NULL);

	total = difftime(end_time, start_time);				       
	sum = strlen(buf1) + strlen(buf2) + strlen(buf3);										
	sum /= total / 60;

												       
	for (int i = 0; i < strlen(practice_1); i++)												
		if (buf1[i] != practice_1[i])
			wrong_count++;					
	for (int i = 0; i < strlen(practice_2); i++)				
		if (buf2[i] != practice_2[i])						
			wrong_count++;					
	for (int i = 0; i < strlen(practice_3); i++)				
		if (buf3[i] != practice_3[i])						
			wrong_count++;
					
	printf("잘못 타이핑한 횟수: %d\n", wrong_count);
	printf("평균 분당 타자수: %d\n", sum);							
	printf("총 걸린시간: %f\n", total);
	
	return 0;
}

