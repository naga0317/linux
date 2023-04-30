#include  <stdio.h>
#include  <string.h>
#include  <stdlib.h>
#include  <unistd.h>
#include  <errno.h>

int main ()
{
    printf ("I am the parent process pid:%d\n", getpid());
    char *arg1= "hello";
    char *arg2= "world";
    char *const env_var[]={"KESAVA","8080", NULL};

    execle("/home/naga/linux/process/execle_child", arg1,arg2,NULL,env_var);
    printf("parent after exec");
    return 0;
}
