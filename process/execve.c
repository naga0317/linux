#include  <stdio.h>
#include  <stdlib.h>
#include  <unistd.h>

int main ()
{
    printf ("I am the parent process pid:%d\n", getpid());
    char *arg_Ptr[5]={"hello", "world", NULL};
    char *const env[] = {"8080","KESAVA",NULL};
    execve("/home/naga/linux/process/execle_child",arg_Ptr,env);
    printf("parent after exec");
    return 0;
}
