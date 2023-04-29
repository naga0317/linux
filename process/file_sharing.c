#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void str(int fd, char *s);

int main()
{
	pid_t pid;
	int fd;
	if((fd = open("race.txt",(O_WRONLY | O_CREAT | O_TRUNC),S_IRUSR | S_IWUSR)) < 0)
	    printf("open error");

	if((pid = fork()) < 0 )
		printf("fork error");
	else if (pid == 0)
	    str(fd, "this is a long long output from child\n");
	else
            str(fd, "this is a long long output from Parent\n");

	exit(0);
}

void str(int fd, char *s)
{
	while(1)
	{
	    printf("%s\n", s);
	    write(fd, s, strlen(s));
	}
}
