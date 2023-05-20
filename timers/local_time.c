#include <stdio.h>
#include <time.h>

int main(int argc, const char * argv[])
{
    /* Define temporary variables */
    time_t current_time;
    struct tm *local_time;

    /* Retrieve the current time */
    current_time = time(NULL);

    /* Get the local time using the current time */
    local_time = localtime(&current_time);

    /* Display the local time */
    printf("The time is: %s", asctime(local_time));

    return 0;
}
