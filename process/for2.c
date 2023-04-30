#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>

void charatatime(int fd, char *);
void set_fl(int fd, int flags);

int main()
{
	pid_t pid;
	int fd;
	if((fd = open("race.txt",(O_WRONLY | O_CREAT | O_TRUNC),S_IRUSR | S_IWUSR)) < 0)
	    printf("open error");

	if((pid = fork()) < 0 )
		printf("fork error");
	else if (pid == 0)
	    charatatime(fd, "this is a long long output from child\n");
	else
            charatatime(fd, "this is a long long output from Parent\n");

	exit(0);
}

void charatatime(int fd, char *str)
{
	set_fl(fd,O_SYNC);
	while(*str) 
	{
		write(fd, str++,1);
		fdatasync(fd);
	}
}

void set_fl(int fd, int flags) {
int val;
 if((val = fcntl(fd,F_GETFL,0)) < 0)
     printf("fcntl F_GETFL error");
 val |= flags;
 if(fcntl(fd,F_SETFL, val) < 0)
 	 printf("fcntl F_SETFL error");
}
