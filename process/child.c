#include  <stdio.h>
#include  <stdlib.h>
#include  <unistd.h>

int main(int argc, char *argv[])
{
    printf ("I am the child process pid:%d\n", getpid());
    int i;

    for(i=0; i<argc; i++)
    {
        printf("argv[%d]:%s\n", i, argv[i]);
    }
    return 0;
}
