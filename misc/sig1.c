#include<stdio.h>
#include<signal.h>
#include<unistd.h>

void sig_handler(int signo)
{
	  if (signo == SIGINT)
	    printf("\nReceived SIGINT\n");
}

int main(void)
{
  int cnt=0;

	  if (signal(SIGINT, sig_handler) == SIG_ERR)
		printf("\ncan't catch SIGINT\n");

	  // A long long wait so that we can easily issue a signal to this 
	  // process
	  while(cnt < 1000) 
	  {
		    sleep(1);
		    cnt++;
	  }
	  return 0;
}
