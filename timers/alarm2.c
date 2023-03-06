#include <stdio.h>
#include <unistd.h>
#include <signal.h>

static void alarmHandler(int signo);

int main(void)
{

    alarm(5);

    signal(SIGALRM, alarmHandler);

    pause();

    return 0;
}

static void alarmHandler(int signo)
{
    printf("Alarm signal sent!\n");

}
