#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define INTERVAL 2

int howmany = 0;

void alarm_wakeup (int i)
{
   struct itimerval tout_val;

   signal(SIGALRM,alarm_wakeup);

   howmany += INTERVAL;

   printf("\n%d sec up partner, Wakeup!!!\n",howmany);
   tout_val.it_interval.tv_sec = 0;
   tout_val.it_interval.tv_usec = 0;
   tout_val.it_value.tv_sec = INTERVAL; /* 10 seconds timer */
   tout_val.it_value.tv_usec = 0;

   setitimer(ITIMER_REAL, &tout_val,0);

}

void exit_func (int i)
{
    signal(SIGINT,exit_func);
    printf("\nBye Bye!!!\n");
    exit(0);
}

int main ()
{
  struct itimerval tout_val;

  tout_val.it_interval.tv_sec = 0;
  tout_val.it_interval.tv_usec = 0;
  tout_val.it_value.tv_sec = INTERVAL; /* 10 seconds timer */
  tout_val.it_value.tv_usec = 0;
  setitimer(ITIMER_REAL, &tout_val,0);

  //alarm(1);

  signal(SIGALRM,alarm_wakeup); /* set the Alarm signal capture */
  signal(SIGINT,exit_func);

  while (1)
  {
     //printf("hello\n");
  }

  return 0;

}
