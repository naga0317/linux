#include  <stdio.h>
#include  <string.h>
#include  <stdlib.h>
#include  <unistd.h>

int main(void) 
{
  /* execute binary path using env export as following 
   * before executing the program 
   * 'export PATH= "$PATH:/home/naga/linux/process"*/
  char *arg1 = "hello";
  char *arg2 = "world";

  printf("In parent %d\n", getpid()); 
  execlp("child", arg1, arg2, NULL);

  return 0;
}
