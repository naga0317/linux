#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
  int pid = fork();
  
  if(pid==0)
  {
    printf("From Child pid =%d, getpid =%d, getppid = %d\n",pid,getpid(),getppid());
  }
  else
  {

    printf("From Parent pid =%d, getpid =%d, getppid = %d\n",pid,getpid(),getppid());
  }
  return 0;
}

