#include  <stdio.h>
#include  <string.h>
#include  <stdlib.h>
#include  <unistd.h>

int main(void) 
{
  char *binaryPath = "/home/naga/linux/process/child";
  char *arg1 = "hello";
  char *arg2 = "world";

  printf("In parent %d\n", getpid()); 
  execl(binaryPath, arg1, arg2, NULL);
  //execl(binaryPath, binaryPath, arg1, arg2, NULL);

  return 0;
}
