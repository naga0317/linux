// C++ program to illustrate the
// gmtime() function
#include <stdio.h>
#include <time.h>
#define CST (+8)
#define IND (-5)

int main()
{

	// object
	time_t current_time;

	// pointer
	struct tm* ptime;

	// use time function
	time(&current_time);

	// gets the current-time
	ptime = gmtime(&current_time);

	// print the current time
	printf("Current time:\n");

	printf("Beijing ( China ):%2d:%02d:%02d\n",
		(ptime->tm_hour + CST) % 24, ptime->tm_min, ptime->tm_sec);

	printf("Delhi ( India ):%2d:%02d:%02d\n",
		(ptime->tm_hour + IND) % 24, ptime->tm_min, ptime->tm_sec);
	return 0;
}

