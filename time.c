#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	time_t timep;
	time(&timep);

	printf("%s", ctime(&timep));
	return 0;
}

