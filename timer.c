#include <stdio.h>
#include <time.h>

int main()
{
	int iterations=0;
	int msec = 0, trigger = 10000; /* 10s => (10000*pow(10,-3)) */
	clock_t before = clock();

	do {
		clock_t difference = clock() - before;
		msec = difference * 1000 / CLOCKS_PER_SEC;
		iterations++;
	} while ( msec < trigger );

	printf("Time taken %d seconds %d milliseconds (%d iterations)\n",
			msec/1000, msec%1000, iterations);
}
