#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define MAX_FLOW_RANGE 0xFFFFF

int main(int argc, char *argv[]) {
	int r, nloops = 10;
	unsigned int seed;
	time_t now;

	if (argc != 3) {
	   //fprintf(stderr, "Usage: %s <seed> <nloops>\n", argv[0]);
	   //exit(EXIT_FAILURE);
	   now = time(NULL);
	   seed = now;
	} else {
		seed = atoi(argv[1]);
		nloops = atoi(argv[2]);
	}

	srand(seed);
	for (int j = 0; j < nloops; j++) {
	   r =  (rand() % MAX_FLOW_RANGE) + 1;
	   printf("%d\n", r);
	}

	exit(EXIT_SUCCESS);
}
