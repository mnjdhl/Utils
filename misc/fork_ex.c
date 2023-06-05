#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int cnt =0;
void forkexample(int ind)
{
    // child process because return value zero
	int pid = fork() ;
    if (pid == 0) {
    	cnt++;
        printf("\n%d. Hello from Child! pid = %d, cnt = %d ", ind, getpid(), cnt);
    }
    // parent process because return value non-zero.
    else {
    	cnt++;
        printf("\n%d. Hello from Parent! pid = %d, cnt = %d ", ind, getpid(), cnt);
    }
}
int main()
{

    for (int i=0;i<10;i++)
	    forkexample(i+1);
    return 0;
}

/*
 *A process executes the following code

for (i = 0; i < n; i++) fork();

The total number of child processes created is
(A) n
(B) 2^n - 1
(C) 2^n
(D) 2^(n+1) - 1


Answer: (B)

Explanation:

         F0       // There will be 1 child process created by first fork
      /     \
    F1      F1    // There will be 2 child processes created by second fork
   /  \    /  \
 F2   F2  F2   F2  // There will be 4 child processes created by third fork
/ \   / \ / \  / \
 ...............   // and so on

If we sum all levels of above tree for i = 0 to n-1, we get 2^n - 1. So there will be 2^n – 1 child processes. On the other hand, the total number of process created are (number of child processes)+1.
 *
 *
 */
