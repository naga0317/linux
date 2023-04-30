#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
   int pid=fork();
   int ret=0;

   if(pid!=0)
   {
     printf("parent:%d\n", getpid());
     exit(0);
   }
   else
   {
     printf("child:%d\n", getppid());
     sleep(3);
     printf("child:%d\n", getppid());
   } 
}

