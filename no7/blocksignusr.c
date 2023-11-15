#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler1(int);
void handler2(int);

sigset_t set;

int main()
{
	struct sigaction act;
	/* Set up signal set with just SIGUSR1. */
	sigemptyset(&set);		   
       	sigaddset(&set, SIGUSR1);

		       
       	/* Trap SIGUSR1. */
	sigemptyset(&act.sa_mask);			   
       	act.sa_flags = 0;			       
       	act.sa_handler = handler1;			
	sigaction(SIGUSR1, &act, NULL);

				  
  	/* Trap SIGUSR2 - just change handler in action. */
	act.sa_handler = handler2;					
    	sigaction(SIGUSR2, &act, NULL);				

	while (1)				
	{
		/* We will get a USR2 first because USR1 is not blocked. */	
		pause();									   
   	}
			
	return 0;
}

void handler1(int sig)
{
	    printf("Got a SIGUSR1 signal\n");
}

void handler2(int sig)
{
	    printf("Got a SIGUSR2 signal\n");
}

