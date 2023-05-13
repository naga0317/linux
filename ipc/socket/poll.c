#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>

#define PORT 8080
#define MAX_CLIENTS 10

int main() 
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Create a TCP socket
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket to a port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if(listen(server_fd, MAX_CLIENTS) < 0) 
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    // Set up pollfd array
    struct pollfd fds[MAX_CLIENTS + 1];
    memset(fds, 0, sizeof(fds));
    fds[0].fd = server_fd;
    fds[0].events = POLLIN;

    int nfds = 1; // Number of file descriptors to monitor

    while (1) 
    {
        int ret = poll(fds, nfds, -1);
        if(ret < 0) 
	{
            perror("poll failed");
            exit(EXIT_FAILURE);
        }

        // Check for incoming connection
        if(fds[0].revents & POLLIN) 
	{
            if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
	    {
                perror("accept failed");
                exit(EXIT_FAILURE);
            }

            // Add new socket to the array
            fds[nfds].fd = new_socket;
            fds[nfds].events = POLLIN;
            nfds++;
        }

        // Check for I/O events on the sockets
        for(int i = 1; i < nfds; i++) 
	{
            if(fds[i].revents & (POLLIN | POLLOUT)) 
	    {
                valread = read(fds[i].fd, buffer, 1024);
                if(valread <= 0) 
		{
                    // Close the socket
                    close(fds[i].fd);

                    // Remove the socket from the array
                    fds[i] = fds[nfds-1];
                    memset(&fds[nfds-1], 0, sizeof(struct pollfd));
                    nfds--;
                    i--;
                } 
		else 
		{
                    printf("Message received: %s\n", buffer);
                    send(fds[i].fd, buffer, strlen(buffer), 0);
                }
            }
        }
    }

    return 0;
}

