#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
 
#define BUF_SIZE 8192
 
int main() 
{
 
    int fd;   
       
    char buffer[BUF_SIZE]="welcome to file"; 
    
    
    fd = open("temp.txt", O_WRONLY | O_CREAT, 0644);

    if(fd == -1)
    {
        perror("open");
        return 3;
    }
 
    write(fd,buffer,strlen(buffer));

    close (fd);
 
    return (EXIT_SUCCESS);
}
