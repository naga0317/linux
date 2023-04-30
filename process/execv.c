#include  <stdio.h>
#include  <string.h>
#include  <stdlib.h>
#include  <unistd.h>
#include  <errno.h>

int main ()
{
    printf ("I am the parent process pid:%d\n", getpid());
    char *arg_Ptr[5]={"child.c", "hello", "child", NULL};
    /*arg_Ptr[0] = "child.c";
    arg_Ptr[1] = "Hello from ";
    arg_Ptr[2] = "process 2 ";
    arg_Ptr[3] = NULL; */

    execv("/home/naga/linux/process/child", arg_Ptr);
    printf("parent after exec");
    return 0;
}
