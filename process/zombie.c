#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

/* While parent was in sleep or in pause state
 * if Child got terminated, notification to 
 * parent gets being ignored and hence the child 
 * pid becomes Zombie */

int main()
{
 pid_t pid = fork();
   if(pid == 0)
   {
     printf("I'm child process %d\n",getpid());
     exit(0); // child exit //
   }
   else
   {
     int ret;
     sleep(30);//Parent is in sleep //
     printf("I'm Parent %d\n",getpid());
   }
 return 0;	 
}
