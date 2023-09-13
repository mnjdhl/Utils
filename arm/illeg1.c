#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#if defined(__arm__)
#include <asm/sigcontext.h>
#endif
//void sig_handler(int signum, siginfo_t *info, void *ucontext) {
void sig_handler(int signum) {
  printf("\n%s:signal %d\n", __FUNCTION__, signum);
#if defined(__arm__)

#endif
  exit(-1);
}

void DoRegSigHandler() {

  struct sigaction sa;

  memset(&sa, 0, sizeof(struct sigaction));
  //sa.sa_handler = SIG_IGN;
  sa.sa_handler = sig_handler;
  //sigaction(SIGPIPE, &sa, NULL);
  sigaction(SIGILL, &sa, NULL);
}

int main() {

    /* To avoid the crash catch the SIGILL signal */
	DoRegSigHandler();
    /* The below line causes core dump in ARM machinei (e.g. aarch64) */
	asm volatile (".word 0xe7f0000f\n");

	return 0;
}
